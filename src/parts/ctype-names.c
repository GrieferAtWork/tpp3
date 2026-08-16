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
#ifndef GUARD_TPP_CTYPE_NAMES_C
#define GUARD_TPP_CTYPE_NAMES_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "ctype.h"
#include "keyword.h"
#include "lexer.h"

#if TPP_HAVE_UNICODE_BYNAME_LOOKUP
#ifndef tpp_unicode_byname_lookup

#include "ctype-names.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

TPP_STATIC_ASSERT(sizeof(uint_least32_t) >= sizeof(tpp_unam_tokenid));
TPP_STATIC_ASSERT(sizeof(uint_least32_t) >= sizeof(tpp_unichar));
#define tpp_decode_uleb128_tokenid (tpp_unam_tokenid)tpp_decode_uleb128
#define tpp_decode_uleb128_unichar (tpp_unichar)tpp_decode_uleb128
#define tpp_decode_uleb128_size    (tpp_size)tpp_decode_uleb128

/* see: https://en.wikipedia.org/wiki/LEB128#Decode_unsigned_integer */
static TPP_NONNULL((1)) uint_least32_t TPPCALL
tpp_decode_uleb128(tpp_char const **tpp_restrict p_iter) {
	uint_least32_t result = 0;
	tpp_char const *iter = *p_iter;
	tpp_char byte, num_bits = 0;
	do {
		byte = *iter++;
		result |= (byte & 0x7f) << num_bits;
		num_bits += 7;
	} while (byte & 0x80);
	*p_iter = iter;
	return result;
}

/* Return a pointer to the start (the first byte of the compressed TEXT
 * section) of the token record into which `db_ptr` points. For this purpose,
 * it doesn't matter which part of the record `db_ptr` points at: this function
 * will always find the record's start and return a pointer for that. */
static TPP_PURECALL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) tpp_char const *TPPCALL
tpp_unam_token_getstart(tpp_char const *tpp_restrict db_ptr) {
	tpp_char const *db_iter = db_ptr;
	while (db_iter[-1] != 0)
		--db_iter;
	(void)tpp_decode_uleb128_tokenid(&db_iter);
	if (db_iter <= db_ptr) {
		/* "db_ptr" points at-or-before the NUL of its own record */
		return db_iter;
	}

	/* "db_ptr" points after the NUL of its own record, and "db_iter"
	 * is actually the end of the record our caller wants us to find
	 * the start of.
	 *
	 * -> Simply find the 2nd preceding NUL-byte, which will be
	 *    the one of the preceding record (or the "0" in "0,1,"
	 *    that preceding the first record)
	 */
	db_iter = db_ptr;
	while (db_iter[-1] != 0)
		--db_iter;
	do {
		--db_iter;
	} while (db_iter[-1] != 0);

	/* Skip the ULEB of the preceding record (or the "1" preceding the first record) */
	(void)tpp_decode_uleb128_tokenid(&db_iter);
	tpp_assert(db_iter < db_ptr);
	return db_iter;
}

/* Helper to decode compressed TEXT */
typedef struct tpp_unam_text_reader {
	tpp_char const *tuntr_ptr;  /* [1..1] Pointer to next unread byte */
	uint_least16_t  tuntr_word; /* Unread data */
	tpp_char        tuntr_bits; /* # of not-yet-processed, least-significant bits in `tuntr_word` */
} tpp_unam_text_reader;
#define tpp_unam_text_reader_init(self, db_ptr)                         \
	(void)((self)->tuntr_word = ((self)->tuntr_ptr = (db_ptr) + 1)[-1], \
	       (self)->tuntr_bits = TPP_CHAR_BIT)

/* Read bits from `self` */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_char TPPCALL
tpp_unam_text_reader_readbits(tpp_unam_text_reader *tpp_restrict self, tpp_char num_bits) {
	tpp_char result, remaining_bits;
	tpp_assert(num_bits <= TPP_CHAR_BIT);
	if (self->tuntr_bits < num_bits) {
		self->tuntr_word <<= TPP_CHAR_BIT;
		self->tuntr_word |= *self->tuntr_ptr++;
		self->tuntr_bits += TPP_CHAR_BIT;
	}
	tpp_assert(self->tuntr_bits >= num_bits);
	remaining_bits = self->tuntr_bits - num_bits;
	result = (tpp_char)(self->tuntr_word >> remaining_bits);
	self->tuntr_word &= ((uint_least16_t)(1 << remaining_bits) - 1);
	self->tuntr_bits = remaining_bits;
	return result;
}

/* Decode+read a compressed unicode character from `self`.
 * s.a. the compression function `ctype-names.dee:serializeText()`
 *
 * Returns `0` if EOF has been reached */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_unichar TPPCALL
tpp_unam_text_reader_readchar(tpp_unam_text_reader *tpp_restrict self) {
	static char const simple_chars[29] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
		'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
		'U', 'V', 'W', 'X', 'Y', 'Z', '.', ':', '-'
	};
	tpp_char word = tpp_unam_text_reader_readbits(self, 1);
	if (word == 0) {
		self->tuntr_word = 0;
		self->tuntr_bits = 1;
		return 0; /* EOF */
	}
	word = tpp_unam_text_reader_readbits(self, 5);
	if (word < tpp_lengthof(simple_chars))
		return simple_chars[word];
	tpp_assert(word == 29 || word == 30 || word == 31);

	/* Special multi-byte character... */
	switch (word) {

#if TPP_UNAM_LOOKUP_USES_UNICODE5
	case 29: { /* UNICODE5 */
		word = tpp_unam_text_reader_readbits(self, 6);
		tpp_assert(word & 0x20);
		return 0x0020 + (word & 0x1f);
	}	break;
#endif /* TPP_UNAM_LOOKUP_USES_UNICODE5 */

#if TPP_UNAM_LOOKUP_USES_UNICODE10
	case 30: { /* UNICODE10 */
		tpp_unichar result;
		word = tpp_unam_text_reader_readbits(self, 6);
		tpp_assert(word & 0x20);
		result = (tpp_unichar)(word & 0x1f) << 5;
		word = tpp_unam_text_reader_readbits(self, 6);
		tpp_assert(word & 0x20);
		result |= (tpp_unichar)(word & 0x1f);
		tpp_assert(result != 0);
		return result;
	}	break;
#endif /* TPP_UNAM_LOOKUP_USES_UNICODE10 */

#if TPP_UNAM_LOOKUP_USES_UNICODE20
	case 31: { /* UNICODE20 */
		tpp_unichar result;
		word = tpp_unam_text_reader_readbits(self, 6);
		tpp_assert(word & 0x20);
		result = (tpp_unichar)(word & 0x1f) << 15;
		word = tpp_unam_text_reader_readbits(self, 6);
		tpp_assert(word & 0x20);
		result = (tpp_unichar)(word & 0x1f) << 10;
		word = tpp_unam_text_reader_readbits(self, 6);
		tpp_assert(word & 0x20);
		result = (tpp_unichar)(word & 0x1f) << 5;
		word = tpp_unam_text_reader_readbits(self, 6);
		tpp_assert(word & 0x20);
		result |= (tpp_unichar)(word & 0x1f);
		tpp_assert(result != 0);
		return result;
	}	break;
#endif /* TPP_UNAM_LOOKUP_USES_UNICODE20 */

	default: tpp_unreachable();
	}
}

/* Skip over a compressed TEXT blob, returning a pointer to what comes after. */
static TPP_PURECALL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1))
tpp_char const *TPPCALL tpp_unam_skiptext(tpp_char const *db_ptr) {
	tpp_unam_text_reader reader;
	tpp_unam_text_reader_init(&reader, db_ptr);
	while (tpp_unam_text_reader_readchar(&reader))
		;
	return reader.tuntr_ptr;
}

/* Skip over a TOKEN element, returning a pointer to what comes after. */
static TPP_PURECALL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1))
tpp_char const *TPPCALL tpp_unam_skiptoken(tpp_char const *db_ptr) {
	tpp_unichar uleb = tpp_decode_uleb128_tokenid(&db_ptr);
	if (uleb == 0)
		db_ptr = tpp_unam_skiptext(db_ptr); /* Text segment */
	return db_ptr;
}



#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM
#define tpp_unam_lexer__PARAM  , tpp_lexer const *tpp_restrict lexer
#define tpp_unam_lexer__ARG(x) , x
#else /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM */
#define tpp_unam_lexer__PARAM  /* nothing */
#define tpp_unam_lexer__ARG(x) /* nothing */
#endif /* !TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM */

typedef struct tpp_unam_token_parser {
	tpp_char const  *tuntp_pos;   /* Pointer to next character */
	tpp_char const  *tuntp_end;   /* Pointer to end-of-input */
#if TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE)
	tpp_char const  *tuntp_start; /* Start pointer */
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE) */
#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM
	tpp_lexer const *tuntp_lexer; /* [1..1][const] Current lexer (for feature flags / BSE) */
#endif /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM */
} tpp_unam_token_parser;

#define tpp_unam_token_parser_ateof(self) \
	((self)->tuntp_pos >= (self)->tuntp_end)

static TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_unam_token_parser_atsymcont(tpp_unam_token_parser const *tpp_restrict self) {
	tpp_char ch;
	tpp_char const *pos = self->tuntp_pos;
	if (pos >= self->tuntp_end)
		return false; /* EOF isn't a SYMCONT-character */
	ch = *pos;
	if (tpp_ascii_issymcont(ch))
		return true;
#if TPP_HAVE_UNICODE
	if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(self->tuntp_lexer))) {
		tpp_unichar uc = tpp_unicode_readutf8(&pos, self->tuntp_end);
		if (tpp_unicode_issymcont(uc))
			return true;
	}
#endif /* TPP_HAVE_UNICODE */
	return false;
}

/* Skip over space (and '_') characters
 * @return: true:  At least 1 character was skipped
 * @return: false: Nothing was skipped */
static TPP_NONNULL((1)) bool TPPCALL
tpp_unam_token_parser_skipspace(tpp_unam_token_parser *tpp_restrict self) {
	bool result = false;
	tpp_char const *pos = self->tuntp_pos;
again:
	pos = tpp_preparse_skipbse_fwd(self->tuntp_lexer, pos, self->tuntp_end);
	if (pos < self->tuntp_end) {
		tpp_char ch = *pos;
		if (tpp_ascii_isspace(ch) || ch == '_') {
			++pos;
			result = true;
			goto again;
		}
#if TPP_HAVE_UNICODE
		if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(self->tuntp_lexer))) {
			tpp_char const *next_pos = pos;
			tpp_unichar uc = tpp_unicode_readutf8(&next_pos, self->tuntp_end);
			if (tpp_unicode_isspace(uc)) {
				pos = next_pos;
				result = true;
				goto again;
			}
		}
#endif /* TPP_HAVE_UNICODE */

	}
	self->tuntp_pos = pos;
	return result;
}

static TPP_WUNUSED TPP_NONNULL((1, 2)) int TPPCALL
tpp_unam_token_parser_strcmp_text_ex(tpp_unam_token_parser *tpp_restrict self,
                                     tpp_unam_text_reader *tpp_restrict reader) {
	tpp_char const *pos = self->tuntp_pos;
#if TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE)
	bool is_leading = true;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE) */
	for (;;) {
		tpp_char ch;
		tpp_char const *next_pos;
		tpp_unichar lhs_uc;
		tpp_unichar rhs_uc = tpp_unam_text_reader_readchar(reader);
		if (rhs_uc == 0) {
			/* Match found! */
			self->tuntp_pos = pos;
			return 0;
		}
		next_pos = tpp_preparse_skipbse_fwd(self->tuntp_lexer, pos, self->tuntp_end);
again_read_after_space:
		if (next_pos >= self->tuntp_end) {
			self->tuntp_pos = pos;
			return -1;
		}
		ch = *next_pos++;
		if (tpp_ascii_isspace(ch) || ch == '_') {
#if TPP_HAVE_UNICODE
handle_space:
#endif /* TPP_HAVE_UNICODE */
#if TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE)
			/* Without 'TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE', only leading
			 * space must be skipped. All other space must be treated as a token
			 * terminator. */
			if (!is_leading && !tpp_lexer_has(self->tuntp_lexer, UNICODE_BYNAME_LOOKUP_ISPACE)) {
				self->tuntp_pos = pos;
				return -1;
			}
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE) */
			goto again_read_after_space; /* Skip over space */
		}

		lhs_uc = (tpp_unichar)ch;
#if TPP_HAVE_UNICODE
		if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(self->tuntp_lexer))) {
			next_pos = pos;
			lhs_uc = tpp_unicode_readutf8(&next_pos, self->tuntp_end);
			if (tpp_unicode_isspace(lhs_uc))
				goto handle_space;
		}
#endif /* TPP_HAVE_UNICODE */

		/* If characters don't match, return indicative */
		if (lhs_uc != rhs_uc) {
#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_ICASE
			if (tpp_lexer_has(self->tuntp_lexer, UNICODE_BYNAME_LOOKUP_ICASE)) {
				if (lhs_uc >= 'a' && lhs_uc <= 'z')
					lhs_uc -= 'a' - 'A';
			}
			if (lhs_uc != rhs_uc)
#endif /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_ICASE */
			{
				self->tuntp_pos = pos;
				return lhs_uc < rhs_uc ? -1 : 1;
			}
		}

		pos = next_pos;
#if TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE)
		is_leading = false;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE) */
	}
	tpp_unreachable();
}

/* Perform a strcmp() operation between text in `self` and `*p_compressed_text`.
 * For this purpose:
 * - `self` is always updated to point *at* the first non-matching character,
 *   no matter the return value.
 * - `*p_compressed_text` is only updated when `0` is returned (otherwise, it
 *   is left unmodified). When `0` is returned, it will point to the first
 *   byte *after* the compressed TEXT-block located at the start of a token
 *   record.
 *
 * @return: < 0:  `TEXT(self) < TEXT(*p_compressed_text)` -- `self->tuntp_pos`
 *                now points at the start of the first non-equal character
 * @return: > 0:  `TEXT(self) > TEXT(*p_compressed_text)` -- `self->tuntp_pos`
 *                now points at the start of the first non-equal character
 * @return: == 0: Text match achieved:
 *                - `self->tuntp_pos` now points to the first character *after* the matched token
 *                - `*p_compressed_text` now updated to point at either the 00h-byte between the
 *                  record's TEXT and ULEB128, or (if that 00h-byte was needed by TEXT), will
 *                  point at the first byte of ULEB128 (which is never another 00h, so you can
 *                  always seek to the ULEB128 by incrementing `*p_compressed_text` if the byte
 *                  pointed-to by it is 00h) */
static TPP_WUNUSED TPP_NONNULL((1, 2)) int TPPCALL
tpp_unam_token_parser_strcmp_text(tpp_unam_token_parser *tpp_restrict self,
                                  tpp_char const **tpp_restrict p_compressed_text) {
	int result;
	tpp_unam_text_reader reader;
	tpp_unam_text_reader_init(&reader, *p_compressed_text);
	result = tpp_unam_token_parser_strcmp_text_ex(self, &reader);
	if (result == 0)
		*p_compressed_text = reader.tuntr_ptr;
	return result;
}



/* Parse the next token, updating `parser` along-the-way.
 * This function greedily matches the longest token found,
 * only stopping when WHITESPACE or `_` is countered, or
 * the position described by `tuntp_end` is reached.
 *
 * When no matching token can be located, return `0` */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_unam_tokenid TPPCALL
tpp_unam_token_lookup(tpp_unam_token_parser *tpp_restrict parser) {
	tpp_unam_tokenid result = 0;
	tpp_char const *result_pos, *saved_end;
	tpp_char const *db_lo, *db_hi;
	tpp_char const *longest_missmatch;
	tpp_char const *orig_end = parser->tuntp_end;
again:
	result_pos = parser->tuntp_pos;
	saved_end = parser->tuntp_end;
	db_lo = tpp_unam_tokens + 2;
	db_hi = tpp_unam_tokens + tpp_lengthof(tpp_unam_tokens);
	longest_missmatch = result_pos;
	while (db_lo < db_hi) {
		tpp_char const *db_mid = db_lo + (db_hi - db_lo) / 2;
		tpp_char const *db_rec = tpp_unam_token_getstart(db_mid);
		tpp_char const *saved_pos = parser->tuntp_pos;
		int cmp = tpp_unam_token_parser_strcmp_text(parser, &db_rec);
		if (cmp < 0) {
			db_hi = db_rec;
		} else if (cmp > 0) {
			db_lo = db_rec;
			/* Seek to start of next record */
			while (*db_lo)
				++db_lo;
			++db_lo; /* Skip NUL */
			(void)tpp_decode_uleb128_tokenid(&db_lo);
		} else {
			if (*db_rec == 0)
				++db_rec; /* Skip 00h-byte between TEXT and ULEB128 */
			result = tpp_decode_uleb128_tokenid(&db_rec);
			result_pos = parser->tuntp_pos;
			/* We *did* get a match, but we get a longer one, too
			 * For this purpose, "db_rec" points at the next token
			 * record, which *may* have the same prefix whilst also
			 * matching even more text from "parser"! */
			db_lo = db_rec;
		}
		if (longest_missmatch < parser->tuntp_pos)
			longest_missmatch = parser->tuntp_pos;
		parser->tuntp_pos = saved_pos;
	}
	parser->tuntp_pos = result_pos;
	if (!result && (longest_missmatch > result_pos &&
	                longest_missmatch < saved_end)) {
		/* If we got a missmatch after a much of characters were matched,
		 * then we must re-attempt to a match whilst limiting ourselves
		 * to only the portion preceding the missmatch:
		 * >> \N{LATIN SMALL LETTER B}
		 *
		 * During token lookup, we might never hit "LATIN" and instead
		 * get a missmatch on "LATINATE" when "S" (or " ", depending on
		 * TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE) is parsed.
		 */
		parser->tuntp_end = longest_missmatch;
		goto again;
	}
	parser->tuntp_end = orig_end;
	return result;
}

static TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_unam_token_parser_skip_tokenid(tpp_unam_token_parser *tpp_restrict self,
                                   tpp_unam_tokenid token_id) {
	tpp_char const *orig_pos = self->tuntp_pos;
	tpp_char const *orig_end = self->tuntp_end;
	tpp_unam_tokenid parsed;
	for (;;) {
		parsed = tpp_unam_token_lookup(self);
		if (parsed == 0)
			break;
		if (parsed == token_id) {
			self->tuntp_end = orig_end;
			return true;
		}
		self->tuntp_end = self->tuntp_pos;
#if TPP_HAVE_UNICODE
		if (tpp_file_isutf8(tpp_lexer_getfile(self->tuntp_lexer))) {
			tpp_unicode_readutf8_bck(orig_pos, &self->tuntp_end);
		} else
#endif /* TPP_HAVE_UNICODE */
		{
			--self->tuntp_end;
		}
		self->tuntp_pos = orig_pos;
	}
	self->tuntp_pos = orig_pos;
	self->tuntp_end = orig_end;
	return false;
}

/* Skip over the token described by `p_db_token`
 * @param: p_db_token: Updated to point *after* match on success;
 *                     Contents are undefined on failure */
static TPP_WUNUSED TPP_NONNULL((1, 2)) bool TPPCALL
tpp_unam_token_parser_skip_token(tpp_unam_token_parser *tpp_restrict self,
                                 tpp_char const **tpp_restrict p_db_token) {
	int compare_result;
	tpp_char const *db_token = *p_db_token;
	tpp_unam_tokenid token_id = tpp_decode_uleb128_tokenid(&db_token);
	*p_db_token = db_token;
	if (token_id != 0)
		return tpp_unam_token_parser_skip_tokenid(self, token_id);

	/* Must skip over compressed TEXT */
	compare_result = tpp_unam_token_parser_strcmp_text(self, p_db_token);
	return compare_result == 0;
}

typedef tpp_char tpp_unam_node;
#define tpp_unam_node_getfeatures(self) ((self)[0])

#define TPP_UNAM_NODE_FEAT_HAS_SIBLING       0x01
#define TPP_UNAM_NODE_FEAT_HAS_CHILDREN      0x02
#define TPP_UNAM_NODE_FEAT_ONE_TOKEN         0x04
#define TPP_UNAM_NODE_FEAT_ONE_ORD           0x08
#define TPP_UNAM_NODE_FEAT_HAS_NUMBER_SUFFIX 0x10

#define TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_BASE           0x0F
#define TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_HEX            0x20
#define TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_REQUIRES_SPACE 0x40
#define TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_LAST           0x80

/* Find the first top-level node whose first token is `first_token` */
static TPP_PURECALL TPP_WUNUSED tpp_unam_node const *TPPCALL
tpp_unam_tree_enter_with_token(tpp_unam_tokenid first_token) {
#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_ENTRY_TABLE
	tpp_size lo = 0;
	tpp_size hi = tpp_lengthof(tpp_unam_tree_token_entries);
	while (lo < hi) {
		tpp_size mid = (lo + hi) / 2;
		tpp_unam_tree_token_entry const *ent = &tpp_unam_tree_token_entries[mid];
		if (first_token < ent->tubnte_token_id) {
			hi = mid;
		} else if (first_token > ent->tubnte_token_id) {
			lo = mid + 1;
		} else {
			return tpp_unam_tree + ent->tubnte_offset;
		}
	}
	return NULL;
#else /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_ENTRY_TABLE */
#error "XXX: scan table manually"
#endif /* !TPP_HAVE_UNICODE_BYNAME_LOOKUP_ENTRY_TABLE */
}

/* Find the first top-level node whose first character is `first_ch` */
static TPP_PURECALL TPP_WUNUSED tpp_unam_node const *TPPCALL
tpp_unam_tree_enter_with_char(tpp_char first_ch) {
#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_ENTRY_TABLE
	tpp_unam_tree_char_offset offset;
	if (first_ch < TPP_UNAM_TREE_CHAR_ENTRY_MINCHAR ||
	    first_ch > TPP_UNAM_TREE_CHAR_ENTRY_MAXCHAR)
		return NULL;
	offset = tpp_unam_tree_char_entries[first_ch - TPP_UNAM_TREE_CHAR_ENTRY_MINCHAR];
#if TPP_UNAM_TREE_CHAR_ENTRY_HASNULL
	if (offset == 0)
		return NULL;
#endif /* TPP_UNAM_TREE_CHAR_ENTRY_HASNULL */
	return tpp_unam_tree + offset;
#else /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_ENTRY_TABLE */
#error "XXX: scan table manually"
#endif /* !TPP_HAVE_UNICODE_BYNAME_LOOKUP_ENTRY_TABLE */
}

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_unam_node const *TPPCALL
tpp_unam_node_match_text_token_after_1char(tpp_unam_node const *tpp_restrict db_self,
                                           tpp_unam_token_parser *tpp_restrict parser,
                                           tpp_char first_ch) {
	tpp_unam_node const *result = NULL;
	tpp_char const *result_pos = parser->tuntp_pos;
	tpp_char const *after_first_ch = parser->tuntp_pos;
	do {
		int cmp;
		tpp_unam_text_reader reader;
		tpp_unam_node const *next_node = NULL;
		tpp_char const *db_iter = db_self;
		tpp_char features;
		tpp_unichar rhs_uc;
		features  = tpp_unam_node_getfeatures(db_iter);
		++db_iter; /* Skip over feature-word */
		if (features & TPP_UNAM_NODE_FEAT_HAS_SIBLING) {
			tpp_size node_size = tpp_decode_uleb128_size(&db_iter);
			next_node = db_iter + node_size;
		}
		if (*db_iter++)
			break; /* No longer a TEXT-based node */
		tpp_unam_text_reader_init(&reader, db_iter);
		rhs_uc = tpp_unam_text_reader_readchar(&reader);
		if (rhs_uc != (tpp_unichar)first_ch)
			break; /* No longer the same first character... */
		tpp_assert(rhs_uc != 0 && "Compressed TEXT should never be empty");
		cmp = tpp_unam_token_parser_strcmp_text_ex(parser, &reader);
		if (cmp < 0)
			break; /* It should have been hit already... */
		if (cmp == 0) {
			result = db_self; /* Found (possible) match! */
			result_pos = parser->tuntp_pos;
			if (!tpp_unam_token_parser_atsymcont(parser) ||
			    tpp_unam_token_parser_skipspace(parser))
				break; /* Max possible match */
		}
		parser->tuntp_pos = after_first_ch;
		db_self = next_node;
	} while (db_self);
	parser->tuntp_pos = result_pos;
	return result;
}


/* Try to match the children of some parent-node, starting with `db_first_child` */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_size TPPCALL
tpp_unam_node_matchtext_children(tpp_unam_node const *tpp_restrict db_first_child,
                                 tpp_unam_token_parser *tpp_restrict parser,
                                 tpp_unichar uc[TPP_UNICODE_BYNAME_LOOKUP_MAXUC]);

#if TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE)
static TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_unam_token_parser_skip_mandatory_space(tpp_unam_token_parser *tpp_restrict parser) {
	if (tpp_unam_token_parser_skipspace(parser)) {
		return true; /* Space can always be used to separate tokens... */
	} else if (parser->tuntp_pos <= parser->tuntp_start) {
		return true; /* OK: At start */
	} else if (!tpp_unam_token_parser_atsymcont(parser)) {
		return true; /* OK: Next character is a stand-alone token (can't continue another token) */
	} else {
		/* Check if preceding character is SYMCONT.
		 * If it isn't, then there doesn't need to be space! */
		tpp_char last_ch = parser->tuntp_pos[-1];
		if (tpp_ascii_issymcont(last_ch)) {
			return false; /* Missing space -> can't be this one (or one of its children) */
		} else
#if TPP_HAVE_UNICODE
		if (tpp_ascii_ismb(last_ch) && tpp_file_isutf8(tpp_lexer_getfile(parser->tuntp_lexer))) {
			tpp_char const *temp = parser->tuntp_pos;
			tpp_unichar last_uc = tpp_unicode_readutf8_bck(parser->tuntp_start, &temp);
			if (tpp_unicode_issymcont(last_uc))
				return false; /* Missing space -> can't be this one (or one of its children) */
		} else
#endif /* TPP_HAVE_UNICODE */
		{
		}
		return true;
	}
	tpp_unreachable();
}
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE) */


/* Same as `tpp_unam_node_matchtext_after_1token()`, but `db_iter` points
 * after the node's first TOKEN, and `features` is already given. */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_size TPPCALL
tpp_unam_node_matchtext_after_1token_ex(tpp_char const *tpp_restrict db_iter,
                                        tpp_unam_token_parser *tpp_restrict parser,
                                        tpp_unichar uc[TPP_UNICODE_BYNAME_LOOKUP_MAXUC],
                                        tpp_char features) {

	/* Parse additional tokens (verifying that all of them are present) */
	if (!(features & TPP_UNAM_NODE_FEAT_ONE_TOKEN)) {
		while (*db_iter != 1) {
			if (!tpp_unam_token_parser_skipspace(parser)) {
#if TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE)
				if (!tpp_lexer_has(parser->tuntp_lexer, UNICODE_BYNAME_LOOKUP_ISPACE)) {
					if (!tpp_unam_token_parser_skip_mandatory_space(parser))
						return 0;
				}
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE) */
			}
			if (!tpp_unam_token_parser_skip_token(parser, (tpp_char const **)&db_iter))
				return 0;
		}
		++db_iter; /* Skip over token list termination byte */
	}

	/* If space is always ignored, then we can skip
	 * over it here to simplify token matching later! */
#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE
	if (tpp_lexer_has(parser->tuntp_lexer, UNICODE_BYNAME_LOOKUP_ISPACE))
		tpp_unam_token_parser_skipspace(parser);
#endif /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE */

	/* Check for special case (before checking if "parser" contains the
	 * mandatory whitespace characters or EOF following the first token
	 * of "db_self"):
	 * - If the node requires only a single token...
	 * - ... *and* has number suffixes...
	 * - ... *and* one of those number suffixes says that
	 *   it doesn't need there to be whitespace...
	 *
	 * ... then that number-suffix must be allowed to match, no matter
	 * has been configured by `TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE`. */
	if (features & TPP_UNAM_NODE_FEAT_HAS_NUMBER_SUFFIX) {
		tpp_unichar ns_result_ord = 0;
		tpp_char const *ns_result_pos = NULL;
		for (;;) {
			tpp_unichar parser_ord;
			tpp_char parser_next;
			tpp_char ns_features  = *db_iter++;
			tpp_unichar ns_minord = tpp_decode_uleb128_unichar((tpp_char const **)&db_iter);
			tpp_unichar ns_maxord = ns_minord + tpp_decode_uleb128_unichar((tpp_char const **)&db_iter);
			tpp_unichar ns_base   = ns_features & TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_BASE;
			tpp_char const *saved_parser_pos = parser->tuntp_pos;
			if (ns_base == TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_BASE)
				ns_base = tpp_decode_uleb128_unichar((tpp_char const **)&db_iter);
#if TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE)
			if (ns_features & TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_REQUIRES_SPACE) {
				if (!tpp_unam_token_parser_skipspace(parser) &&
				    !tpp_lexer_has(parser->tuntp_lexer, UNICODE_BYNAME_LOOKUP_ISPACE) &&
				    !tpp_unam_token_parser_skip_mandatory_space(parser))
					goto next_ns; /* Missing mandatory space */
			}
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE) */
			if (!tpp_unam_token_parser_atsymcont(parser))
				goto next_ns;
			parser_next = *parser->tuntp_pos++;
			if ((ns_features & TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_HEX)
			    ? !tpp_ascii_isxdigit(parser_next)
			    : !tpp_ascii_isdigit(parser_next))
				goto next_ns;
			parser_ord = tpp_ascii_asxdigit(parser_next);
			for (;;) {
				tpp_unichar new_parser_ord;
				tpp_char const *suffix_end = parser->tuntp_pos;
#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE
				if (tpp_lexer_has(parser->tuntp_lexer, UNICODE_BYNAME_LOOKUP_ISPACE)) {
					tpp_unam_token_parser_skipspace(parser);
				} else
#endif /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE */
				{
#if TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE)
					parser->tuntp_pos = tpp_preparse_skipbse_fwd(parser->tuntp_lexer,
					                                             parser->tuntp_pos,
					                                             parser->tuntp_end);
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE) */
				}
				if (parser->tuntp_pos >= parser->tuntp_end) {
break_ns_scan:
					parser->tuntp_pos = suffix_end;
					break;
				}
				parser_next = *parser->tuntp_pos++;
				if (ns_features & TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_HEX) {
					if (!tpp_ascii_isxdigit(parser_next))
						goto break_ns_scan;
					new_parser_ord = parser_ord << 4;
					if ((new_parser_ord >> 4) != parser_ord)
						goto break_ns_scan;
					new_parser_ord |= tpp_ascii_asxdigit(parser_next);
				} else {
					if (!tpp_ascii_isdigit(parser_next))
						goto break_ns_scan;
					new_parser_ord = parser_ord * 10;
					if ((new_parser_ord / 10) != parser_ord)
						goto break_ns_scan;
					new_parser_ord += tpp_ascii_asdigit(parser_next);
				}
				if (new_parser_ord > ns_maxord)
					goto break_ns_scan;
				parser_ord = new_parser_ord;
			}
			if (parser_ord >= ns_base) {
				parser_ord -= ns_base;
				parser_ord += ns_minord;
				if (parser_ord >= ns_minord && parser_ord <= ns_maxord) {
					/* Want to eventually return the NS-match with
					 * the greatest # of input characters consumed. */
					if (ns_result_pos == NULL || (parser->tuntp_pos > ns_result_pos)) {
						ns_result_ord = parser_ord;
						ns_result_pos = parser->tuntp_pos;
					}
				}
			}
next_ns:
			parser->tuntp_pos = saved_parser_pos;
			if (ns_features & TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_LAST)
				break;
		}

		/* Check if we got a result from the number-suffix scan */
		if (ns_result_pos) {
			parser->tuntp_pos = ns_result_pos;
			uc[0] = ns_result_ord;
			return 1;
		}
	}

	/* Without `TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE`, there *needs* to be space or
	 * EOF here. The only special case here was 'TPP_UNAM_NODE_FEAT_HAS_NUMBER_SUFFIX',
	 * which may not have required space characters. */
#if TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE)
	if (!tpp_lexer_has(parser->tuntp_lexer, UNICODE_BYNAME_LOOKUP_ISPACE)) {
		if (!tpp_unam_token_parser_skip_mandatory_space(parser))
			return 0;
	}
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE) */

	/* At this point, "db_iter" points at the node's ordinal encoding!
	 *
	 * However, in the interest of always consuming as much input as
	 * we possibly can, and in case the node has children, we must
	 * first check if one of those children is able to match a longer
	 * sub- string (also: if "db_iter" doesn't have *any* ordinals,
	 * then *only* one of the children is able to match). */
	if (!tpp_unam_token_parser_ateof(parser) &&
	    (features & TPP_UNAM_NODE_FEAT_HAS_CHILDREN)) {
		tpp_size child_result;
		tpp_char const *db_children = db_iter;
		if (features & TPP_UNAM_NODE_FEAT_ONE_ORD) {
			(void)tpp_decode_uleb128_unichar(&db_children);
		} else {
			while (*db_children)
				(void)tpp_decode_uleb128_unichar(&db_children);
			++db_children; /* Skip over "ord_end" terminator */
		}

		/* Try to match children of "db_self" */
		child_result = tpp_unam_node_matchtext_children(db_children, parser, uc);
		if (child_result)
			return child_result;
	}

	/* No children (or none of them matched) -> see if our node has
	 * an ordinal sequence, and if so: that's the sequence we must
	 * return! */
	if (features & TPP_UNAM_NODE_FEAT_ONE_ORD) {
		/* Yes! we have an ordinal (1 of them to be precise) */
		uc[0] = tpp_decode_uleb128_unichar((tpp_char const **)&db_iter);
		return 1;
	}

	/* Variable-number of unicode ordinals (including 0) */
	{
		tpp_size ord_count = 0;
		while (*db_iter) {
			uc[ord_count] = tpp_decode_uleb128_unichar((tpp_char const **)&db_iter);
			++ord_count;
		}
		return ord_count;
	}
}

/* Find the perfectly matching node, whilst being allowed to assume
 * that the first token of `db_self` has already been matched (such
 * that `parser` points *after* said first token) */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_size TPPCALL
tpp_unam_node_matchtext_after_1token(tpp_unam_node const *tpp_restrict db_self,
                                     tpp_unam_token_parser *tpp_restrict parser,
                                     tpp_unichar uc[TPP_UNICODE_BYNAME_LOOKUP_MAXUC]) {
	tpp_char features = tpp_unam_node_getfeatures(db_self);
	tpp_char const *db_iter = db_self + 1;
	if (features & TPP_UNAM_NODE_FEAT_HAS_SIBLING)
		(void)tpp_decode_uleb128_size(&db_iter); /* Skip "size"-field */

	/* Skip over first token (which has already been matched by "parser") */
	db_iter = tpp_unam_skiptoken(db_iter);

	/* Try to match against this node... */
	return tpp_unam_node_matchtext_after_1token_ex(db_iter, parser, uc, features);
}

/* Try to match the children of some parent-node, starting with `db_first_child` */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_size TPPCALL
tpp_unam_node_matchtext_children(tpp_unam_node const *tpp_restrict db_first_child,
                                 tpp_unam_token_parser *tpp_restrict parser,
                                 tpp_unichar uc[TPP_UNICODE_BYNAME_LOOKUP_MAXUC]) {
	tpp_unichar match_uc[TPP_UNICODE_BYNAME_LOOKUP_MAXUC];
	tpp_size match_count = 0;
	tpp_char const *match_ptr = NULL;
	for (;;) {
		tpp_unam_node const *db_next_child = NULL;
		tpp_char child_features = tpp_unam_node_getfeatures(db_first_child);
		tpp_char const *saved_parser_pos = parser->tuntp_pos;
		++db_first_child; /* Skip over "feature" byte */
		if (child_features & TPP_UNAM_NODE_FEAT_HAS_SIBLING) {
			tpp_size child_size = tpp_decode_uleb128_size((tpp_char const **)&db_first_child);
			db_next_child = db_first_child + child_size;
		}

		/* Match the child's first token */
		if (tpp_unam_token_parser_skip_token(parser, (tpp_char const **)&db_first_child)) {
			/* Try to match against this node... */
			tpp_size this_match_count;
			this_match_count = tpp_unam_node_matchtext_after_1token_ex(db_first_child, parser,
			                                                           uc, child_features);
			if (this_match_count) {
				if (tpp_unam_token_parser_skipspace(parser) ||
				    !tpp_unam_token_parser_atsymcont(parser))
					return this_match_count; /* Perfect match! */

				/* See if this match is better (we want the longest match possible) */
				if (match_ptr == NULL || (parser->tuntp_pos > match_ptr)) {
					tpp_memcpy(match_uc, uc, this_match_count * sizeof(tpp_unichar));
					match_ptr   = parser->tuntp_pos;
					match_count = this_match_count;
				}
			}
		}

		parser->tuntp_pos = saved_parser_pos;
		if (!db_next_child)
			break;
		db_first_child = db_next_child;
	}

	if (match_ptr) {
		tpp_memcpy(uc, match_uc, match_count * sizeof(tpp_unichar));
		parser->tuntp_pos = match_ptr;
		return match_count;
	}
	return 0;
}


/* Return the unicode ordinal associated with `*p_iter`
 * @return: 0 : Unknown (`*p_iter` was left unchanged)
 * @return: * : # of unicode ordinals written to `uc` */
#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_size TPPCALL
tpp_unicode_byname_lookup(tpp_char const **tpp_restrict p_iter, tpp_char const *end,
                          tpp_unichar uc[TPP_UNICODE_BYNAME_LOOKUP_MAXUC],
                          struct tpp_lexer const *tpp_restrict lexer)
#else /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_size TPPCALL
tpp_unicode_byname_lookup(tpp_char const **tpp_restrict p_iter, tpp_char const *end,
                          tpp_unichar uc[TPP_UNICODE_BYNAME_LOOKUP_MAXUC])
#endif /* !TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM */
{
	tpp_unam_tokenid token;
	tpp_unam_token_parser parser;
	tpp_char first_ch;
	tpp_unam_node const *start_node;
	tpp_char const *max_match_end;
	tpp_char const *orig_start;
	tpp_dbg_memset(uc, TPP_UNICODE_BYNAME_LOOKUP_MAXUC * sizeof(tpp_unichar));
	parser.tuntp_pos = *p_iter;
	parser.tuntp_end = end;
#if TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE)
	parser.tuntp_start = parser.tuntp_pos;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE) */
#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM
	parser.tuntp_lexer = lexer;
#endif /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM */
	tpp_unam_token_parser_skipspace(&parser);
	orig_start = parser.tuntp_pos;

	/* Try to read the initial token */
	token = tpp_unam_token_lookup(&parser);
	if (token != 0) {
		for (;;) {
			tpp_size result;
			max_match_end = parser.tuntp_pos;
			start_node = tpp_unam_tree_enter_with_token(token);
			if (start_node == NULL)
				break;
			result = tpp_unam_node_matchtext_after_1token(start_node, &parser, uc);
			if (result) {
				tpp_assert(parser.tuntp_pos > *p_iter);
				*p_iter = parser.tuntp_pos;
				return result;
			}

			/* Actually not a match -> see if we can match a shorter leading token... */
#if TPP_HAVE_UNICODE
			if (tpp_file_isutf8(tpp_lexer_getfile(parser.tuntp_lexer))) {
				tpp_unicode_readutf8_bck(*p_iter, &max_match_end);
			} else
#endif /* TPP_HAVE_UNICODE */
			{
				--max_match_end;
			}
			parser.tuntp_pos = orig_start;
			parser.tuntp_end = max_match_end;
			tpp_assert(parser.tuntp_pos <= max_match_end);
			token = tpp_unam_token_lookup(&parser);
			tpp_assert(parser.tuntp_pos <= max_match_end);
			parser.tuntp_end = end;
			if (token == 0)
				break;
		}
		/*parser.tuntp_pos = orig_start;*/
	}

	/* Must enter tree by-char */
	parser.tuntp_pos = orig_start;
	if (parser.tuntp_pos >= end)
		return 0; /* Empty name... */
	first_ch = *parser.tuntp_pos++;
#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_ICASE
	if (tpp_lexer_has(lexer, UNICODE_BYNAME_LOOKUP_ICASE)) {
		if (first_ch >= 'a' && first_ch <= 'z')
			first_ch -= 'a' - 'A';
	}
#endif /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_ICASE */
	start_node = tpp_unam_tree_enter_with_char(first_ch);
	if (start_node == NULL)
		return 0; /* No nodes with that character */
	orig_start = parser.tuntp_pos;
	max_match_end = end;
	for (;;) {
		tpp_unam_node const *match_node;
		tpp_size result;
		match_node = tpp_unam_node_match_text_token_after_1char(start_node, &parser, first_ch);
		if (!match_node)
			break; /* Nothing exists that has a token matching this prefix... */
		max_match_end = parser.tuntp_pos;
		parser.tuntp_end = end;
		result = tpp_unam_node_matchtext_after_1token(match_node, &parser, uc);
		if (result) {
			tpp_assert(parser.tuntp_pos > *p_iter);
			*p_iter = parser.tuntp_pos;
			return result;
		}
		if (max_match_end <= orig_start)
			break; /* No shorter prefix exists */

		/* Actually not a match -> see if we can match something shorter
		 * by trying to find a child of "start_node" that matches a sequence
		 * of characters from "parser" that is 1 character shorter */
#if TPP_HAVE_UNICODE
		if (tpp_file_isutf8(tpp_lexer_getfile(parser.tuntp_lexer))) {
			tpp_unicode_readutf8_bck(orig_start, &max_match_end);
		} else
#endif /* TPP_HAVE_UNICODE */
		{
			--max_match_end;
		}
		parser.tuntp_pos = orig_start;
		parser.tuntp_end = max_match_end;
	}
	return 0;
}

#if TPP_HAVE_UNICODE_BYNAME_PRINTNEAREST

typedef struct tpp_unicode_byname_enumerate_data {
	unsigned int tubned_flags;     /* Set of `TPP_UNAM_NODE_FEAT_HAS_NUMBER_SUFFIX | TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_HEX | TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_REQUIRES_SPACE` */
	tpp_unichar  tubned_ns_minval; /* [valid_if(TPP_UNAM_NODE_FEAT_HAS_NUMBER_SUFFIX)] number-suffix min value */
	tpp_unichar  tubned_ns_maxval; /* [valid_if(TPP_UNAM_NODE_FEAT_HAS_NUMBER_SUFFIX)] number-suffix max value */
	tpp_size     tubned_size;      /* [<= TPP_UNAM_LOOKUP_NAME_MAXLEN] Length of string repr */
	char         tubned_repr[TPP_UNAM_LOOKUP_NAME_MAXLEN]; /* [tubned_size] String repr */
} tpp_unicode_byname_enumerate_data;

static TPP_NONNULL((2)) char *TPPCALL
tpp_unicode_byname_enumerate_repr_tokenid(tpp_unam_tokenid id,
                                          char *tpp_restrict dst) {
#ifdef tpp_unam_token_offsets
	/* Have a table that speeds up this function using a static lookup-table,
	 * where the index is the token-id, and the value is the offset into the
	 * token data-base, pointing to the start of the compressed TEXT segment
	 * describing that token's representation. */
	tpp_unichar uc;
	tpp_unam_text_reader reader;
	tpp_char const *compressed_name;
	tpp_assert(id < tpp_lengthof(tpp_unam_token_offsets));
	compressed_name = tpp_unam_tokens + tpp_unam_token_offsets[id];
	tpp_unam_text_reader_init(&reader, compressed_name);
	while ((uc = tpp_unam_text_reader_readchar(&reader)) != 0)
		dst = (char *)tpp_unicode_writeutf8((tpp_char *)dst, uc);
#else /* tpp_unam_token_offsets */
	/* NOTE: This fallback code is too slow to actually be usable */
	tpp_char const *db_iter = tpp_unam_tokens + 2;
	for (;;) {
		tpp_unam_tokenid iter_id;
		tpp_char const *before_text = db_iter;
		tpp_assert(db_iter < (tpp_unam_tokens + sizeof(tpp_unam_tokens)));
		db_iter = tpp_unam_skiptext(db_iter);
		if (db_iter[-1])
			++db_iter; /* Skip extra NUL byte */
		iter_id = tpp_decode_uleb128_tokenid(&db_iter);
		if (iter_id == id) {
			/* Found it! */
			tpp_unichar uc;
			tpp_unam_text_reader reader;
			tpp_unam_text_reader_init(&reader, before_text);
			while ((uc = tpp_unam_text_reader_readchar(&reader)) != 0)
				dst = (char *)tpp_unicode_writeutf8((tpp_char *)dst, uc);
			break;
		}
	}
#endif /* !tpp_unam_token_offsets */
	return dst;
}

/* Decode a TOKEN element at `*p_iter` and write its textual representation to `dst` */
static TPP_NONNULL((1, 2)) char *TPPCALL
tpp_unicode_byname_enumerate_decode_token(tpp_char const **tpp_restrict p_iter,
                                          char *tpp_restrict dst) {
	tpp_char const *db_iter = *p_iter;
	if (*db_iter) {
		/* Token ID */
		tpp_unam_tokenid id = tpp_decode_uleb128_tokenid(&db_iter);
		dst = tpp_unicode_byname_enumerate_repr_tokenid(id, dst);
	} else {
		/* Inline text... */
		tpp_unichar uc;
		tpp_unam_text_reader reader;
		++db_iter; /* Skip leading NUL-marker-byte */
		tpp_unam_text_reader_init(&reader, db_iter);
		while ((uc = tpp_unam_text_reader_readchar(&reader)) != 0)
			dst = (char *)tpp_unicode_writeutf8((tpp_char *)dst, uc);
		db_iter = reader.tuntr_ptr;
	}
	*p_iter = db_iter;
	return dst;
}

/* Decode a list of number-suffix groups and invoke "cb" for each of them. */
static TPP_NONNULL((1, 2, 3)) tpp_ssize TPPCALL
tpp_unicode_byname_enumerate_decode_ns(tpp_char const **tpp_restrict p_iter,
                                       tpp_unicode_byname_enumerate_data *data,
                                       tpp_ssize (TPPCALL *cb)(tpp_unicode_byname_enumerate_data *data)) {
	tpp_ssize temp, result = 0;
	tpp_char const *db_iter = *p_iter;
	for (;;) {
		tpp_char ns_features = *db_iter++;
		tpp_unichar ns_range;
		(void)tpp_decode_uleb128_unichar(&db_iter); /* MINORD */
		ns_range = tpp_decode_uleb128_unichar(&db_iter);
		data->tubned_ns_minval = ns_features & TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_BASE;
		if (data->tubned_ns_minval == TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_BASE)
			data->tubned_ns_minval = tpp_decode_uleb128_unichar(&db_iter);
		data->tubned_ns_maxval = data->tubned_ns_minval + ns_range;
		data->tubned_flags &= ~(TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_HEX |
		                        TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_REQUIRES_SPACE);
		data->tubned_flags |= ns_features & (TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_HEX |
		                                     TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_REQUIRES_SPACE);
		temp = (*cb)(data);
		if (temp < 0)
			return temp;
		result += temp;
		if (ns_features & TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_LAST)
			break;
	}
	*p_iter = db_iter;
	return result;
}

static TPP_NONNULL((1, 2, 3)) tpp_ssize TPPCALL
tpp_unicode_byname_enumerate_impl(tpp_unicode_byname_enumerate_data *tpp_restrict data,
                                  tpp_char const *tpp_restrict db_iter,
                                  tpp_ssize (TPPCALL *cb)(tpp_unicode_byname_enumerate_data *data)) {
	tpp_ssize temp, result = 0;
	tpp_char feat;
	tpp_size basesize = data->tubned_size;
	do {
		tpp_char const *next_node = NULL;
		bool has_ord;
		char *write_dst;
		feat = *db_iter++;
		data->tubned_flags = feat;
		if (feat & TPP_UNAM_NODE_FEAT_HAS_SIBLING) {
			tpp_size node_size = tpp_decode_uleb128_size((tpp_char const **)&db_iter);
			next_node = db_iter + node_size;
		}
		write_dst = data->tubned_repr + basesize;
		write_dst = tpp_unicode_byname_enumerate_decode_token((tpp_char const **)&db_iter, write_dst);
		if (!(feat & TPP_UNAM_NODE_FEAT_ONE_TOKEN)) {
			/* Multiple token -> decode more (and inject spaces) */
			while (*db_iter != 0x01) {
				*write_dst++ = ' ';
				write_dst = tpp_unicode_byname_enumerate_decode_token((tpp_char const **)&db_iter, write_dst);
			}
			++db_iter; /* Skip over token list termination byte */
		}
		data->tubned_size = (tpp_size)(write_dst - data->tubned_repr);
		tpp_assert(data->tubned_size <= TPP_UNAM_LOOKUP_NAME_MAXLEN);
		if (feat & TPP_UNAM_NODE_FEAT_HAS_NUMBER_SUFFIX) {
			temp = tpp_unicode_byname_enumerate_decode_ns((tpp_char const **)&db_iter, data, cb);
			if (temp < 0)
				return temp;
			result += temp;
		}
		if (feat & TPP_UNAM_NODE_FEAT_ONE_ORD) {
			has_ord = true;
			(void)tpp_decode_uleb128_unichar((tpp_char const **)&db_iter);
		} else {
			has_ord = false;
			while (*db_iter) {
				(void)tpp_decode_uleb128_unichar((tpp_char const **)&db_iter);
				has_ord = true;
			}
			++db_iter;
		}

		/* Invoke for ordinal (if there is one) */
		if (has_ord) {
			temp = (*cb)(data);
			if (temp < 0)
				return temp;
			result += temp;
		}

		/* Recurse for children */
		if (feat & TPP_UNAM_NODE_FEAT_HAS_CHILDREN) {
			tpp_assert(data->tubned_size < TPP_UNAM_LOOKUP_NAME_MAXLEN);
			data->tubned_repr[data->tubned_size++] = ' ';
			temp = tpp_unicode_byname_enumerate_impl(data, db_iter, cb);
			if (temp < 0)
				return temp;
			result += temp;
		}
		db_iter = next_node;
	} while (db_iter);
	return result;
}

static TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_unicode_byname_enumerate(tpp_unicode_byname_enumerate_data *data,
                             tpp_ssize (TPPCALL *cb)(tpp_unicode_byname_enumerate_data *data)) {
	data->tubned_size = 0;
	return tpp_unicode_byname_enumerate_impl(data, tpp_unam_tree, cb);
}


typedef struct tpp_unicode_byname_printnearest_data {
	tpp_unicode_byname_enumerate_data tubnpnd_enum;      /* Enumeration data */
	tpp_size                          tubnpnd_win_score; /* Winner score */
	tpp_unicode_byname_enumerate_data tubnpnd_win_data;  /* Winner data */
	tpp_size                          tubnpnd_usr_size;  /* User-specified string size (w/ number suffix) */
	char                              tubnpnd_usr_name[TPP_UNAM_LOOKUP_NAME_MAXLEN]; /* [tubnpnd_usr_size] User-specified string */
	tpp_size                          tubnpnd_usr_nsdsiz; /* [<= tubnpnd_usr_size] Size of user-defined name with decimal number-suffix removed */
	tpp_unichar                       tubnpnd_usr_nsdval; /* [valid_if(tubnpnd_usr_nsdsiz < tubnpnd_usr_size)] Number-suffix decimal value */
	tpp_size                          tubnpnd_usr_nsxsiz; /* [<= tubnpnd_usr_size] Size of user-defined name with hex number-suffix removed */
	tpp_unichar                       tubnpnd_usr_nsxval; /* [valid_if(tubnpnd_usr_bsize < tubnpnd_usr_size)] Number-suffix hex value */
} tpp_unicode_byname_printnearest_data;

static tpp_ssize TPPCALL
tpp_unicode_byname_printnearest_cb(tpp_unicode_byname_enumerate_data *data) {
	tpp_unicode_byname_printnearest_data *self;
	tpp_size this_score = 0, usr_size;
	self = tpp_container_of(data, tpp_unicode_byname_printnearest_data, tubnpnd_enum);
	/*printf("NAME: '%.*s'\n",
	       (int)self->tubnpnd_enum.tubned_size,
	       self->tubnpnd_enum.tubned_repr);*/
	if (self->tubnpnd_enum.tubned_flags & TPP_UNAM_NODE_FEAT_HAS_NUMBER_SUFFIX) {
		bool hex = (self->tubnpnd_enum.tubned_flags & TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_HEX) != 0;
		tpp_unichar usr_nsval;
		usr_size = hex ? self->tubnpnd_usr_nsxsiz : self->tubnpnd_usr_nsdsiz;
		usr_nsval = hex ? self->tubnpnd_usr_nsxval : self->tubnpnd_usr_nsdval;
		if (self->tubnpnd_enum.tubned_flags & TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_REQUIRES_SPACE) {
			if (usr_size && self->tubnpnd_usr_name[usr_size - 1] == ' ') {
				--usr_size;
			} else {
				this_score += 0xff;
			}
		}

		/* Add score if ranges don't match */
		if (usr_nsval < self->tubnpnd_enum.tubned_ns_minval) {
			this_score += self->tubnpnd_enum.tubned_ns_minval - usr_nsval;
		} else if (usr_nsval > self->tubnpnd_enum.tubned_ns_maxval) {
			this_score += usr_nsval - self->tubnpnd_enum.tubned_ns_maxval;
		}
	} else {
		usr_size = self->tubnpnd_usr_size;
	}

	/* Fuzzy compare character names */
	this_score += tpp_fuzzy_memcmp((tpp_char const *)self->tubnpnd_enum.tubned_repr,
	                               self->tubnpnd_enum.tubned_size,
	                               (tpp_char const *)self->tubnpnd_usr_name,
	                               usr_size);

	/* Compare score against winning score */
	if (this_score < self->tubnpnd_win_score) {
		/* We got a new leader! */
		self->tubnpnd_win_score = this_score;
		self->tubnpnd_win_data  = self->tubnpnd_enum;
	}
	return 0;
}

static TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL
tpp_utoa_hex(char buf[TPP_UTOA_MAXLEN], tpp_uintmax value) {
	char *ptr = buf + TPP_UTOA_MAXLEN;
	do {
		tpp_char nibble = (tpp_char)(value & 0xf);
		*--ptr = tpp_ascii_touprxdigit(nibble);
		value >>= 3;
	} while (value);
	return ptr;
}


/* Print the name of some unicode character name that matches the
 * given `name` most closely.
 *
 * @return: * : Sum of return values of `printer`
 * @return: <0: First negative return value of `printer` */
#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 5)) tpp_size TPPCALL
tpp_unicode_byname_printnearest(tpp_char const *start, tpp_char const *end,
                                tpp_formatprinter printer, void *arg,
                                struct tpp_lexer const *tpp_restrict lexer)
#else /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_size TPPCALL
tpp_unicode_byname_printnearest(tpp_char const *start, tpp_char const *end,
                                tpp_formatprinter printer, void *arg)
#endif /* !TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM */
{
	tpp_ssize temp, result;
	tpp_unicode_byname_printnearest_data data;
#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM
	(void)lexer;
#endif /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM */

	/* Parse+normalize given "start...+=end" buffer to:
	 * - Replace all space characters with U+0020 SPACE
	 * - Convert lower-case to upper-case
	 * - Skip over BSE sequences */
	{
		tpp_char const *iter = start;
		char *dst, *dst_end;
		dst_end = (dst = data.tubnpnd_usr_name) + sizeof(data.tubnpnd_usr_name);
#if TPP_HAVE_UNICODE
		if (tpp_file_isutf8(tpp_lexer_getfile(lexer))) {
			while (iter < end) {
				tpp_unichar uc = tpp_unicode_readutf8(&iter, end);
				iter = tpp_preparse_skipbse_fwd(lexer, iter, end);
				if (tpp_unicode_isspace(uc)) {
again_skip_space_utf8:
					/* Skip consecutive space characters */
					if (iter >= end)
						break;
					uc = tpp_unicode_readutf8(&iter, end);
					iter = tpp_preparse_skipbse_fwd(lexer, iter, end);
					if (tpp_unicode_isspace(uc))
						goto again_skip_space_utf8;
					*dst++ = ' ';
					if (dst >= dst_end)
						break;
				}
				if (uc >= 'a' && uc <= 'z')
					uc -= 'a' - 'A';
				if (uc < 0x80) {
					*dst++ = (tpp_char)uc;
				} else {
					tpp_char buf[TPP_UTF8_MAXLEN];
					tpp_size buflen;
					buflen = tpp_unicode_writeutf8(buf, uc) - buf;
					if ((dst + buflen) > dst_end)
						break;
					dst = (char *)tpp_mempcpy(dst, buf, buflen);
				}
				if (dst >= dst_end)
					break;
			}
		} else
#endif /* TPP_HAVE_UNICODE */
		{
			while (iter < end) {
				tpp_char ch = *iter++;
				iter = tpp_preparse_skipbse_fwd(lexer, iter, end);
				if (tpp_ascii_isspace(ch)) {
again_skip_space:
					/* Skip consecutive space characters */
					if (iter >= end)
						break;
					ch = *iter++;
					iter = tpp_preparse_skipbse_fwd(lexer, iter, end);
					if (tpp_ascii_isspace(ch))
						goto again_skip_space;
					*dst++ = ' ';
					if (dst >= dst_end)
						break;
				}
				if (ch >= 'a' && ch <= 'z')
					ch -= 'a' - 'A';
				*dst++ = ch;
				if (dst >= dst_end)
					break;
			}
		}
		data.tubnpnd_usr_size = (tpp_size)(dst - data.tubnpnd_usr_name);
	}

	/* Try to decode trailing hex/decimal number suffixes */
	data.tubnpnd_usr_nsdsiz = data.tubnpnd_usr_size;
	data.tubnpnd_usr_nsxsiz = data.tubnpnd_usr_size;
	if (data.tubnpnd_usr_size) {
		tpp_char lastch = (tpp_char)data.tubnpnd_usr_name[data.tubnpnd_usr_size - 1];
		if (tpp_ascii_isxdigit(lastch)) {
			tpp_unichar dval = 0;
			tpp_unichar xval = tpp_ascii_asxdigit(lastch);
			tpp_unichar dmul = 1;
			tpp_unichar xmul = 1;
			char const *xend = data.tubnpnd_usr_name + data.tubnpnd_usr_size - 1;
			char const *dend = data.tubnpnd_usr_name + data.tubnpnd_usr_size;
			if (tpp_ascii_isdigit(lastch)) {
				dval = xval;
				dend = xend;
			}
			while (xend > data.tubnpnd_usr_name) {
				tpp_unichar digit;
				lastch = (tpp_char)xend[-1];
				if (!tpp_ascii_isxdigit(lastch))
					break;
				digit = tpp_ascii_asxdigit(lastch);
				if (dend == xend && digit < 10 && (dmul * 16) < dmul) {
					dmul *= 10;
					dval += dmul * digit;
					dend = xend - 1;
				}
				if ((xmul * 16) < xmul)
					break;
				xmul *= 16;
				xval += xmul * digit;
				--xend;
			}
			data.tubnpnd_usr_nsdsiz = (tpp_size)((data.tubnpnd_usr_name + data.tubnpnd_usr_size) - dend);
			data.tubnpnd_usr_nsdval = dval;
			data.tubnpnd_usr_nsxsiz = (tpp_size)((data.tubnpnd_usr_name + data.tubnpnd_usr_size) - xend);
			data.tubnpnd_usr_nsxval = xval;

			/* Skip over leading 0-characters */
			while (data.tubnpnd_usr_nsdsiz && data.tubnpnd_usr_name[data.tubnpnd_usr_nsdsiz - 1] == '0')
				--data.tubnpnd_usr_nsdsiz;
			while (data.tubnpnd_usr_nsxsiz && data.tubnpnd_usr_name[data.tubnpnd_usr_nsxsiz - 1] == '0')
				--data.tubnpnd_usr_nsxsiz;
		}
	}

	/* Enumerate unicode name database to find closest matching name */
	data.tubnpnd_win_score = TPP_SIZE_MAX;
	data.tubnpnd_win_data.tubned_size = 0;
	data.tubnpnd_win_data.tubned_flags = 0;
	(void)tpp_unicode_byname_enumerate(&data.tubnpnd_enum, &tpp_unicode_byname_printnearest_cb);

	/* Print name of closest unicode character name */
	result = tpp_formatprinter_print_cstr(printer, arg,
	                                      data.tubnpnd_win_data.tubned_repr,
	                                      data.tubnpnd_win_data.tubned_size);
	if (result < 0)
		return result;

	/* Deal with special case: if winner has a number-suffix, also print that suffix:
	 * >> MY UNICODE CHAR {<lo>,...,<hi>}    <-- If "TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_REQUIRES_SPACE"
	 * >> MY UNICODE CHAR{<lo>,...,<hi>}
	 */
	if (data.tubnpnd_win_data.tubned_flags & TPP_UNAM_NODE_FEAT_HAS_NUMBER_SUFFIX) {
		char minbuf[TPP_UTOA_MAXLEN], *minptr;
		char maxbuf[TPP_UTOA_MAXLEN], *maxptr;
		if (data.tubnpnd_win_data.tubned_flags & TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_REQUIRES_SPACE) {
			temp = tpp_formatprinter_print_conststr(printer, arg, " ");
			if (temp < 0)
				return temp;
			result += temp;
		}
		if (data.tubnpnd_win_data.tubned_flags & TPP_UNAM_NODE_NUMBER_SUFFIX_FEAT_HEX) {
			minptr = tpp_utoa_hex(minbuf, data.tubnpnd_win_data.tubned_ns_minval);
			maxptr = tpp_utoa_hex(maxbuf, data.tubnpnd_win_data.tubned_ns_maxval);
		} else {
			minptr = tpp_utoa(minbuf, data.tubnpnd_win_data.tubned_ns_minval);
			maxptr = tpp_utoa(maxbuf, data.tubnpnd_win_data.tubned_ns_maxval);
		}
		temp = tpp_formatprinter_print_conststr(printer, arg, "{");
		if (temp < 0)
			return temp;
		result += temp;
		temp = tpp_formatprinter_print_cstr(printer, arg, minptr,
		                                    (tpp_size)((minbuf + TPP_UTOA_MAXLEN) - minptr));
		if (temp < 0)
			return temp;
		result += temp;
		if (data.tubnpnd_win_data.tubned_ns_minval == data.tubnpnd_win_data.tubned_ns_maxval) {
			/* ... */
			temp = 0;
		} else if ((data.tubnpnd_win_data.tubned_ns_minval + 1) == data.tubnpnd_win_data.tubned_ns_maxval) {
			/* ... */
			temp = tpp_formatprinter_print_conststr(printer, arg, ", ");
		} else {
			temp = tpp_formatprinter_print_conststr(printer, arg, ", ..., ");
		}
		if (temp < 0)
			return temp;
		result += temp;
		temp = tpp_formatprinter_print_cstr(printer, arg, maxptr,
		                                    (tpp_size)((maxbuf + TPP_UTOA_MAXLEN) - maxptr));
		if (temp < 0)
			return temp;
		result += temp;
		temp = tpp_formatprinter_print_conststr(printer, arg, "}");
		if (temp < 0)
			return temp;
		result += temp;
	}
	return result;
}
#endif /*  TPP_HAVE_UNICODE_BYNAME_PRINTNEAREST*/

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !tpp_unicode_byname_lookup */
#endif /* TPP_HAVE_UNICODE_BYNAME_LOOKUP */

#endif /* !GUARD_TPP_CTYPE_NAMES_C */
