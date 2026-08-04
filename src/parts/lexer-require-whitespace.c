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
#ifndef GUARD_TPP_LEXER_REQUIRE_WHITESPACE_C
#define GUARD_TPP_LEXER_REQUIRE_WHITESPACE_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "lexer.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_REQUIRE_WHITESPACE
static TPP_CONSTCALL TPP_WUNUSED bool TPPCALL
tpp_token_is_keyword_like(tpp_token_id tok) {
	switch (tok) {

#if TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL
	case TPP_TOK_CXX_RAW_STRING_LITERAL:
#endif /* TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL
	case TPP_TOK_CXX_WIDE_STRING_LITERAL:
#if TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL
	case TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL:
#endif /* TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL */
#if TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL
	case TPP_TOK_CXX_UTF8_STRING_LITERAL:
#if TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL
	case TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL:
#endif /* TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL */
#if TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL
	case TPP_TOK_CXX_UTF16_STRING_LITERAL:
#if TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL
	case TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL:
#endif /* TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL */
#if TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL
	case TPP_TOK_CXX_UTF32_STRING_LITERAL:
#if TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL
	case TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL:
#endif /* TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL */
#if TPP_HAVE_TOK_RAW_STRING_LITERAL
	case TPP_TOK_RAW_STRING_LITERAL:
#endif /* TPP_HAVE_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL
	case TPP_TOK_CXX_RAW_CHAR_LITERAL:
#endif /* TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL
	case TPP_TOK_CXX_WIDE_CHAR_LITERAL:
#if TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL
	case TPP_TOK_CXX_RAW_WIDE_CHAR_LITERAL:
#endif /* TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL */
#endif /* TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL */
#if TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL
	case TPP_TOK_CXX_UTF8_CHAR_LITERAL:
#if TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL
	case TPP_TOK_CXX_RAW_UTF8_CHAR_LITERAL:
#endif /* TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL */
#endif /* TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL */
#if TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL
	case TPP_TOK_CXX_UTF16_CHAR_LITERAL:
#if TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL
	case TPP_TOK_CXX_RAW_UTF16_CHAR_LITERAL:
#endif /* TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL */
#endif /* TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL */
#if TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL
	case TPP_TOK_CXX_UTF32_CHAR_LITERAL:
#if TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL
	case TPP_TOK_CXX_RAW_UTF32_CHAR_LITERAL:
#endif /* TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL */
#endif /* TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL */
#if TPP_HAVE_TOK_RAW_CHAR_LITERAL
	case TPP_TOK_RAW_CHAR_LITERAL:
#endif /* TPP_HAVE_TOK_RAW_CHAR_LITERAL */
	TPP_CASE_TPP_TOK_INT
	TPP_CASE_TPP_TOK_FLOAT
#if !TPP_HAVE_TOK_C_INT && !TPP_HAVE_TOK_C_FLOAT
	case TPP_TOK_0:
	case TPP_TOK_1:
	case TPP_TOK_2:
	case TPP_TOK_3:
	case TPP_TOK_4:
	case TPP_TOK_5:
	case TPP_TOK_6:
	case TPP_TOK_7:
	case TPP_TOK_8:
	case TPP_TOK_9:
#endif /* !TPP_HAVE_TOK_C_INT && !TPP_HAVE_TOK_C_FLOAT */
		return true;

	default: break;
	}
	return TPP_TOK_ISKEYWORD(tok);
}

/* Check if 2 tokens, when written directly adjacent to each other,
 * *might* (though not necessarily) result in a different set of
 * tokens when re-parsed. */
TPP_IMPL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_require_whitespace(tpp_lexer const *tpp_restrict self,
                             tpp_token_id lhs, tpp_token_id rhs) {
	bool lhs_is_keyword = tpp_token_is_keyword_like(lhs);
	bool rhs_is_keyword = tpp_token_is_keyword_like(rhs);
	if (lhs_is_keyword || rhs_is_keyword) {
		/* If both sides are keyword-like, always need whitespace.
		 *
		 * If only one side is keyword-like, don't need need space,
		 * since keyword-like can only form larger keywords when
		 * combined with another keyword-like (joining a keyword
		 * with something like a '+' token will never produce some
		 * larger keyword) */
		return lhs_is_keyword && rhs_is_keyword;
	}
	(void)self;

	/* This case here could be made way more complicated, since it could
	 * be made to includes stuff like:
	 * >> if (lhs == TPP_TOK_PLUS && rhs == TPP_TOK_EQUAL)
	 * >>     return tpp_lexer_has(<lexer>, TOK_PLUS_EQUAL); */

	switch (lhs) {
/*[[[deemon
import * from deemon;
import MC_TOKENS, charNames, tokenName from .config;

@@Mapping like @{"**": "TOK_STAR_STAR", "??=": "TRIGRAPHS"}
local featuresByCharSeq: {string: string} = Dict({
	"??=" : "TRIGRAPHS",
	"??(" : "TRIGRAPHS",
	"??/" : "TRIGRAPHS",
	"??)" : "TRIGRAPHS",
	"??'" : "TRIGRAPHS",
	"??<" : "TRIGRAPHS",
	"??!" : "TRIGRAPHS",
	"??>" : "TRIGRAPHS",
	"??-" : "TRIGRAPHS",
	"/" "*" : "TOK_C_COMMENT",
	"(*" : "TOK_PASCAL_COMMENT",
	"<!--" : "TOK_HTML_COMMENT",
	"//" : "TOK_CXX_COMMENT",
	"@@" : "TOK_AT_AT_COMMENT",
});

for (local charSeq, none, none: MC_TOKENS) {
	local features = featuresByCharSeq.get(charSeq, "");
	if (features)
		features += "||";
	features += "TOK_" + tokenName(charSeq);
	featuresByCharSeq[charSeq] = features;
}

@@Get all possible 2-token pairs that can form @charSeq. Only
@@character sequences part of @featuresByCharSeq, as well as
@@all 1-char sequences are returned
@@e.g.: @get2CharConstructionFormsOf("??=") = @{("?", "?="), ("??", "=")}
@@e.g.: @get2CharConstructionFormsOf("??-") = @{("??", "-")}
@@      (doesn't return @("?", "?-") because there is no `?-` token)
function get2CharConstructionFormsOf(charSeq: string): {(string, string)...} {
	for (local splitIndex: [1:#charSeq]) {
		local part1 = charSeq[:splitIndex];
		local part2 = charSeq[splitIndex:];
		if ((#part1 == 1 || part1 in featuresByCharSeq) &&
		    (#part2 == 1 || part2 in featuresByCharSeq))
			yield (part1, part2);
	}
}

@@All token-sequences that can form multi-character tokens,
@@with the key being the character-sequence of the first token,
@@and the value being all possible character-sequences for the
@@second token.
local mcFormingSequences: {string: HashSet with string} = Dict();

for (local mcCharSeq: featuresByCharSeq.keys) {
	for (local part1, part2: get2CharConstructionFormsOf(mcCharSeq)) {
		local pairedSequences = mcFormingSequences.get(part1);
		if (pairedSequences is none)
			mcFormingSequences[part1] = pairedSequences = HashSet();
		pairedSequences.insert(part2);
	}
}

local oneCharStartsWithTokens: {string: {string...}} = {
	"'" : {"TPP_TOK_C_CHAR", "TPP_TOK_BLOCK_CHAR_LITERAL"},
	'"' : {"TPP_TOK_C_STRING", "TPP_TOK_BLOCK_STRING_LITERAL"},
	'#' : {"'#'", "TPP_TOK_SHELL_COMMENT", "TPP_TOK_SOL_SHELL_COMMENT"},
	'/' : {"'/'", "TPP_TOK_SLASH_COMMENT", "TPP_TOK_SOL_SLASH_COMMENT"},
	'@' : {"'@'", "TPP_TOK_AT_COMMENT", "TPP_TOK_SOL_AT_COMMENT"},
};

function tokenSymbols(charSeq: string): {string...} {
	if (#charSeq == 1) {
		local result = oneCharStartsWithTokens.get(charSeq);
		return result ?? {f"'{charSeq.encode("c-escape")}'"};
	}
	return {"TPP_TOK_" + tokenName(charSeq)};
}

local reverseCharNames = Dict.Frozen(
	for (local charSeq, token: charNames)
		(token, charSeq)
);

function getTokenCond(tokenSymbol: string): string {
	if (tokenSymbol.startswith("'"))
		return "1"; // 1-char token
	try {
		local baseName = tokenSymbol.lsstrip("TPP_TOK_");
		local charSeq = "".join(
			for (local part: baseName.split("_"))
				reverseCharNames[part]
		);
		local features = featuresByCharSeq[charSeq];
		return " || ".join(
			for (local feature: features.split("||"))
				 "TPP_HAVE_" + feature
		);
	} catch (UnknownKey from errors) {
		if (tokenSymbol.startswith("TPP_TOK_"))
			return "TPP_HAVE_" + tokenSymbol.lsstrip("TPP_");
		throw;
	}
}

function joinCond(how: string, conditions...: string): string {
	local result = "";
	local otherHow = { "||": "&&", "&&": "||" }[how];
	for (local cond: conditions) {
		if (cond == "1") {
			if (how == "||")
				return "1";
			if (how == "&&")
				continue;
		} else if (cond == "0") {
			if (how == "||")
				continue;
			if (how == "&&")
				return "0";
		}
		if (result)
			result += f" {how} ";
		if (otherHow in cond)
			cond = f"({cond})";
		result += cond;
	}
	if (!result)
		return how == "&&" ? "1" : "0";
	while (result.startswith("(") && result.endswith(")") &&
	       result.findmatch("(", ")", 1) == (#result - 1))
		result = result[1:-1];
	return result;
}

function builtTppLexerHas(features: string): string {
	return " || ".join(
		for (local feature: features.split("||"))
				f"tpp_lexer_has(self, {feature})"
	);
}

for (local firstCharSeq, possibleExtraCharSeqs: mcFormingSequences) {
	assert possibleExtraCharSeqs;
	local tokens = tokenSymbols(firstCharSeq);
	local casesByCondAndFeature: {(string, string): HashSet with string} = Dict();
	for (local secondCharSeq: possibleExtraCharSeqs) {
		local resCharSeq = firstCharSeq + secondCharSeq;
		local secondTokenSymbols = tokenSymbols(secondCharSeq);
		local resTokenSymbols = tokenSymbols(resCharSeq);
		local rhsAndResCond = joinCond("&&",
			joinCond("||", (for (local secondTokenSymbol: secondTokenSymbols) getTokenCond(secondTokenSymbol))...),
			joinCond("||", (for (local resTokenSymbol: resTokenSymbols) getTokenCond(resTokenSymbol))...),
		);
		local key = (rhsAndResCond, featuresByCharSeq[resCharSeq]);
		casesByCondAndFeature.setdefault(key, HashSet()).insert(secondCharSeq);
	}
	local numCases = casesByCondAndFeature.values.each.length + ...;
	local lhsCond = joinCond("||", (for (local token: tokens) getTokenCond(token))...);
	function joinRhsEqualsOneOfCases(prefix: string, cases: {string...}): string {
		return (("0" + "".join((() -> {
			for (local secondCharSeq: cases) {
				local secondTokenSymbols = tokenSymbols(secondCharSeq);
				for (local secondTokenSymbol: secondTokenSymbols) {
					local cond = "1";
					if (#secondTokenSymbols != 1)
						cond = getTokenCond(secondTokenSymbol);
					if (cond == "1") {
						yield f" || rhs == {secondTokenSymbol}";
					} else {
						yield f"\n"
							f"#if {cond}\n"
							f"{prefix}|| rhs == {secondTokenSymbol}\n"
							f"#endif /" f"* {cond} *" f"/\n"
							f"{prefix}";
					}
				}
			}
		})())).lsstrip("0 || ") + "~").rstriplines()[:-1].rereplace(r"\n\n+", "\n");
	}
	if (numCases == 1) {
		local key, cases = casesByCondAndFeature.first...;
		local rhsAndResCond, features = key...;
		local mergedCond = joinCond("&&", lhsCond, rhsAndResCond);
		if (mergedCond != "1")
			print("#if ", mergedCond);
		for (local token: tokens) {
			local thisTokenCond = "1";
			if (#tokens > 1)
				thisTokenCond = getTokenCond(token);
			if (thisTokenCond != "1")
				print("#if ", thisTokenCond);
			print("	case ", token, ":");
			if (thisTokenCond != "1")
				print("#endif /" "* ", thisTokenCond, " *" "/");
		}
		print("\t\treturn ", joinCond("&&",
			joinRhsEqualsOneOfCases("\t\t       ", cases),
			builtTppLexerHas(features),
		), ";");
		if (mergedCond != "1")
			print("#endif /" "* ", mergedCond, " *" "/");
		continue;
	}

	if (lhsCond != "1")
		print("#if ", lhsCond);
	for (local token: tokens) {
		local thisTokenCond = "1";
		if (#tokens > 1)
			thisTokenCond = getTokenCond(token);
		if (thisTokenCond != "1")
			print("#if ", thisTokenCond);
		print("	case ", token, ":");
		if (thisTokenCond != "1")
			print("#endif /" "* ", thisTokenCond, " *" "/");
	}
	if (numCases >= 3) {
		print("		switch (rhs) {");
		for (local key, cases: casesByCondAndFeature) {
			local rhsAndResCond, features = key...;
			if (rhsAndResCond != "1")
				print("#if ", rhsAndResCond);
			for (local secondCharSeq: cases) {
				local secondTokenSymbols = tokenSymbols(secondCharSeq);
				for (local secondTokenSymbol: secondTokenSymbols) {
					local secondTokenCond = "1";
					if (#secondTokenSymbols > 1)
						secondTokenCond = getTokenCond(secondTokenSymbol);
					if (secondTokenCond != "1")
						print("#if ", secondTokenCond);
					print("		case ", secondTokenSymbol, ":");
					if (secondTokenCond != "1")
						print("#endif /" "* ", secondTokenCond, " *" "/");
				}
			}
			print("			return ", builtTppLexerHas(features), ";");
			if (rhsAndResCond != "1")
				print("#endif /" "* ", rhsAndResCond, " *" "/");
		}
		print("		default: break;");
		print("		}");
	} else {
		for (local key, cases: casesByCondAndFeature) {
			local rhsAndResCond, features = key...;
			if (rhsAndResCond != "1")
				print("#if ", rhsAndResCond);
			print("		if (", joinRhsEqualsOneOfCases("\t\t    ", cases), ")");
			print("			return ", builtTppLexerHas(features), ";");
			if (rhsAndResCond != "1")
				print("#endif /" "* ", rhsAndResCond, " *" "/");
		}
	}
	print("		break;");
	if (lhsCond != "1")
		print("#endif /" "* ", lhsCond, " *" "/");
}

]]]*/
	case '?':
		switch (rhs) {
#if TPP_HAVE_TOK_QMARK_EQUAL && TPP_HAVE_TRIGRAPHS
		case TPP_TOK_QMARK_EQUAL:
			return tpp_lexer_has(self, TRIGRAPHS);
#endif /* TPP_HAVE_TOK_QMARK_EQUAL && TPP_HAVE_TRIGRAPHS */
#if TPP_HAVE_TOK_QMARK_QMARK
		case '?':
			return tpp_lexer_has(self, TOK_QMARK_QMARK);
#endif /* TPP_HAVE_TOK_QMARK_QMARK */
#if TPP_HAVE_TOK_QMARK_EQUAL
		case '=':
			return tpp_lexer_has(self, TOK_QMARK_EQUAL);
#endif /* TPP_HAVE_TOK_QMARK_EQUAL */
		default: break;
		}
		break;
#if TPP_HAVE_TOK_QMARK_QMARK
	case TPP_TOK_QMARK_QMARK:
		switch (rhs) {
#if TPP_HAVE_TRIGRAPHS
		case '=':
		case '(':
		case '/':
#if TPP_HAVE_TOK_SLASH_COMMENT
		case TPP_TOK_SLASH_COMMENT:
#endif /* TPP_HAVE_TOK_SLASH_COMMENT */
#if TPP_HAVE_TOK_SOL_SLASH_COMMENT
		case TPP_TOK_SOL_SLASH_COMMENT:
#endif /* TPP_HAVE_TOK_SOL_SLASH_COMMENT */
		case ')':
		case '<':
		case '!':
		case '>':
		case '-':
			return tpp_lexer_has(self, TRIGRAPHS);
#endif /* TPP_HAVE_TRIGRAPHS */
#if (TPP_HAVE_TOK_C_CHAR || TPP_HAVE_TOK_BLOCK_CHAR_LITERAL) && TPP_HAVE_TRIGRAPHS
#if TPP_HAVE_TOK_C_CHAR
		case TPP_TOK_C_CHAR:
#endif /* TPP_HAVE_TOK_C_CHAR */
#if TPP_HAVE_TOK_BLOCK_CHAR_LITERAL
		case TPP_TOK_BLOCK_CHAR_LITERAL:
#endif /* TPP_HAVE_TOK_BLOCK_CHAR_LITERAL */
			return tpp_lexer_has(self, TRIGRAPHS);
#endif /* (TPP_HAVE_TOK_C_CHAR || TPP_HAVE_TOK_BLOCK_CHAR_LITERAL) && TPP_HAVE_TRIGRAPHS */
		default: break;
		}
		break;
#endif /* TPP_HAVE_TOK_QMARK_QMARK */
	case '/':
#if TPP_HAVE_TOK_SLASH_COMMENT
	case TPP_TOK_SLASH_COMMENT:
#endif /* TPP_HAVE_TOK_SLASH_COMMENT */
#if TPP_HAVE_TOK_SOL_SLASH_COMMENT
	case TPP_TOK_SOL_SLASH_COMMENT:
#endif /* TPP_HAVE_TOK_SOL_SLASH_COMMENT */
		switch (rhs) {
#if TPP_HAVE_TOK_C_COMMENT
		case '*':
			return tpp_lexer_has(self, TOK_C_COMMENT);
#endif /* TPP_HAVE_TOK_C_COMMENT */
#if TPP_HAVE_TOK_SLASH_EQUAL
		case '=':
			return tpp_lexer_has(self, TOK_SLASH_EQUAL);
#endif /* TPP_HAVE_TOK_SLASH_EQUAL */
#if TPP_HAVE_TOK_CXX_COMMENT || TPP_HAVE_TOK_SLASH_SLASH
		case '/':
#if TPP_HAVE_TOK_SLASH_COMMENT
		case TPP_TOK_SLASH_COMMENT:
#endif /* TPP_HAVE_TOK_SLASH_COMMENT */
#if TPP_HAVE_TOK_SOL_SLASH_COMMENT
		case TPP_TOK_SOL_SLASH_COMMENT:
#endif /* TPP_HAVE_TOK_SOL_SLASH_COMMENT */
			return tpp_lexer_has(self, TOK_CXX_COMMENT) || tpp_lexer_has(self, TOK_SLASH_SLASH);
#endif /* TPP_HAVE_TOK_CXX_COMMENT || TPP_HAVE_TOK_SLASH_SLASH */
#if TPP_HAVE_TOK_SLASH_EQUAL && TPP_HAVE_TOK_SLASH_SLASH_EQUAL
		case TPP_TOK_SLASH_EQUAL:
			return tpp_lexer_has(self, TOK_SLASH_SLASH_EQUAL);
#endif /* TPP_HAVE_TOK_SLASH_EQUAL && TPP_HAVE_TOK_SLASH_SLASH_EQUAL */
		default: break;
		}
		break;
#if TPP_HAVE_TOK_PASCAL_COMMENT
	case '(':
		return rhs == '*' && tpp_lexer_has(self, TOK_PASCAL_COMMENT);
#endif /* TPP_HAVE_TOK_PASCAL_COMMENT */
	case '<':
		switch (rhs) {
#if TPP_HAVE_TOK_LANGLE_LANGLE
		case '<':
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TOK_LANGLE_EQUAL
		case '=':
			return tpp_lexer_has(self, TOK_LANGLE_EQUAL);
#endif /* TPP_HAVE_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TOK_LANGLE_EQUAL && TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL
		case TPP_TOK_LANGLE_EQUAL:
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE_EQUAL);
#endif /* TPP_HAVE_TOK_LANGLE_EQUAL && TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TOK_LANGLE_RANGLE
		case '>':
			return tpp_lexer_has(self, TOK_LANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TOK_LANGLE_LANGLE && TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE
		case TPP_TOK_LANGLE_LANGLE:
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE && TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL && TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL
		case TPP_TOK_LANGLE_LANGLE_EQUAL:
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE_LANGLE_EQUAL);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL && TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TOK_LANGLE_MINUS
		case '-':
			return tpp_lexer_has(self, TOK_LANGLE_MINUS);
#endif /* TPP_HAVE_TOK_LANGLE_MINUS */
#if TPP_HAVE_TOK_EQUAL_RANGLE && TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE
		case TPP_TOK_EQUAL_RANGLE:
			return tpp_lexer_has(self, TOK_LANGLE_EQUAL_RANGLE);
#endif /* TPP_HAVE_TOK_EQUAL_RANGLE && TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TOK_MINUS_RANGLE && TPP_HAVE_TOK_LANGLE_MINUS_RANGLE
		case TPP_TOK_MINUS_RANGLE:
			return tpp_lexer_has(self, TOK_LANGLE_MINUS_RANGLE);
#endif /* TPP_HAVE_TOK_MINUS_RANGLE && TPP_HAVE_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TOK_EQUAL_LANGLE && TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE
		case TPP_TOK_EQUAL_LANGLE:
			return tpp_lexer_has(self, TOK_LANGLE_EQUAL_LANGLE);
#endif /* TPP_HAVE_TOK_EQUAL_LANGLE && TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TOK_MINUS_LANGLE && TPP_HAVE_TOK_LANGLE_MINUS_LANGLE
		case TPP_TOK_MINUS_LANGLE:
			return tpp_lexer_has(self, TOK_LANGLE_MINUS_LANGLE);
#endif /* TPP_HAVE_TOK_MINUS_LANGLE && TPP_HAVE_TOK_LANGLE_MINUS_LANGLE */
#if TPP_HAVE_TOK_LANGLE_MINUS && TPP_HAVE_TOK_LANGLE_LANGLE_MINUS
		case TPP_TOK_LANGLE_MINUS:
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE_MINUS);
#endif /* TPP_HAVE_TOK_LANGLE_MINUS && TPP_HAVE_TOK_LANGLE_LANGLE_MINUS */
#if TPP_HAVE_TOK_LANGLE_LANGLE_MINUS && TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS
		case TPP_TOK_LANGLE_LANGLE_MINUS:
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE_LANGLE_MINUS);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_MINUS && TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS */
		default: break;
		}
		break;
	case '>':
		switch (rhs) {
#if TPP_HAVE_TOK_RANGLE_RANGLE
		case '>':
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TOK_RANGLE_EQUAL
		case '=':
			return tpp_lexer_has(self, TOK_RANGLE_EQUAL);
#endif /* TPP_HAVE_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TOK_RANGLE_EQUAL && TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL
		case TPP_TOK_RANGLE_EQUAL:
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE_EQUAL);
#endif /* TPP_HAVE_TOK_RANGLE_EQUAL && TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TOK_RANGLE_RANGLE && TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE
		case TPP_TOK_RANGLE_RANGLE:
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE && TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL && TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL
		case TPP_TOK_RANGLE_RANGLE_EQUAL:
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE_RANGLE_EQUAL);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL && TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TOK_RANGLE_LANGLE
		case '<':
			return tpp_lexer_has(self, TOK_RANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TOK_RANGLE_MINUS
		case '-':
			return tpp_lexer_has(self, TOK_RANGLE_MINUS);
#endif /* TPP_HAVE_TOK_RANGLE_MINUS */
#if TPP_HAVE_TOK_EQUAL_LANGLE && TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE
		case TPP_TOK_EQUAL_LANGLE:
			return tpp_lexer_has(self, TOK_RANGLE_EQUAL_LANGLE);
#endif /* TPP_HAVE_TOK_EQUAL_LANGLE && TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TOK_MINUS_LANGLE && TPP_HAVE_TOK_RANGLE_MINUS_LANGLE
		case TPP_TOK_MINUS_LANGLE:
			return tpp_lexer_has(self, TOK_RANGLE_MINUS_LANGLE);
#endif /* TPP_HAVE_TOK_MINUS_LANGLE && TPP_HAVE_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TOK_EQUAL_RANGLE && TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE
		case TPP_TOK_EQUAL_RANGLE:
			return tpp_lexer_has(self, TOK_RANGLE_EQUAL_RANGLE);
#endif /* TPP_HAVE_TOK_EQUAL_RANGLE && TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TOK_MINUS_RANGLE && TPP_HAVE_TOK_RANGLE_MINUS_RANGLE
		case TPP_TOK_MINUS_RANGLE:
			return tpp_lexer_has(self, TOK_RANGLE_MINUS_RANGLE);
#endif /* TPP_HAVE_TOK_MINUS_RANGLE && TPP_HAVE_TOK_RANGLE_MINUS_RANGLE */
#if TPP_HAVE_TOK_RANGLE_MINUS && TPP_HAVE_TOK_RANGLE_RANGLE_MINUS
		case TPP_TOK_RANGLE_MINUS:
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE_MINUS);
#endif /* TPP_HAVE_TOK_RANGLE_MINUS && TPP_HAVE_TOK_RANGLE_RANGLE_MINUS */
#if TPP_HAVE_TOK_RANGLE_RANGLE_MINUS && TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS
		case TPP_TOK_RANGLE_RANGLE_MINUS:
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE_RANGLE_MINUS);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_MINUS && TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS */
		default: break;
		}
		break;
	case '=':
		switch (rhs) {
#if TPP_HAVE_TOK_EQUAL_EQUAL
		case '=':
			return tpp_lexer_has(self, TOK_EQUAL_EQUAL);
#endif /* TPP_HAVE_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TOK_EQUAL_EQUAL && TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL
		case TPP_TOK_EQUAL_EQUAL:
			return tpp_lexer_has(self, TOK_EQUAL_EQUAL_EQUAL);
#endif /* TPP_HAVE_TOK_EQUAL_EQUAL && TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TOK_EQUAL_PLUS
		case '+':
			return tpp_lexer_has(self, TOK_EQUAL_PLUS);
#endif /* TPP_HAVE_TOK_EQUAL_PLUS */
#if TPP_HAVE_TOK_EQUAL_MINUS
		case '-':
			return tpp_lexer_has(self, TOK_EQUAL_MINUS);
#endif /* TPP_HAVE_TOK_EQUAL_MINUS */
#if TPP_HAVE_TOK_EQUAL_STAR
		case '*':
			return tpp_lexer_has(self, TOK_EQUAL_STAR);
#endif /* TPP_HAVE_TOK_EQUAL_STAR */
#if TPP_HAVE_TOK_STAR_STAR && TPP_HAVE_TOK_EQUAL_STAR_STAR
		case TPP_TOK_STAR_STAR:
			return tpp_lexer_has(self, TOK_EQUAL_STAR_STAR);
#endif /* TPP_HAVE_TOK_STAR_STAR && TPP_HAVE_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TOK_EQUAL_SLASH
		case '/':
#if TPP_HAVE_TOK_SLASH_COMMENT
		case TPP_TOK_SLASH_COMMENT:
#endif /* TPP_HAVE_TOK_SLASH_COMMENT */
#if TPP_HAVE_TOK_SOL_SLASH_COMMENT
		case TPP_TOK_SOL_SLASH_COMMENT:
#endif /* TPP_HAVE_TOK_SOL_SLASH_COMMENT */
			return tpp_lexer_has(self, TOK_EQUAL_SLASH);
#endif /* TPP_HAVE_TOK_EQUAL_SLASH */
#if (TPP_HAVE_TOK_CXX_COMMENT || TPP_HAVE_TOK_SLASH_SLASH) && TPP_HAVE_TOK_EQUAL_SLASH_SLASH
		case TPP_TOK_SLASH_SLASH:
			return tpp_lexer_has(self, TOK_EQUAL_SLASH_SLASH);
#endif /* (TPP_HAVE_TOK_CXX_COMMENT || TPP_HAVE_TOK_SLASH_SLASH) && TPP_HAVE_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TOK_EQUAL_PERCENT
		case '%':
			return tpp_lexer_has(self, TOK_EQUAL_PERCENT);
#endif /* TPP_HAVE_TOK_EQUAL_PERCENT */
#if TPP_HAVE_TOK_EQUAL_AMP
		case '&':
			return tpp_lexer_has(self, TOK_EQUAL_AMP);
#endif /* TPP_HAVE_TOK_EQUAL_AMP */
#if TPP_HAVE_TOK_EQUAL_PIPE
		case '|':
			return tpp_lexer_has(self, TOK_EQUAL_PIPE);
#endif /* TPP_HAVE_TOK_EQUAL_PIPE */
#if TPP_HAVE_TOK_EQUAL_HAT
		case '^':
			return tpp_lexer_has(self, TOK_EQUAL_HAT);
#endif /* TPP_HAVE_TOK_EQUAL_HAT */
#if TPP_HAVE_TOK_EQUAL_LANGLE
		case '<':
			return tpp_lexer_has(self, TOK_EQUAL_LANGLE);
#endif /* TPP_HAVE_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TOK_LANGLE_LANGLE && TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE
		case TPP_TOK_LANGLE_LANGLE:
			return tpp_lexer_has(self, TOK_EQUAL_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE && TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE && TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE
		case TPP_TOK_LANGLE_LANGLE_LANGLE:
			return tpp_lexer_has(self, TOK_EQUAL_LANGLE_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE && TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TOK_EQUAL_RANGLE
		case '>':
			return tpp_lexer_has(self, TOK_EQUAL_RANGLE);
#endif /* TPP_HAVE_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TOK_RANGLE_RANGLE && TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE
		case TPP_TOK_RANGLE_RANGLE:
			return tpp_lexer_has(self, TOK_EQUAL_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE && TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE && TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE
		case TPP_TOK_RANGLE_RANGLE_RANGLE:
			return tpp_lexer_has(self, TOK_EQUAL_RANGLE_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE && TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TOK_EQUAL_AT
		case '@':
#if TPP_HAVE_TOK_AT_COMMENT
		case TPP_TOK_AT_COMMENT:
#endif /* TPP_HAVE_TOK_AT_COMMENT */
#if TPP_HAVE_TOK_SOL_AT_COMMENT
		case TPP_TOK_SOL_AT_COMMENT:
#endif /* TPP_HAVE_TOK_SOL_AT_COMMENT */
			return tpp_lexer_has(self, TOK_EQUAL_AT);
#endif /* TPP_HAVE_TOK_EQUAL_AT */
#if TPP_HAVE_TOK_EQUAL_TILDE
		case '~':
			return tpp_lexer_has(self, TOK_EQUAL_TILDE);
#endif /* TPP_HAVE_TOK_EQUAL_TILDE */
#if TPP_HAVE_TOK_EQUAL_COLON
		case ':':
			return tpp_lexer_has(self, TOK_EQUAL_COLON);
#endif /* TPP_HAVE_TOK_EQUAL_COLON */
#if TPP_HAVE_TOK_EQUAL_EXCLAIM
		case '!':
			return tpp_lexer_has(self, TOK_EQUAL_EXCLAIM);
#endif /* TPP_HAVE_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TOK_EQUAL_EXCLAIM && TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM
		case TPP_TOK_EQUAL_EXCLAIM:
			return tpp_lexer_has(self, TOK_EQUAL_EQUAL_EXCLAIM);
#endif /* TPP_HAVE_TOK_EQUAL_EXCLAIM && TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM */
#if TPP_HAVE_TOK_EQUAL_QMARK
		case '?':
			return tpp_lexer_has(self, TOK_EQUAL_QMARK);
#endif /* TPP_HAVE_TOK_EQUAL_QMARK */
#if TPP_HAVE_TOK_PERCENT_PERCENT && TPP_HAVE_TOK_EQUAL_PERCENT_PERCENT
		case TPP_TOK_PERCENT_PERCENT:
			return tpp_lexer_has(self, TOK_EQUAL_PERCENT_PERCENT);
#endif /* TPP_HAVE_TOK_PERCENT_PERCENT && TPP_HAVE_TOK_EQUAL_PERCENT_PERCENT */
#if (TPP_HAVE_TOK_AT_AT_COMMENT || TPP_HAVE_TOK_AT_AT) && TPP_HAVE_TOK_EQUAL_AT_AT
		case TPP_TOK_AT_AT:
			return tpp_lexer_has(self, TOK_EQUAL_AT_AT);
#endif /* (TPP_HAVE_TOK_AT_AT_COMMENT || TPP_HAVE_TOK_AT_AT) && TPP_HAVE_TOK_EQUAL_AT_AT */
		default: break;
		}
		break;
	case '!':
		switch (rhs) {
#if TPP_HAVE_TOK_EXCLAIM_EQUAL
		case '=':
			return tpp_lexer_has(self, TOK_EXCLAIM_EQUAL);
#endif /* TPP_HAVE_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TOK_EQUAL_EQUAL && TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL
		case TPP_TOK_EQUAL_EQUAL:
			return tpp_lexer_has(self, TOK_EXCLAIM_EQUAL_EQUAL);
#endif /* TPP_HAVE_TOK_EQUAL_EQUAL && TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TOK_EXCLAIM_EXCLAIM
		case '!':
			return tpp_lexer_has(self, TOK_EXCLAIM_EXCLAIM);
#endif /* TPP_HAVE_TOK_EXCLAIM_EXCLAIM */
		default: break;
		}
		break;
	case '.':
		switch (rhs) {
#if TPP_HAVE_TOK_DOT_DOT && TPP_HAVE_TOK_DOT_DOT_DOT
		case TPP_TOK_DOT_DOT:
			return tpp_lexer_has(self, TOK_DOT_DOT_DOT);
#endif /* TPP_HAVE_TOK_DOT_DOT && TPP_HAVE_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TOK_DOT_STAR
		case '*':
			return tpp_lexer_has(self, TOK_DOT_STAR);
#endif /* TPP_HAVE_TOK_DOT_STAR */
#if TPP_HAVE_TOK_DOT_DOT
		case '.':
			return tpp_lexer_has(self, TOK_DOT_DOT);
#endif /* TPP_HAVE_TOK_DOT_DOT */
		default: break;
		}
		break;
#if TPP_HAVE_TOK_DOT_DOT && TPP_HAVE_TOK_DOT_DOT_DOT
	case TPP_TOK_DOT_DOT:
		return rhs == '.' && tpp_lexer_has(self, TOK_DOT_DOT_DOT);
#endif /* TPP_HAVE_TOK_DOT_DOT && TPP_HAVE_TOK_DOT_DOT_DOT */
	case '+':
#if TPP_HAVE_TOK_PLUS_EQUAL
		if (rhs == '=')
			return tpp_lexer_has(self, TOK_PLUS_EQUAL);
#endif /* TPP_HAVE_TOK_PLUS_EQUAL */
#if TPP_HAVE_TOK_PLUS_PLUS
		if (rhs == '+')
			return tpp_lexer_has(self, TOK_PLUS_PLUS);
#endif /* TPP_HAVE_TOK_PLUS_PLUS */
		break;
	case '-':
		switch (rhs) {
#if TPP_HAVE_TOK_MINUS_EQUAL
		case '=':
			return tpp_lexer_has(self, TOK_MINUS_EQUAL);
#endif /* TPP_HAVE_TOK_MINUS_EQUAL */
#if TPP_HAVE_TOK_MINUS_MINUS
		case '-':
			return tpp_lexer_has(self, TOK_MINUS_MINUS);
#endif /* TPP_HAVE_TOK_MINUS_MINUS */
#if TPP_HAVE_TOK_MINUS_RANGLE
		case '>':
			return tpp_lexer_has(self, TOK_MINUS_RANGLE);
#endif /* TPP_HAVE_TOK_MINUS_RANGLE */
#if TPP_HAVE_TOK_MINUS_LANGLE
		case '<':
			return tpp_lexer_has(self, TOK_MINUS_LANGLE);
#endif /* TPP_HAVE_TOK_MINUS_LANGLE */
#if TPP_HAVE_TOK_RANGLE_RANGLE && TPP_HAVE_TOK_MINUS_RANGLE_RANGLE
		case TPP_TOK_RANGLE_RANGLE:
			return tpp_lexer_has(self, TOK_MINUS_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE && TPP_HAVE_TOK_MINUS_RANGLE_RANGLE */
#if TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE && TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE
		case TPP_TOK_RANGLE_RANGLE_RANGLE:
			return tpp_lexer_has(self, TOK_MINUS_RANGLE_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE && TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TOK_LANGLE_LANGLE && TPP_HAVE_TOK_MINUS_LANGLE_LANGLE
		case TPP_TOK_LANGLE_LANGLE:
			return tpp_lexer_has(self, TOK_MINUS_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE && TPP_HAVE_TOK_MINUS_LANGLE_LANGLE */
#if TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE && TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE
		case TPP_TOK_LANGLE_LANGLE_LANGLE:
			return tpp_lexer_has(self, TOK_MINUS_LANGLE_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE && TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE */
		default: break;
		}
		break;
	case '*':
		switch (rhs) {
#if TPP_HAVE_TOK_STAR_EQUAL
		case '=':
			return tpp_lexer_has(self, TOK_STAR_EQUAL);
#endif /* TPP_HAVE_TOK_STAR_EQUAL */
#if TPP_HAVE_TOK_STAR_EQUAL && TPP_HAVE_TOK_STAR_STAR_EQUAL
		case TPP_TOK_STAR_EQUAL:
			return tpp_lexer_has(self, TOK_STAR_STAR_EQUAL);
#endif /* TPP_HAVE_TOK_STAR_EQUAL && TPP_HAVE_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TOK_STAR_STAR
		case '*':
			return tpp_lexer_has(self, TOK_STAR_STAR);
#endif /* TPP_HAVE_TOK_STAR_STAR */
#if TPP_HAVE_TOK_LANGLE_MINUS && TPP_HAVE_TOK_STAR_LANGLE_MINUS
		case TPP_TOK_LANGLE_MINUS:
			return tpp_lexer_has(self, TOK_STAR_LANGLE_MINUS);
#endif /* TPP_HAVE_TOK_LANGLE_MINUS && TPP_HAVE_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TOK_STAR_DOT
		case '.':
			return tpp_lexer_has(self, TOK_STAR_DOT);
#endif /* TPP_HAVE_TOK_STAR_DOT */
		default: break;
		}
		break;
	case '%':
		switch (rhs) {
#if TPP_HAVE_TOK_PERCENT_EQUAL
		case '=':
			return tpp_lexer_has(self, TOK_PERCENT_EQUAL);
#endif /* TPP_HAVE_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TOK_PERCENT_PERCENT
		case '%':
			return tpp_lexer_has(self, TOK_PERCENT_PERCENT);
#endif /* TPP_HAVE_TOK_PERCENT_PERCENT */
#if TPP_HAVE_TOK_PERCENT_EQUAL && TPP_HAVE_TOK_PERCENT_PERCENT_EQUAL
		case TPP_TOK_PERCENT_EQUAL:
			return tpp_lexer_has(self, TOK_PERCENT_PERCENT_EQUAL);
#endif /* TPP_HAVE_TOK_PERCENT_EQUAL && TPP_HAVE_TOK_PERCENT_PERCENT_EQUAL */
		default: break;
		}
		break;
#if TPP_HAVE_TOK_LANGLE_LANGLE
	case TPP_TOK_LANGLE_LANGLE:
		switch (rhs) {
#if TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL
		case '=':
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE_EQUAL);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE
		case '<':
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TOK_LANGLE_EQUAL && TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL
		case TPP_TOK_LANGLE_EQUAL:
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE_LANGLE_EQUAL);
#endif /* TPP_HAVE_TOK_LANGLE_EQUAL && TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TOK_LANGLE_LANGLE_MINUS
		case '-':
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE_MINUS);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_MINUS */
#if TPP_HAVE_TOK_LANGLE_MINUS && TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS
		case TPP_TOK_LANGLE_MINUS:
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE_LANGLE_MINUS);
#endif /* TPP_HAVE_TOK_LANGLE_MINUS && TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS */
		default: break;
		}
		break;
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TOK_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE:
		switch (rhs) {
#if TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL
		case '=':
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE_EQUAL);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE
		case '>':
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TOK_RANGLE_EQUAL && TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL
		case TPP_TOK_RANGLE_EQUAL:
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE_RANGLE_EQUAL);
#endif /* TPP_HAVE_TOK_RANGLE_EQUAL && TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TOK_RANGLE_RANGLE_MINUS
		case '-':
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE_MINUS);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_MINUS */
#if TPP_HAVE_TOK_RANGLE_MINUS && TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS
		case TPP_TOK_RANGLE_MINUS:
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE_RANGLE_MINUS);
#endif /* TPP_HAVE_TOK_RANGLE_MINUS && TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS */
		default: break;
		}
		break;
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE */
	case '&':
#if TPP_HAVE_TOK_AMP_EQUAL
		if (rhs == '=')
			return tpp_lexer_has(self, TOK_AMP_EQUAL);
#endif /* TPP_HAVE_TOK_AMP_EQUAL */
#if TPP_HAVE_TOK_AMP_AMP
		if (rhs == '&')
			return tpp_lexer_has(self, TOK_AMP_AMP);
#endif /* TPP_HAVE_TOK_AMP_AMP */
		break;
	case '|':
#if TPP_HAVE_TOK_PIPE_EQUAL
		if (rhs == '=')
			return tpp_lexer_has(self, TOK_PIPE_EQUAL);
#endif /* TPP_HAVE_TOK_PIPE_EQUAL */
#if TPP_HAVE_TOK_PIPE_PIPE
		if (rhs == '|')
			return tpp_lexer_has(self, TOK_PIPE_PIPE);
#endif /* TPP_HAVE_TOK_PIPE_PIPE */
		break;
	case '^':
#if TPP_HAVE_TOK_HAT_EQUAL
		if (rhs == '=')
			return tpp_lexer_has(self, TOK_HAT_EQUAL);
#endif /* TPP_HAVE_TOK_HAT_EQUAL */
#if TPP_HAVE_TOK_HAT_HAT
		if (rhs == '^')
			return tpp_lexer_has(self, TOK_HAT_HAT);
#endif /* TPP_HAVE_TOK_HAT_HAT */
		break;
#if (TPP_HAVE_TOK_CXX_COMMENT || TPP_HAVE_TOK_SLASH_SLASH) && TPP_HAVE_TOK_SLASH_SLASH_EQUAL
	case TPP_TOK_SLASH_SLASH:
		return rhs == '=' && tpp_lexer_has(self, TOK_SLASH_SLASH_EQUAL);
#endif /* (TPP_HAVE_TOK_CXX_COMMENT || TPP_HAVE_TOK_SLASH_SLASH) && TPP_HAVE_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TOK_STAR_STAR && TPP_HAVE_TOK_STAR_STAR_EQUAL
	case TPP_TOK_STAR_STAR:
		return rhs == '=' && tpp_lexer_has(self, TOK_STAR_STAR_EQUAL);
#endif /* TPP_HAVE_TOK_STAR_STAR && TPP_HAVE_TOK_STAR_STAR_EQUAL */
	case '@':
#if TPP_HAVE_TOK_AT_COMMENT
	case TPP_TOK_AT_COMMENT:
#endif /* TPP_HAVE_TOK_AT_COMMENT */
#if TPP_HAVE_TOK_SOL_AT_COMMENT
	case TPP_TOK_SOL_AT_COMMENT:
#endif /* TPP_HAVE_TOK_SOL_AT_COMMENT */
		switch (rhs) {
#if TPP_HAVE_TOK_AT_EQUAL
		case '=':
			return tpp_lexer_has(self, TOK_AT_EQUAL);
#endif /* TPP_HAVE_TOK_AT_EQUAL */
#if TPP_HAVE_TOK_AT_AT_COMMENT || TPP_HAVE_TOK_AT_AT
		case '@':
#if TPP_HAVE_TOK_AT_COMMENT
		case TPP_TOK_AT_COMMENT:
#endif /* TPP_HAVE_TOK_AT_COMMENT */
#if TPP_HAVE_TOK_SOL_AT_COMMENT
		case TPP_TOK_SOL_AT_COMMENT:
#endif /* TPP_HAVE_TOK_SOL_AT_COMMENT */
			return tpp_lexer_has(self, TOK_AT_AT_COMMENT) || tpp_lexer_has(self, TOK_AT_AT);
#endif /* TPP_HAVE_TOK_AT_AT_COMMENT || TPP_HAVE_TOK_AT_AT */
#if TPP_HAVE_TOK_AT_EQUAL && TPP_HAVE_TOK_AT_AT_EQUAL
		case TPP_TOK_AT_EQUAL:
			return tpp_lexer_has(self, TOK_AT_AT_EQUAL);
#endif /* TPP_HAVE_TOK_AT_EQUAL && TPP_HAVE_TOK_AT_AT_EQUAL */
		default: break;
		}
		break;
#if TPP_HAVE_TOK_POUND_POUND
	case '#':
#if TPP_HAVE_TOK_SHELL_COMMENT
	case TPP_TOK_SHELL_COMMENT:
#endif /* TPP_HAVE_TOK_SHELL_COMMENT */
#if TPP_HAVE_TOK_SOL_SHELL_COMMENT
	case TPP_TOK_SOL_SHELL_COMMENT:
#endif /* TPP_HAVE_TOK_SOL_SHELL_COMMENT */
		return (rhs == '#'
#if TPP_HAVE_TOK_SHELL_COMMENT
		       || rhs == TPP_TOK_SHELL_COMMENT
#endif /* TPP_HAVE_TOK_SHELL_COMMENT */
#if TPP_HAVE_TOK_SOL_SHELL_COMMENT
		       || rhs == TPP_TOK_SOL_SHELL_COMMENT
#endif /* TPP_HAVE_TOK_SOL_SHELL_COMMENT */
		       ) && tpp_lexer_has(self, TOK_POUND_POUND);
#endif /* TPP_HAVE_TOK_POUND_POUND */
	case '~':
#if TPP_HAVE_TOK_TILDE_TILDE
		if (rhs == '~')
			return tpp_lexer_has(self, TOK_TILDE_TILDE);
#endif /* TPP_HAVE_TOK_TILDE_TILDE */
#if TPP_HAVE_TOK_TILDE_EQUAL
		if (rhs == '=')
			return tpp_lexer_has(self, TOK_TILDE_EQUAL);
#endif /* TPP_HAVE_TOK_TILDE_EQUAL */
		break;
	case ':':
#if TPP_HAVE_TOK_COLON_EQUAL
		if (rhs == '=')
			return tpp_lexer_has(self, TOK_COLON_EQUAL);
#endif /* TPP_HAVE_TOK_COLON_EQUAL */
#if TPP_HAVE_TOK_COLON_COLON
		if (rhs == ':')
			return tpp_lexer_has(self, TOK_COLON_COLON);
#endif /* TPP_HAVE_TOK_COLON_COLON */
		break;
#if TPP_HAVE_TOK_MINUS_RANGLE
	case TPP_TOK_MINUS_RANGLE:
		switch (rhs) {
#if TPP_HAVE_TOK_MINUS_RANGLE_STAR
		case '*':
			return tpp_lexer_has(self, TOK_MINUS_RANGLE_STAR);
#endif /* TPP_HAVE_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TOK_MINUS_RANGLE_RANGLE
		case '>':
			return tpp_lexer_has(self, TOK_MINUS_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_MINUS_RANGLE_RANGLE */
#if TPP_HAVE_TOK_RANGLE_RANGLE && TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE
		case TPP_TOK_RANGLE_RANGLE:
			return tpp_lexer_has(self, TOK_MINUS_RANGLE_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE && TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE */
		default: break;
		}
		break;
#endif /* TPP_HAVE_TOK_MINUS_RANGLE */
#if TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE
	case TPP_TOK_LANGLE_LANGLE_LANGLE:
#if TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL
		if (rhs == '=')
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE_LANGLE_EQUAL);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS
		if (rhs == '-')
			return tpp_lexer_has(self, TOK_LANGLE_LANGLE_LANGLE_MINUS);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS */
		break;
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE_RANGLE:
#if TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL
		if (rhs == '=')
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE_RANGLE_EQUAL);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS
		if (rhs == '-')
			return tpp_lexer_has(self, TOK_RANGLE_RANGLE_RANGLE_MINUS);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS */
		break;
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TOK_EQUAL_EQUAL
	case TPP_TOK_EQUAL_EQUAL:
#if TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL
		if (rhs == '=')
			return tpp_lexer_has(self, TOK_EQUAL_EQUAL_EQUAL);
#endif /* TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM
		if (rhs == '!')
			return tpp_lexer_has(self, TOK_EQUAL_EQUAL_EXCLAIM);
#endif /* TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM */
		break;
#endif /* TPP_HAVE_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TOK_EXCLAIM_EQUAL && TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL
	case TPP_TOK_EXCLAIM_EQUAL:
		return rhs == '=' && tpp_lexer_has(self, TOK_EXCLAIM_EQUAL_EQUAL);
#endif /* TPP_HAVE_TOK_EXCLAIM_EQUAL && TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TOK_EQUAL_STAR && TPP_HAVE_TOK_EQUAL_STAR_STAR
	case TPP_TOK_EQUAL_STAR:
		return rhs == '*' && tpp_lexer_has(self, TOK_EQUAL_STAR_STAR);
#endif /* TPP_HAVE_TOK_EQUAL_STAR && TPP_HAVE_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TOK_EQUAL_SLASH && TPP_HAVE_TOK_EQUAL_SLASH_SLASH
	case TPP_TOK_EQUAL_SLASH:
		return (rhs == '/'
#if TPP_HAVE_TOK_SLASH_COMMENT
		       || rhs == TPP_TOK_SLASH_COMMENT
#endif /* TPP_HAVE_TOK_SLASH_COMMENT */
#if TPP_HAVE_TOK_SOL_SLASH_COMMENT
		       || rhs == TPP_TOK_SOL_SLASH_COMMENT
#endif /* TPP_HAVE_TOK_SOL_SLASH_COMMENT */
		       ) && tpp_lexer_has(self, TOK_EQUAL_SLASH_SLASH);
#endif /* TPP_HAVE_TOK_EQUAL_SLASH && TPP_HAVE_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TOK_EQUAL_LANGLE
	case TPP_TOK_EQUAL_LANGLE:
#if TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE
		if (rhs == '<')
			return tpp_lexer_has(self, TOK_EQUAL_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TOK_LANGLE_LANGLE && TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE
		if (rhs == TPP_TOK_LANGLE_LANGLE)
			return tpp_lexer_has(self, TOK_EQUAL_LANGLE_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE && TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
		break;
#endif /* TPP_HAVE_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE && TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	case TPP_TOK_EQUAL_LANGLE_LANGLE:
		return rhs == '<' && tpp_lexer_has(self, TOK_EQUAL_LANGLE_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE && TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TOK_EQUAL_RANGLE
	case TPP_TOK_EQUAL_RANGLE:
#if TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE
		if (rhs == '>')
			return tpp_lexer_has(self, TOK_EQUAL_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TOK_RANGLE_RANGLE && TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE
		if (rhs == TPP_TOK_RANGLE_RANGLE)
			return tpp_lexer_has(self, TOK_EQUAL_RANGLE_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE && TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
		break;
#endif /* TPP_HAVE_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE && TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	case TPP_TOK_EQUAL_RANGLE_RANGLE:
		return rhs == '>' && tpp_lexer_has(self, TOK_EQUAL_RANGLE_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE && TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TOK_LANGLE_EQUAL
	case TPP_TOK_LANGLE_EQUAL:
#if TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE
		if (rhs == '>')
			return tpp_lexer_has(self, TOK_LANGLE_EQUAL_RANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE
		if (rhs == '<')
			return tpp_lexer_has(self, TOK_LANGLE_EQUAL_LANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE */
		break;
#endif /* TPP_HAVE_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TOK_LANGLE_MINUS
	case TPP_TOK_LANGLE_MINUS:
#if TPP_HAVE_TOK_LANGLE_MINUS_RANGLE
		if (rhs == '>')
			return tpp_lexer_has(self, TOK_LANGLE_MINUS_RANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TOK_LANGLE_MINUS_LANGLE
		if (rhs == '<')
			return tpp_lexer_has(self, TOK_LANGLE_MINUS_LANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_MINUS_LANGLE */
		break;
#endif /* TPP_HAVE_TOK_LANGLE_MINUS */
#if TPP_HAVE_TOK_RANGLE_EQUAL
	case TPP_TOK_RANGLE_EQUAL:
#if TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE
		if (rhs == '<')
			return tpp_lexer_has(self, TOK_RANGLE_EQUAL_LANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE
		if (rhs == '>')
			return tpp_lexer_has(self, TOK_RANGLE_EQUAL_RANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE */
		break;
#endif /* TPP_HAVE_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TOK_RANGLE_MINUS
	case TPP_TOK_RANGLE_MINUS:
#if TPP_HAVE_TOK_RANGLE_MINUS_LANGLE
		if (rhs == '<')
			return tpp_lexer_has(self, TOK_RANGLE_MINUS_LANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TOK_RANGLE_MINUS_RANGLE
		if (rhs == '>')
			return tpp_lexer_has(self, TOK_RANGLE_MINUS_RANGLE);
#endif /* TPP_HAVE_TOK_RANGLE_MINUS_RANGLE */
		break;
#endif /* TPP_HAVE_TOK_RANGLE_MINUS */
#if TPP_HAVE_TOK_PERCENT_PERCENT && TPP_HAVE_TOK_PERCENT_PERCENT_EQUAL
	case TPP_TOK_PERCENT_PERCENT:
		return rhs == '=' && tpp_lexer_has(self, TOK_PERCENT_PERCENT_EQUAL);
#endif /* TPP_HAVE_TOK_PERCENT_PERCENT && TPP_HAVE_TOK_PERCENT_PERCENT_EQUAL */
#if TPP_HAVE_TOK_EQUAL_PERCENT && TPP_HAVE_TOK_EQUAL_PERCENT_PERCENT
	case TPP_TOK_EQUAL_PERCENT:
		return rhs == '%' && tpp_lexer_has(self, TOK_EQUAL_PERCENT_PERCENT);
#endif /* TPP_HAVE_TOK_EQUAL_PERCENT && TPP_HAVE_TOK_EQUAL_PERCENT_PERCENT */
#if (TPP_HAVE_TOK_AT_AT_COMMENT || TPP_HAVE_TOK_AT_AT) && TPP_HAVE_TOK_AT_AT_EQUAL
	case TPP_TOK_AT_AT:
		return rhs == '=' && tpp_lexer_has(self, TOK_AT_AT_EQUAL);
#endif /* (TPP_HAVE_TOK_AT_AT_COMMENT || TPP_HAVE_TOK_AT_AT) && TPP_HAVE_TOK_AT_AT_EQUAL */
#if TPP_HAVE_TOK_EQUAL_AT && TPP_HAVE_TOK_EQUAL_AT_AT
	case TPP_TOK_EQUAL_AT:
		return (rhs == '@'
#if TPP_HAVE_TOK_AT_COMMENT
		       || rhs == TPP_TOK_AT_COMMENT
#endif /* TPP_HAVE_TOK_AT_COMMENT */
#if TPP_HAVE_TOK_SOL_AT_COMMENT
		       || rhs == TPP_TOK_SOL_AT_COMMENT
#endif /* TPP_HAVE_TOK_SOL_AT_COMMENT */
		       ) && tpp_lexer_has(self, TOK_EQUAL_AT_AT);
#endif /* TPP_HAVE_TOK_EQUAL_AT && TPP_HAVE_TOK_EQUAL_AT_AT */
#if TPP_HAVE_TOK_MINUS_RANGLE_RANGLE && TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE
	case TPP_TOK_MINUS_RANGLE_RANGLE:
		return rhs == '>' && tpp_lexer_has(self, TOK_MINUS_RANGLE_RANGLE_RANGLE);
#endif /* TPP_HAVE_TOK_MINUS_RANGLE_RANGLE && TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TOK_MINUS_LANGLE
	case TPP_TOK_MINUS_LANGLE:
#if TPP_HAVE_TOK_MINUS_LANGLE_LANGLE
		if (rhs == '<')
			return tpp_lexer_has(self, TOK_MINUS_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_MINUS_LANGLE_LANGLE */
#if TPP_HAVE_TOK_LANGLE_LANGLE && TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE
		if (rhs == TPP_TOK_LANGLE_LANGLE)
			return tpp_lexer_has(self, TOK_MINUS_LANGLE_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE && TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE */
		break;
#endif /* TPP_HAVE_TOK_MINUS_LANGLE */
#if TPP_HAVE_TOK_MINUS_LANGLE_LANGLE && TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE
	case TPP_TOK_MINUS_LANGLE_LANGLE:
		return rhs == '<' && tpp_lexer_has(self, TOK_MINUS_LANGLE_LANGLE_LANGLE);
#endif /* TPP_HAVE_TOK_MINUS_LANGLE_LANGLE && TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE */
/*[[[end]]]*/
	default: break;
	}
	return false;
}
#endif /* TPP_HAVE_LEXER_REQUIRE_WHITESPACE */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_REQUIRE_WHITESPACE_C */
