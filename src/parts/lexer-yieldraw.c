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
#ifndef GUARD_TPP_LEXER_YIELDRAW_C
#define GUARD_TPP_LEXER_YIELDRAW_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "ctype.h"
#include "extensions.h"
#include "features.h"
#include "file.h"
#include "keyword.h"
#include "lexer.h"
#include "token.h"
#include "warnings.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_UNICODE

/* clang-format off */
#define TPP_UTF8_SEQLEN_INIT(_0, _1, _2, _3, _4, _5, _6, _7, _8)         \
	{                                                                    \
		/* Unicode follow-up word (`0b10??????'). */                     \
		_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0, /* 0x80-0x8f */ \
		_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0, /* 0x90-0x9f */ \
		_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0, /* 0xa0-0xaf */ \
		_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0, /* 0xb0-0xbf */ \
		/* `0b110?????' */                                               \
		_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2, /* 0xc0-0xcf */ \
		_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2, /* 0xd0-0xdf */ \
		/* `0b1110????' */                                               \
		_3,_3,_3,_3,_3,_3,_3,_3,_3,_3,_3,_3,_3,_3,_3,_3, /* 0xe0-0xef */ \
		/* `0b11110???' */                                               \
		_4,_4,_4,_4,_4,_4,_4,_4,                         /* 0xf0-0xf7 */ \
		_5,_5,_5,_5,                                     /* 0xf8-0xfb */ \
		_6,_6,                                           /* 0xfc-0xfd */ \
		_7,                                              /* 0xfe */      \
		_8                                               /* 0xff */      \
	}
/* clang-format on */
static uint_least8_t const tpp_unicode_utf8seqlen[128] =
TPP_UTF8_SEQLEN_INIT(0, ~, 2, 3, 4, 0, 0, 0, 0);
static uint_least8_t const tpp_unicode_utf8seqlen_safe[128] =
TPP_UTF8_SEQLEN_INIT(1, ~, 2, 3, 4, 5, 6, 7, 8);
#undef TPP_UTF8_SEQLEN_INIT



/* Read a single unicode character from a given utf-8 blob.
 * WARNING: This function doesn't do any validity checking,
 *          allowing over-long utf-8 sequences, as well as
 *          incorrectly positioned UTF-8 continuation bytes. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_unichar TPPCALL
tpp_unicode_readutf8(tpp_char const **p_pos, tpp_char const *end) {
	tpp_char const *pos = *p_pos;
	tpp_unichar uc;
	if tpp_unlikely(pos >= end)
		return 0;
	uc = (tpp_unichar)*pos++;
	if (uc >= 0xc0) {
		uint_least8_t len = tpp_unicode_utf8seqlen_safe[uc & 0x7f];
		tpp_size maxlen = (tpp_size)(end - pos);
		if ((tpp_size)len > maxlen)
			len = (uint_least8_t)maxlen;
		switch (len) {
		case 0:
		case 1:
			break;
		case 2:
			uc = (uc & 0x1f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		case 3:
			uc  = (uc & 0x0f) << 12;
			uc |= (*pos++ & 0x3f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		case 4:
			uc  = (uc & 0x07) << 18;
			uc |= (*pos++ & 0x3f) << 12;
			uc |= (*pos++ & 0x3f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		case 5:
			uc = (uc & 0x03) << 24;
			uc |= (*pos++ & 0x3f) << 18;
			uc |= (*pos++ & 0x3f) << 12;
			uc |= (*pos++ & 0x3f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		case 6:
			uc = (uc & 0x01) << 30;
			uc |= (*pos++ & 0x3f) << 24;
			uc |= (*pos++ & 0x3f) << 18;
			uc |= (*pos++ & 0x3f) << 12;
			uc |= (*pos++ & 0x3f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		case 7:
			uc = (*pos++ & 0x03/*0x3f*/) << 30;
			uc |= (*pos++ & 0x3f) << 24;
			uc |= (*pos++ & 0x3f) << 18;
			uc |= (*pos++ & 0x3f) << 12;
			uc |= (*pos++ & 0x3f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		case 8:
			/*uc = (*pos & 0x3f) << 36;*/
			++pos;
			uc = (*pos++ & 0x03/*0x3f*/) << 30;
			uc |= (*pos++ & 0x3f) << 24;
			uc |= (*pos++ & 0x3f) << 18;
			uc |= (*pos++ & 0x3f) << 12;
			uc |= (*pos++ & 0x3f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		default: tpp_unreachable();
		}
	}
	*p_pos = pos;
	return uc;
}

/* Same as `tpp_unicode_readutf8()', but read in reverse */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_unichar TPPCALL
tpp_unicode_readutf8_rev(tpp_char const **p_end, tpp_char const *base) {
	tpp_unichar uc;
	tpp_char const *iter = *p_end;
	uint_least8_t seqlen = 1;
	if tpp_unlikely(iter <= base)
		return 0;
	for (;;) {
		uc = *--iter;
		if ((uc & 0xc0) != 0x80)
			break;
		if (seqlen >= 8)
			break;
		++seqlen;
		if (iter <= base)
			break;
	}
	if (uc >= 0xc0) {
		switch (seqlen) {

		case 0:
		case 1:
			break;

		case 2:
			uc  = (uc & 0x1f) << 6;
			uc |= (iter[0] & 0x3f);
			break;

		case 3:
			uc  = (uc & 0x0f) << 12;
			uc |= (iter[0] & 0x3f) << 6;
			uc |= (iter[1] & 0x3f);
			break;

		case 4:
			uc  = (uc & 0x07) << 18;
			uc |= (iter[0] & 0x3f) << 12;
			uc |= (iter[1] & 0x3f) << 6;
			uc |= (iter[2] & 0x3f);
			break;

		case 5:
			uc  = (uc & 0x03) << 24;
			uc |= (iter[0] & 0x3f) << 18;
			uc |= (iter[1] & 0x3f) << 12;
			uc |= (iter[2] & 0x3f) << 6;
			uc |= (iter[3] & 0x3f);
			break;

		case 6:
			uc  = (uc & 0x01) << 30;
			uc |= (iter[0] & 0x3f) << 24;
			uc |= (iter[1] & 0x3f) << 18;
			uc |= (iter[2] & 0x3f) << 12;
			uc |= (iter[3] & 0x3f) << 6;
			uc |= (iter[4] & 0x3f);
			break;

		case 7:
			uc  = (iter[0] & 0x03/*0x3f*/) << 30;
			uc |= (iter[1] & 0x3f) << 24;
			uc |= (iter[2] & 0x3f) << 18;
			uc |= (iter[3] & 0x3f) << 12;
			uc |= (iter[4] & 0x3f) << 6;
			uc |= (iter[5] & 0x3f);
			break;

		case 8:
			/*result = (iter[0] & 0x3f) << 36;*/
			uc  = (iter[1] & 0x03/*0x3f*/) << 30;
			uc |= (iter[2] & 0x3f) << 24;
			uc |= (iter[3] & 0x3f) << 18;
			uc |= (iter[4] & 0x3f) << 12;
			uc |= (iter[5] & 0x3f) << 6;
			uc |= (iter[6] & 0x3f);
			break;

		default: tpp_unreachable();
		}
	}
	*p_end = iter;
	return uc;
}


/* Decode a single utf-8 character.
 * - If necessary, expand the current file's chunk
 * - If an illegal utf-8 byte sequence is encountered,
 *   change TPP_FILE_ENCODING_UTF8 to TPP_FILE_ENCODING_ASCII,
 *   leave `*p_pos' unchanged, set `*p_result = 0' and return
 *   `TPP_EOK'. However, if TPP_FILE_ENCODING_FORCE_UTF8 is
 *   used, this never happens and *all* characters are valid
 *
 * Caller must ensure:
 * - That `tpp_file_isutf8(tpp_lexer_getfile(self))'
 * - That `*p_pos < tpp_lexer_getfile(self)->tf_end'
 * - That `tpp_ascii_ismb(**p_pos)'
 */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_readutf8(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos,
                   tpp_unichar *tpp_restrict p_result) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
	tpp_char const *end = file->tf_end;
	uint_least8_t len;
	tpp_unichar uc;
	tpp_assert(tpp_file_isutf8(file));
	tpp_assert(pos < end);
	uc = *pos;
	tpp_assert(tpp_ascii_ismb(uc));
	len = tpp_unicode_utf8seqlen[uc & 0x7f];
	if tpp_unlikely(len == 0)
		goto handle_ilseq;
	tpp_assert(len >= 2 && len <= 4);
	while tpp_unlikely((pos + len) > end) {
		/* Must extend the file */
		tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
		tpp_size rel_end = tpp_file_ptr2rel(file, end);
		tpp_errno error  = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			return error;
		end = tpp_file_rel2ptr(file, rel_end);
		tpp_assert(end <= file->tf_end);
		if tpp_unlikely(end >= file->tf_end)
			goto handle_ilseq; /* EOF in incomplete UTF-8 sequence :( */
		pos = tpp_file_rel2ptr(file, rel_pos);
		end = file->tf_end;
	}

	/* All required data has been loaded into memory -> parse the sequence */
	switch (len) {
	case 2:
		uc = (uc & 0x1f) << 6;
		uc |= (pos[1] & 0x3f);
		if tpp_unlikely(uc <= TPP_UTF8_1BYTE_MAX)
			goto handle_ilseq; /* under-long utf-8 sequence */
		break;
	case 3:
		uc  = (uc & 0x0f) << 12;
		uc |= (pos[1] & 0x3f) << 6;
		uc |= (pos[2] & 0x3f);
		if tpp_unlikely(uc <= TPP_UTF8_2BYTE_MAX)
			goto handle_ilseq; /* under-long utf-8 sequence */
		break;
	case 4:
		uc  = (uc & 0x07) << 18;
		uc |= (pos[1] & 0x3f) << 12;
		uc |= (pos[2] & 0x3f) << 6;
		uc |= (pos[3] & 0x3f);
		if tpp_unlikely(uc <= TPP_UTF8_3BYTE_MAX)
			goto handle_ilseq; /* under-long utf-8 sequence */
		break;
	/* 5+ doesn't appear in `tpp_unicode_utf8seqlen' */
	default: tpp_unreachable();
	}

	/* Write back results... */
write_result:
	pos += len;
	*p_pos    = pos;
	*p_result = uc;
	return TPP_EOK;

handle_ilseq:
	if (file->tf_enc == TPP_FILE_ENCODING_UTF8) {
		/* Automatic UTF-8 -> switch to ASCII */
		file->tf_enc = TPP_FILE_ENCODING_ASCII;
		*p_result = 0;
		return TPP_EOK;
	}

	/* Forced UTF-8 */
	len = tpp_unicode_utf8seqlen_safe[uc & 0x7f];
	tpp_assert(len >= 1 && len <= 8);

	while ((pos + len) > end) {
		/* Must extend the file */
		tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
		tpp_size rel_end = tpp_file_ptr2rel(file, end);
		tpp_errno error  = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			return error;
		end = tpp_file_rel2ptr(file, rel_end);
		tpp_assert(end <= file->tf_end);
		pos = tpp_file_rel2ptr(file, rel_pos);
		if tpp_unlikely(end >= file->tf_end) {
			/* EOF in incomplete UTF-8 sequence :( */
			end = file->tf_end;
			len = (uint_least8_t)(tpp_size)(end - pos);
			tpp_assert(len >= 1 && len <= 8);
			break;
		}
		end = file->tf_end;
	}

	switch (len) {
	case 1:
		break;
	case 2:
		uc = (uc & 0x1f) << 6;
		uc |= (pos[1] & 0x3f);
		break;
	case 3:
		uc  = (uc & 0x0f) << 12;
		uc |= (pos[1] & 0x3f) << 6;
		uc |= (pos[2] & 0x3f);
		break;
	case 4:
		uc  = (uc & 0x07) << 18;
		uc |= (pos[1] & 0x3f) << 12;
		uc |= (pos[2] & 0x3f) << 6;
		uc |= (pos[3] & 0x3f);
		break;
	case 5:
		uc = (uc & 0x03) << 24;
		uc |= (pos[1] & 0x3f) << 18;
		uc |= (pos[2] & 0x3f) << 12;
		uc |= (pos[3] & 0x3f) << 6;
		uc |= (pos[4] & 0x3f);
		break;
	case 6:
		uc = (uc & 0x01) << 30;
		uc |= (pos[1] & 0x3f) << 24;
		uc |= (pos[2] & 0x3f) << 18;
		uc |= (pos[3] & 0x3f) << 12;
		uc |= (pos[4] & 0x3f) << 6;
		uc |= (pos[5] & 0x3f);
		break;
	case 7:
		uc = (pos[1] & 0x03/*0x3f*/) << 30;
		uc |= (pos[2] & 0x3f) << 24;
		uc |= (pos[3] & 0x3f) << 18;
		uc |= (pos[4] & 0x3f) << 12;
		uc |= (pos[5] & 0x3f) << 6;
		uc |= (pos[6] & 0x3f);
		break;
	case 8:
		/*uc = (pos[1] & 0x3f) << 36;*/
		uc = (pos[2] & 0x03/*0x3f*/) << 30;
		uc |= (pos[3] & 0x3f) << 24;
		uc |= (pos[4] & 0x3f) << 18;
		uc |= (pos[5] & 0x3f) << 12;
		uc |= (pos[6] & 0x3f) << 6;
		uc |= (pos[7] & 0x3f);
		break;
	default: tpp_unreachable();
	}
	goto write_result;
}
#endif /* TPP_HAVE_UNICODE */


/* Skip all space character (excluding line-feeds) starting
 * at "*p_pos" (this handles both ASCII and UNICODE space) */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_skipspace_nolf(tpp_lexer *tpp_restrict self,
                         tpp_char const **tpp_restrict p_pos) {
	tpp_errno error;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
	tpp_char const *end = file->tf_end;
	tpp_char ch;
again:
	if (pos >= end) {
		/* Must extend the file */
		tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
		error = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			goto return_error;
		pos = tpp_file_rel2ptr(file, rel_pos);
		end = file->tf_end;
		if (pos >= end)
			goto done;
	}

	ch = *pos;
	if (tpp_ascii_isspace_nolf(ch)) {
		++pos;
		goto again;
	}

#if TPP_HAVE_UNICODE
	if (tpp_ascii_ismb(ch) && tpp_file_isutf8(file)) {
		tpp_unichar uc;
		tpp_char const *npos = pos;
		error = tpp_lexer_readutf8(self, &npos, &uc);
		if (TPP_ISERR(error))
			goto return_error;
		if (tpp_unicode_isspace_nolf(uc)) {
			pos = npos;
			goto again;
		}
	}
#endif /* TPP_HAVE_UNICODE */

done:
	*p_pos = pos;
	return TPP_EOK;
return_error:
	return error;
}


#if TPP_HAVE_BSE
/* Check if **p_pos is the \-character of an escaped line-feed, and if BSE is enabled.
 * If both are the case, skip over said escaped linefeed, possibly expanding the
 * currently loaded text-chunk, before returning a pointer past said BSE. (if the
 * BSE is followed by further BSE, those are all skipped also, meaning that the
 * returned pointer is either EOF or something other than the start of a BSE)
 *
 * When BSE isn't enabled, or the \-character doesn't escape a line-feed, don't alter `*p_pos'
 */
static tpp_errno TPPCALL
tpp_lexer_skip_bse(tpp_lexer *self, tpp_char const **p_pos) {
	tpp_errno error;
	tpp_char ch;
	tpp_char const *scan = *p_pos;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_size rel_pos;
#if TPP_HAVE_TRIGRAPHS && TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
	bool is_trigraph;
#endif /* TPP_HAVE_TRIGRAPHS && TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
	if (!tpp_lexer_has(self, BSE))
		return TPP_EOK;

again_scan:
	rel_pos = tpp_file_ptr2rel(file, scan);
	tpp_assert(scan < file->tf_end);
#if TPP_HAVE_TRIGRAPHS
	tpp_assert(*scan == '\\' || *scan == '?');
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
	is_trigraph = false;
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
	++scan;
	if (scan[-1] == '?') {
		scan += 2; /* ??/ */
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
		is_trigraph = true;
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
	}
#else /* TPP_HAVE_TRIGRAPHS */
	tpp_assert(*scan == '\\');
	++scan;
#endif /* !TPP_HAVE_TRIGRAPHS */

#if TPP_HAVE_BSE_WHITESPACE
	if (tpp_lexer_has(self, BSE_WHITESPACE)) {
		error = tpp_lexer_skipspace_nolf(self, &scan);
		if (TPP_ISERR(error))
			goto return_error;
	}
#endif /* TPP_HAVE_BSE_WHITESPACE */
	if tpp_unlikely(scan >= file->tf_end) {
		/* Must extend file! */
		tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
		error = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			goto return_error;
		scan = tpp_file_rel2ptr(file, rel_scan);
		if tpp_unlikely(scan >= file->tf_end)
			goto return_noop;
	}

	/* Now check if "scan" points at a line-feed */
	ch = *scan;
	if (tpp_ascii_islf(ch)) {
		++scan;
#if TPP_HAVE_CR_LF_DETECTION
		if (ch == TPP_ASCII_CR) {
			if tpp_unlikely(scan >= file->tf_end) {
				/* Must extend file! */
				tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
				error = tpp_file_expandchunk(file);
				if (TPP_ISERR(error))
					goto return_error;
				scan = tpp_file_rel2ptr(file, rel_scan);
			}
			if (scan < file->tf_end && *scan == TPP_ASCII_LF)
				++scan;
		}
#endif /* TPP_HAVE_CR_LF_DETECTION */
#if TPP_HAVE_UNICODE
got_bse_after_linefeed:
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TRIGRAPHS && TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
		if (is_trigraph) {
			error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_pos),
			                           TPP_W_ENCOUNTERED_TRIGRAPH);
			if (TPP_ISERR(error))
				goto return_error;
		}
#endif /* TPP_HAVE_TRIGRAPHS && TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */

		/* Check for further BSE */
		if tpp_unlikely(scan >= file->tf_end) {
			/* Must extend file! */
			tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
			error = tpp_file_expandchunk(file);
			if (TPP_ISERR(error))
				goto return_error;
			scan = tpp_file_rel2ptr(file, rel_scan);
		}
		if (scan < file->tf_end) {
			if (*scan == '\\')
				goto again_scan;
#if TPP_HAVE_TRIGRAPHS
			if (*scan == '?' && tpp_lexer_has(self, TRIGRAPHS)) {
				if ((scan + 1) >= file->tf_end) {
					tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
					error = tpp_file_expandchunk(file);
					if (TPP_ISERR(error))
						goto return_error;
					scan = tpp_file_rel2ptr(file, rel_scan);
				}
				if (scan[1] == '?') {
					if ((scan + 2) >= file->tf_end) {
						tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
						error = tpp_file_expandchunk(file);
						if (TPP_ISERR(error))
							goto return_error;
						scan = tpp_file_rel2ptr(file, rel_scan);
					}
					if (scan[2] == '/')
						goto again_scan;
				}
			}
#endif /* TPP_HAVE_TRIGRAPHS */
		}

		/* Return updated pointer */
		*p_pos = scan;
		return TPP_EOK;
	}
#if TPP_HAVE_UNICODE
	if (tpp_ascii_ismb(ch) && tpp_file_isutf8(file)) {
		tpp_unichar uc;
		tpp_char const *nscan = scan;
		error = tpp_lexer_readutf8(self, &nscan, &uc);
		if (TPP_ISERR(error))
			goto return_error;
		if (tpp_unicode_islf(uc)) {
			scan = nscan;
			goto got_bse_after_linefeed;
		}
	}
#endif /* TPP_HAVE_UNICODE */
return_noop:
	*p_pos = tpp_file_rel2ptr(file, rel_pos);
	return TPP_EOK;
return_error:
	return error;
}
#endif /* TPP_HAVE_BSE */

/* Given a "*p_pos" that points past the initial "issymstrt" character,
 * seek (and potentially extend the lexer's current file) until the
 * first non-issymcont character is found, then return a pointer to
 * said non-issymcont character
 *
 * NOTE: This function does NOT handle BSE!
 */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_keyword(tpp_lexer *self, tpp_char const **p_pos) {
	tpp_errno error;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
	tpp_char const *end = file->tf_end;
	tpp_char ch;
again:
	if (pos >= end) {
		/* Must extend the file */
		tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
		error = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			goto return_error;
		pos = tpp_file_rel2ptr(file, rel_pos);
		end = file->tf_end;
		if (pos >= end)
			goto done;
	}

	ch = *pos;
	if (tpp_ascii_issymcont(ch)) {
#if TPP_HAVE_TPP_TOK_DOLLAR
		if (ch == '$' && tpp_lexer_has(self, TPP_TOK_DOLLAR))
			goto done;
#endif /* TPP_HAVE_TPP_TOK_DOLLAR */
		++pos;
		goto again;
	}

#if TPP_HAVE_UNICODE
	if (tpp_ascii_ismb(ch) && tpp_file_isutf8(file)) {
		tpp_unichar uc;
		tpp_char const *npos = pos;
		error = tpp_lexer_readutf8(self, &npos, &uc);
		if (TPP_ISERR(error))
			goto return_error;
		if (tpp_unicode_issymcont(uc)) {
			pos = npos;
			goto again;
		}
	}
#endif /* TPP_HAVE_UNICODE */

done:
	*p_pos = pos;
	return TPP_EOK;
return_error:
	return error;
}

#undef NEED_tpp_lexer_seek_eol
#if (TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT || \
     TPP_HAVE_TPP_TOK_AT_AT_COMMENT ||       \
     TPP_HAVE_TPP_TOK_SLASH_COMMENT ||       \
     TPP_HAVE_TPP_TOK_POUND_COMMENT ||       \
     TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT)
#define NEED_tpp_lexer_seek_eol 1
#else /* ... */
#define NEED_tpp_lexer_seek_eol 0
#endif /* !... */

#undef NEED_tpp_lexer_seek_end_of_string
#if (TPP_HAVE_TPP_TOK_C_STRING ||                 \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_C_CHAR ||                   \
     TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL)
#define NEED_tpp_lexer_seek_end_of_string 1
#else /* ... */
#define NEED_tpp_lexer_seek_end_of_string 0
#endif /* !... */

#undef NEED_tpp_lexer_seek_end_of_block_string
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
#define NEED_tpp_lexer_seek_end_of_block_string 1
#else /* ... */
#define NEED_tpp_lexer_seek_end_of_block_string 0
#endif /* !... */

#undef NEED_tpp_lexer_seek_end_of_cxx_raw_string
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
#define NEED_tpp_lexer_seek_end_of_cxx_raw_string 1
#else /* ... */
#define NEED_tpp_lexer_seek_end_of_cxx_raw_string 0
#endif /* !... */

#undef NEED_tpp_lexer_seek_end_of_raw_string
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
#define NEED_tpp_lexer_seek_end_of_raw_string 1
#else /* ... */
#define NEED_tpp_lexer_seek_end_of_raw_string 0
#endif /* !... */


/* Read a single character (byte) whilst accounting for BSE (if enabled)
 * and automatically extending the current file if EOF is reached.
 * On true EOF:
 * - *p_result = '\0'
 * - *p_pos = tpp_lexer_getfile(self)->tf_end
 * - return TPP_EOK;
 *
 * @return: TPP_EOK:    Character was read
 * @return: TPP_ENOMEM: Out of memory
 * @return: TPP_EIO:    Failed to read from underlying file */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_readchar(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos,
                   tpp_char *tpp_restrict p_result) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
	tpp_char const *end = file->tf_end;
	tpp_char ch;
#if TPP_HAVE_BSE
continue_at_pos:
#endif /* TPP_HAVE_BSE */
	tpp_assert(pos <= end);
	if tpp_unlikely(pos >= end) {
		/* Must extend the current chunk */
		tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
		tpp_size rel_end = tpp_file_ptr2rel(file, end);
		tpp_errno error  = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			return error;
		end = tpp_file_rel2ptr(file, rel_end);
		tpp_assert(end <= file->tf_end);
		pos = tpp_file_rel2ptr(file, rel_pos);
		if tpp_unlikely(end >= file->tf_end) {
			/* EOF */
			*p_pos    = pos;
			*p_result = '\0';
			return TPP_EOK;
		}
		end = file->tf_end;
	}
	tpp_assert(pos < end);
	ch = *pos;
#if TPP_HAVE_BSE
	if (ch == '\\') {
		/* Deal with BSE sequences... */
		tpp_size rel_after;
		tpp_size rel_before = tpp_file_ptr2rel(file, pos);
		tpp_errno error = tpp_lexer_skip_bse(self, &pos);
		if (TPP_ISERR(error))
			return error;
		rel_after = tpp_file_ptr2rel(file, pos);
		tpp_assert(rel_before <= rel_after);
		if (rel_before < rel_after) {
			end = file->tf_end;
			goto continue_at_pos;
		}
	}
#endif /* TPP_HAVE_BSE */
	++pos;
#if TPP_HAVE_TRIGRAPHS
	/* Support for character aliases:
	 * "??=" -> "#"
	 * "??(" -> "["
	 * "??/" -> "\"
	 * "??)" -> "]"
	 * "??'" -> "^"
	 * "??<" -> "{"
	 * "??!" -> "|"
	 * "??>" -> "}"
	 * "??-" -> "~"
	 * "???" -> "?" */
	if (ch == '?' && tpp_lexer_has(self, TRIGRAPHS)) {
		if (pos >= end) {
			tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
			tpp_errno error  = tpp_file_expandchunk(file);
			if (TPP_ISERR(error))
				return error;
			pos = tpp_file_rel2ptr(file, rel_pos);
			end = file->tf_end;
		}
		if (pos < end && *pos == '?') {
			++pos;
			if (pos >= end) {
				tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
				tpp_errno error  = tpp_file_expandchunk(file);
				if (TPP_ISERR(error))
					return error;
				pos = tpp_file_rel2ptr(file, rel_pos);
				end = file->tf_end;
			}
			if (pos >= end) {
not_a_trigraph:
				--pos;
			} else {
				switch (*pos) {
				case '=': ch = '#'; break;
				case '(': ch = '['; break;
				case ')': ch = ']'; break;
				case '\'': ch = '^'; break;
				case '<': ch = '{'; break;
				case '!': ch = '|'; break;
				case '>': ch = '}'; break;
				case '-': ch = '~'; break;
				case '?': ch = '?'; break;
				case '/':
					ch = '\\';
#if TPP_HAVE_BSE
					/* Deal with BSE sequences... */
					if (tpp_lexer_has(self, BSE)) {
						tpp_size rel_after;
						tpp_size rel_before;
						tpp_errno error;
						pos -= 2;
						rel_before = tpp_file_ptr2rel(file, pos);
						error = tpp_lexer_skip_bse(self, &pos);
						if (TPP_ISERR(error))
							return error;
						rel_after = tpp_file_ptr2rel(file, pos);
						tpp_assert(rel_before <= rel_after);
						if (rel_before < rel_after) {
							end = file->tf_end;
							goto continue_at_pos;
						}
						pos = tpp_file_rel2ptr(file, rel_before) + 1;
					}
#endif /* TPP_HAVE_BSE */
					break;
				default: goto not_a_trigraph;
				}
				++pos;
			}
		}
	}
#endif /* TPP_HAVE_TRIGRAPHS */

	*p_pos = pos;
	*p_result = ch;
	return TPP_EOK;
}


#if TPP_HAVE_UNICODE
/* Same as `tpp_lexer_readchar()', but (if the current file's encoding allows
 * it, and IN(*p_pos) points at a multi-byte character), decode a multi-byte
 * character and return it. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_readunichar(tpp_lexer *tpp_restrict self,
                      tpp_char const **tpp_restrict p_pos,
                      tpp_unichar *tpp_restrict p_result) {
	tpp_char ch;
	tpp_errno error = tpp_lexer_readchar(self, p_pos, &ch);
	if (!TPP_ISERR(error) && tpp_ascii_ismb(ch)) {
		tpp_file const *const file = tpp_lexer_getfile(self);
		if (tpp_file_isutf8(file)) {
			--(*p_pos);
			return tpp_lexer_readutf8(self, p_pos, p_result);
		}
		ch &= 0x7f; /* ??? What else could be done here? */
	}
	*p_result = (tpp_unichar)ch;
	return error;
}
#endif /* TPP_HAVE_UNICODE */



#if (NEED_tpp_lexer_seek_eol || (TPP_HAVE_CPP_ERROR ||             \
                                 TPP_HAVE_CPP_WARNING ||           \
                                 TPP_HAVE_TPP_TOK_POUND_COMMENT || \
                                 TPP_HAVE_CPP_EMBED ||             \
                                 TPP_HAVE_CPP_DIGIT_LINE))
#undef tpp_lexer_seek_eol__STYLE_PARAM
#undef tpp_lexer_seek_eol__STYLE_ARG
#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
#define tpp_lexer_seek_eol__STYLE_PARAM  , tpp_token_id comment_style
#define tpp_lexer_seek_eol__STYLE_ARG(x) , x
#else /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */
#define tpp_lexer_seek_eol__STYLE_PARAM  /* nothing */
#define tpp_lexer_seek_eol__STYLE_ARG(x) /* nothing */
#endif /* !TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */

/* Seek forward until *after* the next line-feed character (or true EOF)
 * Given `*p_pos' will be updated to point *after* the LF character (or *at* the EOF) */
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TPP_TOK_POUND_COMMENT || TPP_HAVE_CPP_EMBED || TPP_HAVE_CPP_DIGIT_LINE
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_eol(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos
                   tpp_lexer_seek_eol__STYLE_PARAM);
TPP_INTERN_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_eol(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos
                   tpp_lexer_seek_eol__STYLE_PARAM)
#else /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TPP_TOK_POUND_COMMENT || TPP_HAVE_CPP_EMBED || TPP_HAVE_CPP_DIGIT_LINE */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_eol(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos
                   tpp_lexer_seek_eol__STYLE_PARAM)
#endif /* !TPP_HAVE_CPP_ERROR && !TPP_HAVE_CPP_WARNING && !TPP_HAVE_TPP_TOK_POUND_COMMENT && !TPP_HAVE_CPP_EMBED && !TPP_HAVE_CPP_DIGIT_LINE */
{
	tpp_errno error = TPP_EOK;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
	tpp_size rel_after_bse, rel_before_bse;
#endif /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */
	tpp_char ch;
again:
	if (pos >= file->tf_end) {
		tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
		error = tpp_file_expandchunk(file);
		pos = tpp_file_rel2ptr(file, rel_pos);
		if (TPP_ISERR(error))
			goto done;
		if (pos >= file->tf_end)
			goto done; /* True EOF */
	}
	ch = *pos++;
	if (tpp_ascii_islf(ch)) {
#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
handle_ascii_lf:
#endif /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */
#if TPP_HAVE_CR_LF_DETECTION
		if (ch == TPP_ASCII_CR) {
			if (pos >= file->tf_end) {
				tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
				error = tpp_file_expandchunk(file);
				pos = tpp_file_rel2ptr(file, rel_pos);
				if (TPP_ISERR(error))
					goto done;
			}
			if (pos < file->tf_end && *pos == TPP_ASCII_LF)
				++pos;
		}
#endif /* TPP_HAVE_CR_LF_DETECTION */
		goto done;
	} else
#if TPP_HAVE_UNICODE
	if (tpp_ascii_ismb(ch) && tpp_file_isutf8(file)) {
		tpp_unichar uc;
		--pos;
		error = tpp_lexer_readutf8(self, &pos, &uc);
		if (TPP_ISERR(error))
			goto done;
		if (tpp_unicode_islf(uc))
			goto done;
		goto again;
	} else
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
handle_non_lf_ch:
#endif /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */

#if TPP_HAVE_BSE
	if (ch == '\\') {
		/* Deal with BSE sequences... */
#if TPP_HAVE_TRIGRAPHS
handle_backslash:
#endif /* TPP_HAVE_TRIGRAPHS */
		--pos;
		rel_before_bse = tpp_file_ptr2rel(file, pos);
		error = tpp_lexer_skip_bse(self, &pos);
		if (TPP_ISERR(error))
			goto done;
		rel_after_bse = tpp_file_ptr2rel(file, pos);
		tpp_assert(rel_before_bse <= rel_after_bse);
		if (rel_before_bse >= rel_after_bse) {
#if TPP_HAVE_TRIGRAPHS
			if (ch != '\\')
				pos += 2;
#endif /* TPP_HAVE_TRIGRAPHS */
			++pos;
			goto again; /* Not a BSE sequence */
		}
#if TPP_HAVE_TRIGRAPHS && TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
		if (ch != '\\') {
			tpp_char const *trigraph_pos = tpp_file_rel2ptr(file, rel_before_bse);
			error = tpp_lexer_warnf_at(self, file, trigraph_pos, TPP_W_ENCOUNTERED_TRIGRAPH);
			if (TPP_ISERR(error))
				goto done;
		}
#endif /* TPP_HAVE_TRIGRAPHS && TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */

#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
		/* Emit warning if we don't encounter a start-of-line matching "comment_style":
		 * >> // foo \    .
		 * >>    bar      << Warn here
		 *
		 * >> // foo \    .
		 * >> // bar      << Do not warn here */
		if (comment_style != TPP_TOK_EOF) {
#if TPP_HAVE_CPP_DIRECTIVES && TPP_HAVE_TPP_TOK_POUND_COMMENT
			if (comment_style == TPP_TOK_POUND_COMMENT) {
				/* Special case: don't warn about line-continuation in #-comments
				 *               when CPP directives are enabled. Else, we'd be warning
				 *               about every #define or similar that uses a trailing \ */
				if (tpp_lexer_has(self, CPP_DIRECTIVES))
					goto again;
			}
#endif /* TPP_HAVE_CPP_DIRECTIVES && TPP_HAVE_TPP_TOK_POUND_COMMENT */
			for (;;) {
				if (pos >= file->tf_end) {
					tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
					error = tpp_file_expandchunk(file);
					pos = tpp_file_rel2ptr(file, rel_pos);
					if (TPP_ISERR(error))
						goto done;
					if (pos >= file->tf_end)
						goto done; /* True EOF */
				}
				ch = *pos++;
				if (tpp_ascii_islf(ch)) {
					error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_before_bse),
					                           TPP_W_LINE_COMMENT_CONTINUED);
					if (TPP_ISERR(error))
						goto done;
					goto handle_ascii_lf;
				} else if (tpp_ascii_isspace(ch)) {
					continue;
				} else
#if TPP_HAVE_UNICODE
				if (tpp_ascii_ismb(ch) && tpp_file_isutf8(file)) {
					tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
					tpp_unichar uc;
					--pos;
					error = tpp_lexer_readutf8(self, &pos, &uc);
					if (TPP_ISERR(error))
						goto done;
					if (tpp_unicode_islf(uc)) {
						error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_before_bse),
						                           TPP_W_LINE_COMMENT_CONTINUED);
						goto done;
					}
					pos = tpp_file_rel2ptr(file, rel_pos);
				} else
#endif /* TPP_HAVE_UNICODE */
				{
				}

				switch (comment_style) {
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT
				case TPP_TOK_SLASH_SLASH_COMMENT:
					if (ch == '/') {
						if (pos >= file->tf_end) {
							tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
							error = tpp_file_expandchunk(file);
							pos = tpp_file_rel2ptr(file, rel_pos);
							if (TPP_ISERR(error))
								goto done;
							if (pos >= file->tf_end)
								break;
						}
						if (pos[0] == '/') {
							++pos;
							goto again;
						}
					}
					break;
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT */
#if TPP_HAVE_TPP_TOK_POUND_COMMENT && TPP_CONF_MAYBE_0(TPP_HAVE_CPP_DIRECTIVES)
				case TPP_TOK_POUND_COMMENT:
					if (ch == '#')
						goto again;
#if TPP_HAVE_TRIGRAPHS
					if (ch == '?' && tpp_lexer_has(self, TRIGRAPHS)) {
						if (pos >= file->tf_end) {
							tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
							error = tpp_file_expandchunk(file);
							pos = tpp_file_rel2ptr(file, rel_pos);
							if (TPP_ISERR(error))
								goto done;
							if (pos >= file->tf_end)
								break;
						}
						if (pos[0] == '?') {
							if ((pos + 1) >= file->tf_end) {
								tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
								error = tpp_file_expandchunk(file);
								pos = tpp_file_rel2ptr(file, rel_pos);
								if (TPP_ISERR(error))
									goto done;
								if ((pos + 1) >= file->tf_end)
									break;
							}
							if (pos[1] == '=') {
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
								error = tpp_lexer_warnf_at(self, file, pos - 1, TPP_W_ENCOUNTERED_TRIGRAPH);
								if (TPP_ISERR(error))
									goto done;
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
								pos += 2;
								goto again;
							}
						}
					}
#endif /* TPP_HAVE_TRIGRAPHS */
					break;
#endif /* TPP_HAVE_TPP_TOK_POUND_COMMENT && TPP_CONF_MAYBE_0(TPP_HAVE_CPP_DIRECTIVES) */
#if TPP_HAVE_TPP_TOK_SLASH_COMMENT
				case TPP_TOK_SLASH_COMMENT:
					if (ch == '/')
						goto again;
					break;
#endif /* TPP_HAVE_TPP_TOK_SLASH_COMMENT */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT
				case TPP_TOK_MINUS_MINUS_COMMENT:
					if (ch == '-') {
						if (pos >= file->tf_end) {
							tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
							error = tpp_file_expandchunk(file);
							pos = tpp_file_rel2ptr(file, rel_pos);
							if (TPP_ISERR(error))
								goto done;
						}
						if (pos[0] == '-') {
							++pos;
							goto again;
						}
					}
					break;
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT */
#if TPP_HAVE_TPP_TOK_AT_AT_COMMENT
				case TPP_TOK_AT_AT_COMMENT:
					if (ch == '@') {
						if (pos >= file->tf_end) {
							tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
							error = tpp_file_expandchunk(file);
							pos = tpp_file_rel2ptr(file, rel_pos);
							if (TPP_ISERR(error))
								goto done;
						}
						if (pos[0] == '@') {
							++pos;
							goto again;
						}
					}
					break;
#endif /* TPP_HAVE_TPP_TOK_AT_AT_COMMENT */
				default: tpp_unreachable();
				}
				error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_before_bse),
				                           TPP_W_LINE_COMMENT_CONTINUED);
				if (TPP_ISERR(error))
					goto done;
				goto handle_non_lf_ch;
			}
		}
#endif /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */
		goto again;
	} else
#if TPP_HAVE_TRIGRAPHS
	if (ch == '?' && tpp_lexer_has(self, TRIGRAPHS)) {
		if (pos >= file->tf_end) {
			tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
			error = tpp_file_expandchunk(file);
			pos = tpp_file_rel2ptr(file, rel_pos);
			if (TPP_ISERR(error))
				goto done;
			if (pos >= file->tf_end)
				goto done; /* True EOF */
		}
		if (pos[0] != '?')
			goto again;
		if ((pos + 1) >= file->tf_end) {
			tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
			error = tpp_file_expandchunk(file);
			pos = tpp_file_rel2ptr(file, rel_pos);
			if (TPP_ISERR(error))
				goto done;
			if ((pos + 1) >= file->tf_end)
				goto again;
		}
		if (pos[1] != '/')
			goto again;
		goto handle_backslash;
	} else
#endif /* TPP_HAVE_TRIGRAPHS */
#endif /* TPP_HAVE_BSE */
	{
		goto again;
	}

done:
	*p_pos = pos;
	return error;
}
#endif /* NEED_tpp_lexer_seek_eol || ... */


/* Find the end of a "foo" or 'foo' string
 *      IN(*p_pos) == ^   ^ == OUT(*p_pos) */
#if NEED_tpp_lexer_seek_end_of_string
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_string(tpp_lexer *tpp_restrict self,
                             tpp_char const **tpp_restrict p_pos,
                             tpp_char quote_char) {
	tpp_file *const file = tpp_lexer_getfile(self);
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF || TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
	tpp_size rel_start = tpp_file_ptr2rel(file, *p_pos);
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF || TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
	for (;;) {
		tpp_char ch;
		tpp_errno error;
#if TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ALLOW_MULTILINE)
		tpp_size old_pos = tpp_file_ptr2rel(file, *p_pos);
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ALLOW_MULTILINE) */
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch == quote_char)
			break;
		if (ch == 0 && (*p_pos) >= file->tf_end)
			goto warn_premature_eof;
		if (ch == '\\') {
			error = tpp_lexer_readchar(self, p_pos, &ch);
			if (TPP_ISERR(error))
				return error;
			if (ch == 0 && (*p_pos) >= file->tf_end)
				goto warn_premature_eof;
		} else
#if TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ALLOW_MULTILINE)
		if (tpp_ascii_islf(ch)) {
#if TPP_HAVE_UNICODE
handle_linefeed:
#endif /* TPP_HAVE_UNICODE */
			if (!tpp_lexer_has(self, STRING_ALLOW_MULTILINE)) {
				*p_pos = tpp_file_rel2ptr(file, old_pos);
				/* Warning if a line-feed is encountered */
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
				return tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_start),
				                          TPP_W_STRING_TERMINATED_BY_LINEFEED);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
				break;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
			} else {
#if TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED
				error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, old_pos),
				                           TPP_W_STRING_CONTINUED_AFTER_LINEFEED);
				if (TPP_ISERR(error))
					return error;
#endif /* TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED */
			}
		} else
#if TPP_HAVE_UNICODE
		if (tpp_ascii_ismb(ch) && tpp_file_isutf8(file)) {
			/* Check for unicode linefeed */
			tpp_unichar uc;
			*p_pos = tpp_file_rel2ptr(file, old_pos);
			error  = tpp_lexer_readutf8(self, p_pos, &uc);
			if (TPP_ISERR(error))
				return error;
			if (tpp_unicode_islf(uc))
				goto handle_linefeed;
		} else
#endif /* TPP_HAVE_UNICODE */
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ALLOW_MULTILINE) */
		{
		}
	}
	return TPP_EOK;
warn_premature_eof:
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
	return tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_start),
	                          TPP_W_STRING_TERMINATED_BY_EOF);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
}
#endif /* NEED_tpp_lexer_seek_end_of_string */


/* Find the end of a """foo""" or '''foo''' string
 *        IN(*p_pos) == ^     ^ == OUT(*p_pos) */
#if NEED_tpp_lexer_seek_end_of_block_string
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_block_string(tpp_lexer *tpp_restrict self,
                                   tpp_char const **tpp_restrict p_pos,
                                   tpp_char quote_char) {
	/* NOTE: """triple quote \""" and string continues"""
	 * iow: \-escape sequences still exist in block-strings! */
	tpp_file *const file = tpp_lexer_getfile(self);
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
	tpp_size const rel_start = tpp_file_ptr2rel(file, *p_pos);
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
	for (;;) {
		tpp_char ch;
		tpp_errno error;
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch == quote_char) {
			error = tpp_lexer_readchar(self, p_pos, &ch);
			if (TPP_ISERR(error))
				return error;
			if (ch == quote_char) {
				error = tpp_lexer_readchar(self, p_pos, &ch);
				if (TPP_ISERR(error))
					return error;
				if (ch == quote_char)
					break; /* triple quote_char -> end block-string */
			}
		}
		if (ch == 0 && (*p_pos) >= file->tf_end)
			goto warn_premature_eof;
		if (ch == '\\') {
			error = tpp_lexer_readchar(self, p_pos, &ch);
			if (TPP_ISERR(error))
				return error;
			if (ch == 0 && (*p_pos) >= file->tf_end)
				goto warn_premature_eof;
		}
	}
	return TPP_EOK;
warn_premature_eof:
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
	return tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_start),
	                          TPP_W_STRING_TERMINATED_BY_EOF);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
}
#endif /* NEED_tpp_lexer_seek_end_of_block_string */


/* Find the end of a R"AB(foo)AB" string
 *       IN(*p_pos) == ^         ^ == OUT(*p_pos) */
#if NEED_tpp_lexer_seek_end_of_cxx_raw_string
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_cxx_raw_string(tpp_lexer *tpp_restrict self,
                                     tpp_char const **tpp_restrict p_pos,
                                     tpp_char quote_ch) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_size rel_pattern_start = tpp_file_ptr2rel(file, *p_pos);
	tpp_size rel_pattern_end, delim_len;
	tpp_char ch;
	tpp_errno error;

	/* Find end of pattern string */
	delim_len = 0;
	for (;;) {
		rel_pattern_end = tpp_file_ptr2rel(file, *p_pos);
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch == '(')
			break;
		if (tpp_ascii_islf(ch)) {
			/* TODO: Warning if a line-feed is encountered */
		}
		if (delim_len == 16) {
			/* TODO: Warning if raw string delimiter longer than 16 characters */
		}

		++delim_len;
		if (ch == 0 && (*p_pos) >= file->tf_end)
			goto warn_premature_eof;
	}

	/* Scan string body... */
	for (;;) {
		tpp_size rel_pattern_iter;
		tpp_char const *pos2;
		tpp_size rel_continue;
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch != ')') {
			if (ch == 0 && (*p_pos) >= file->tf_end)
				goto warn_premature_eof;
			continue;
		}
		rel_continue = tpp_file_ptr2rel(file, *p_pos); /* Rewind to here on pattern missmatch */
		for (rel_pattern_iter = rel_pattern_start;
		     rel_pattern_iter < rel_pattern_end;) {
			tpp_char pattern_ch;
			tpp_size rel_pos;

			/* Read pattern character */
			rel_pos = tpp_file_ptr2rel(file, *p_pos);
			pos2    = tpp_file_rel2ptr(file, rel_pattern_iter);
			error   = tpp_lexer_readchar(self, &pos2, &pattern_ch);
			rel_pattern_iter = tpp_file_ptr2rel(file, pos2);
			(*p_pos) = tpp_file_rel2ptr(file, rel_pos);
			if (TPP_ISERR(error))
				return error; /* Shouldn't cause errors (was already scanned before) */

			/* Read input character */
			error = tpp_lexer_readchar(self, p_pos, &ch);
			if (TPP_ISERR(error))
				return error;
			if (pattern_ch != ch)
				goto continue_string;
		}

		/* Entire pattern was matched -> next character must be '"' */
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch == quote_ch)
			break;
continue_string:
		*p_pos = tpp_file_rel2ptr(file, rel_continue);
	}
	return TPP_EOK;
warn_premature_eof:
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
	return tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_pattern_start),
	                          TPP_W_STRING_TERMINATED_BY_EOF);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
}
#endif /* NEED_tpp_lexer_seek_end_of_cxx_raw_string */


/* Find the end of a R"foo" or R'foo' string
 *       IN(*p_pos) == ^   ^ == OUT(*p_pos) */
#if NEED_tpp_lexer_seek_end_of_raw_string
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_raw_string(tpp_lexer *tpp_restrict self,
                                 tpp_char const **tpp_restrict p_pos,
                                 tpp_char quote_char) {
	tpp_file *const file = tpp_lexer_getfile(self);
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF || TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
	tpp_size rel_start = tpp_file_ptr2rel(file, *p_pos);
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF || TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
	for (;;) {
		tpp_char ch;
		tpp_errno error;
#if TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ALLOW_MULTILINE)
		tpp_size old_pos = tpp_file_ptr2rel(file, *p_pos);
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ALLOW_MULTILINE) */
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch == quote_char)
			break;
		if (ch == 0 && (*p_pos) >= file->tf_end)
			goto warn_premature_eof;
#if TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ALLOW_MULTILINE)
		if (tpp_ascii_islf(ch)) {
#if TPP_HAVE_UNICODE
handle_linefeed:
#endif /* TPP_HAVE_UNICODE */
			if (!tpp_lexer_has(self, STRING_ALLOW_MULTILINE)) {
				*p_pos = tpp_file_rel2ptr(file, old_pos);
				/* Warning if a line-feed is encountered */
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
				return tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_start),
				                          TPP_W_STRING_TERMINATED_BY_LINEFEED);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
				break;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
			} else {
#if TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED
				error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, old_pos),
				                           TPP_W_STRING_CONTINUED_AFTER_LINEFEED);
				if (TPP_ISERR(error))
					return error;
#endif /* TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED */
			}
		} else
#if TPP_HAVE_UNICODE
		if (tpp_ascii_ismb(ch) && tpp_file_isutf8(file)) {
			/* Check for unicode linefeed */
			tpp_unichar uc;
			*p_pos = tpp_file_rel2ptr(file, old_pos);
			error  = tpp_lexer_readutf8(self, p_pos, &uc);
			if (TPP_ISERR(error))
				return error;
			if (tpp_unicode_islf(uc))
				goto handle_linefeed;
		} else
#endif /* TPP_HAVE_UNICODE */
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ALLOW_MULTILINE) */
		{
		}
	}
	return TPP_EOK;
warn_premature_eof:
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
	return tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_start),
	                          TPP_W_STRING_TERMINATED_BY_EOF);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
}
#endif /* NEED_tpp_lexer_seek_end_of_raw_string */


#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS
/* Seek end of unichar: foo\U12345678XY
 *                         ^=in      ^out */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_skip_bsi(tpp_lexer *tpp_restrict self, tpp_char const **p_pos) {
	/* C says that (implementations can threat) this:
	 * >> char const *\U0001f431 = "cat";
	 *
	 * as a valid identifier. -- We should support that (*and* interpret
	 * it as "\xF0\x9F\x90\xB1" (its utf-8 repr) during keyword lookup)
	 *
	 * For this purpose, the "*_bse" version of keyword lookup functions
	 * should also have another extension that lets them treat \u and \U
	 * sequences specially! */
	tpp_errno error = TPP_EOK;
	tpp_char const *scan = *p_pos;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_size rel_start = tpp_file_ptr2rel(file, scan);
	tpp_assert(scan < file->tf_end);
#if TPP_HAVE_TRIGRAPHS
	tpp_assert(*scan == '\\' || *scan == '?');
	++scan;
	if (scan[-1] == '?')
		scan += 2; /* ??/ */
#else /* TPP_HAVE_TRIGRAPHS */
	tpp_assert(*scan == '\\');
	++scan;
#endif /* !TPP_HAVE_TRIGRAPHS */
	if (scan >= file->tf_end) {
		tpp_size rel_pos = tpp_file_ptr2rel(file, scan);
		error = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			goto done;
		scan = tpp_file_rel2ptr(file, rel_pos);
	}
	if (*scan == 'u' || *scan == 'U') {
		/* No BSE allowed in here -- \u \U happens at the same time,
		 * and I don't want to allow one escape escaping another escape.
		 *
		 * NOTE: If this ever needs to be added, would also need to add
		 *       support in "tpp_decode_bsi()" (our partner function)
		 *
		 * TODO: This is incorrect -- the C standard allows BSE inside
		 *       \u (and \U)-characters keywords since C++23:
		 * https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2621r2.pdf
		 */
		unsigned int cur_digit = 0;
		unsigned int max_digit = *scan == 'U' ? 8 : 4;
		do {
			tpp_char nibble_ch;
			if ((scan + cur_digit + 1) >= file->tf_end) {
				tpp_size rel_pos = tpp_file_ptr2rel(file, scan);
				error = tpp_file_expandchunk(file);
				if (TPP_ISERR(error))
					goto done;
				scan = tpp_file_rel2ptr(file, rel_pos);
				if ((scan + cur_digit + 1) >= file->tf_end)
					break;
			}
			nibble_ch = scan[cur_digit + 1];
			if (tpp_ascii_isdigit(nibble_ch)) {
				/* ... */
			} else if (tpp_ascii_islwrxdigit(nibble_ch)) {
				/* ... */
			} else if (tpp_ascii_isuprxdigit(nibble_ch)) {
				/* ... */
			} else {
				break;
			}
			++cur_digit;
		} while (cur_digit < max_digit);
		if (cur_digit == 0)
			goto done;
		scan += 1;
		scan += cur_digit;
		*p_pos = scan;
		return TPP_EOK;
	}
done:
	*p_pos = tpp_file_rel2ptr(file, rel_start);
	return error;
}
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */


#if TPP_HAVE_TPP_TOK_FLOAT
/* Seek the end of a TPP_TOK_FLOAT (or TPP_TOK_INT) token
 * @param: result:               Token mode: TPP_TOK_FLOAT if a "." was already encountered;
 *                               else TPP_TOK_INT (or TPP_TOK_EOF if int-tokens are disabled).
 * @return: TPP_TOK_FLOAT:       Success; this is a float token
 * @return: TPP_TOK_INT:         Nothing found that would qualify as a float, so it's an int
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_FLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_seek_end_of_float(tpp_lexer *tpp_restrict self,
                            tpp_char const **p_pos,
                            tpp_token_id result) {
	tpp_errno error;
	tpp_file const *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
	tpp_size rel_end;
	tpp_char ch;
	bool allow_hex_chars = false;
#if TPP_HAVE_SMART_FLOAT_TOKENS
	bool has_exponent = false;
#endif /* TPP_HAVE_SMART_FLOAT_TOKENS */

	/* Number tokens are anything matching the regex:
	 * r"[.]?\d([\w\d_.]|[eEpP][-+])*"
	 *         ^
	 *         We get called with *p_pos already here
	 *
	 * #if TPP_HAVE_SMART_FLOAT_TOKENS
	 * NOTE: One of <<. e+ e- E+ E- p+ p- P+ P->> can only
	 *       be encountered at most once. When an additional
	 *       instance is encountered, the "." or trailing "+"
	 *       or "-" will *NOT* be part of the number token.
	 * #endif // TPP_HAVE_SMART_FLOAT_TOKENS
	 *
	 * Additionally, one of those tokens being encountered
	 * causes the resulting token to become a "TPP_TOK_FLOAT"
	 */
again:
	rel_end = tpp_file_ptr2rel(file, pos);
	error   = tpp_lexer_readchar(self, &pos, &ch);
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
again_ch:
	if (ch == '.') {
#if TPP_HAVE_SMART_FLOAT_TOKENS
		if (tpp_lexer_has(self, SMART_FLOAT_TOKENS)) {
			tpp_token_id old_result = result;
			if (result == TPP_TOK_FLOAT)
				goto done;
			result = TPP_TOK_FLOAT;

			/* Make sure that next token is one of:
			 *   [0-9]          Digit
			 *   [a-fA-F]       hex digit (only if "allow_hex_chars == true")
			 *   [eEpP][+-]     Exponent
			 *
			 * If it isn't then, the result is a TPP_TOK_INT,
			 * and ends just before the '.' character. */
			error = tpp_lexer_readchar(self, &pos, &ch);
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
			if (tpp_ascii_isdigit(ch))
				goto again;
			if ((tpp_ascii_islwrxdigit(ch) || tpp_ascii_isuprxdigit(ch)) && allow_hex_chars) {
				rel_end = tpp_file_ptr2rel(file, pos);
				goto again_ch;
			}
			if ((allow_hex_chars ? (ch == 'p' || ch == 'P')
			                     : (ch == 'e' || ch == 'E')) &&
			    !has_exponent) {
				tpp_size saved_pos = tpp_file_ptr2rel(file, pos);
				tpp_char after_exp_ch;
				error = tpp_lexer_readchar(self, &pos, &after_exp_ch);
				if (TPP_ISERR(error))
					return TPP_TOK_OFERR(error);
				if (after_exp_ch == '+' || after_exp_ch == '-' || tpp_ascii_isdigit(after_exp_ch)) {
					has_exponent = true;
					goto again; /* Exponent directly after decimal -> allowed */
				}
				pos = tpp_file_rel2ptr(file, saved_pos);
			}

			/* If enabled, allow strings that would qualify as float type suffixes here:
			 * >> 1.F;    // Must always be [FLOAT:1.F], rather than [INT:1][DOT:.][F:F]
			 *
			 * NOTE: Intentionally do this "#ifdef tpp_lexer_isfloatsuffix_char", rather
			 *       that "#if TPP_HAVE_LEXER_DECODEFLOAT_SUFFIX", so-as to allow users
			 *       to simply "#define tpp_lexer_isfloatsuffix_char" to add special
			 *       handling here, without "TPP_HAVE_LEXER_DECODEFLOAT_SUFFIX" needing
			 *       to be enabled also! */
#ifdef tpp_lexer_isfloatsuffix_char
			if (tpp_lexer_isfloatsuffix_char(self, ch))
				goto again;
#endif /* tpp_lexer_isfloatsuffix_char */

			/* End the token before the "." as whatever it was at the time. */
			result = old_result;
			goto done;
		}
#endif /* TPP_HAVE_SMART_FLOAT_TOKENS */
		result = TPP_TOK_FLOAT; /* It's a floating-point token */
		goto again;
	} else if (tpp_ascii_issymcont(ch)) { /* SYMCONT matches [\w\d_] */
#if TPP_CONF_MAYBE_0(TPP_HAVE_SMART_FLOAT_TOKENS)
		if (tpp_lexer_has(self, SMART_FLOAT_TOKENS)
		    ? (allow_hex_chars ? (ch == 'p' || ch == 'P')
		                       : (ch == 'e' || ch == 'E'))
		    : (ch == 'e' || ch == 'E' || ch == 'p' || ch == 'P'))
#elif TPP_HAVE_SMART_FLOAT_TOKENS
		if (allow_hex_chars ? (ch == 'p' || ch == 'P')
		                    : (ch == 'e' || ch == 'E'))
#else /* TPP_HAVE_SMART_FLOAT_TOKENS */
		if (ch == 'e' || ch == 'E' || ch == 'p' || ch == 'P')
#endif /* !TPP_HAVE_SMART_FLOAT_TOKENS */
		{
			tpp_char exp_ch = ch;
			rel_end = tpp_file_ptr2rel(file, pos);
			error   = tpp_lexer_readchar(self, &pos, &ch);
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
			if (ch == '+' || ch == '-') {
#if TPP_HAVE_SMART_FLOAT_TOKENS
				if (tpp_lexer_has(self, SMART_FLOAT_TOKENS)) {
					if (has_exponent)
						goto done;
					has_exponent = true;
				}
#endif /* TPP_HAVE_SMART_FLOAT_TOKENS */
				result = TPP_TOK_FLOAT;
				goto again;
			} else if (tpp_ascii_isdigit(ch)) {
				/* Special case to we detect the correct typing for:
				 * -   1E2    (TPP_TOK_FLOAT)
				 * -   1P2    (TPP_TOK_INT)
				 * - 0x1E2    (TPP_TOK_INT)
				 * - 0x1P2    (TPP_TOK_FLOAT)
				 */
				if (allow_hex_chars ? (exp_ch == 'p' || exp_ch == 'P')
				                    : (exp_ch == 'e' || exp_ch == 'E'))
					result = TPP_TOK_FLOAT;
#if TPP_HAVE_SMART_FLOAT_TOKENS
				if (tpp_lexer_has(self, SMART_FLOAT_TOKENS)) {
					if (result == TPP_TOK_FLOAT) {
						if (has_exponent)
							goto done;
						has_exponent = true;
					}
				}
#endif /* TPP_HAVE_SMART_FLOAT_TOKENS */
				goto again;
			}
			goto again_ch;
		}
		if (ch == 'x' || ch == 'X') {
#if TPP_HAVE_SMART_FLOAT_TOKENS
			if (tpp_lexer_has(self, SMART_FLOAT_TOKENS)) {
				if (result == TPP_TOK_FLOAT) {
					/* The "x" cannot appear after a construct that indicates a float.
					 * If it does anyways, it can't be used to indicate a hex-float! */
					goto again;
				}
				if (!allow_hex_chars) {
					allow_hex_chars = true;

					/* If what comes next is a hex-exponent: "0xP+12", then we mustn't
					 * parse that as a float-token (but as [0xP][+][12]). strtof requires
					 * that there be at least 1 hex-digit before a potential exponent! */
					rel_end = tpp_file_ptr2rel(file, pos);
					error   = tpp_lexer_readchar(self, &pos, &ch);
					if (TPP_ISERR(error))
						return TPP_TOK_OFERR(error);
					if (ch == 'p' || ch == 'P')
						goto again; /* Skip over character -- don't allow processing as exponent */
					goto again_ch;
				}
			} else {
				allow_hex_chars = true;
			}
#else /* TPP_HAVE_SMART_FLOAT_TOKENS */
			allow_hex_chars = true;
#endif /* !TPP_HAVE_SMART_FLOAT_TOKENS */
		}
		goto again;
	} else
#if TPP_HAVE_UNICODE
	if (tpp_ascii_ismb(ch) && tpp_file_isutf8(file)) {
		tpp_unichar uc;
		--pos;
		error = tpp_lexer_readutf8(self, &pos, &uc);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
		if (tpp_unicode_issymcont(uc))
			goto again;
	} else
#endif /* TPP_HAVE_UNICODE */
	{
	}

done:
	*p_pos = tpp_file_rel2ptr(file, rel_end);
	return result;
}
#endif /* TPP_HAVE_TPP_TOK_FLOAT */


#if TPP_HAVE_TPP_TOK_INT && TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_FLOAT)
/* Seek the end of a TPP_TOK_INT token
 * @return: TPP_EOK:         Success
 * @return: TPP_ENOMEM:      Out of memory
 * @return: TPP_EIO:         I/O error while trying to read from file
 * @return: TPP_EWOULDBLOCK: Current file uses "TPP_FILE_FLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_ELEXERROR:   Lexer error
 * @return: TPP_EWARNPRINT:  Error while printing a warning */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_int(tpp_lexer *tpp_restrict self,
                          tpp_char const **p_pos) {
	tpp_errno error;
	tpp_file const *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
	tpp_size rel_end;
	tpp_char ch;
again:
	rel_end = tpp_file_ptr2rel(file, pos);
	error   = tpp_lexer_readchar(self, &pos, &ch);
	if (TPP_ISERR(error))
		return error;
	if (tpp_ascii_issymcont(ch)) {
		goto again;
	} else
#if TPP_HAVE_UNICODE
	if (tpp_ascii_ismb(ch) && tpp_file_isutf8(file)) {
		tpp_unichar uc;
		--pos;
		error = tpp_lexer_readutf8(self, &pos, &uc);
		if (TPP_ISERR(error))
			return error;
		if (tpp_unicode_issymcont(uc))
			goto again;
	} else
#endif /* TPP_HAVE_UNICODE */
	{
	}
	*p_pos = tpp_file_rel2ptr(file, rel_end);
	return TPP_EOK;
}
#endif /* TPP_HAVE_TPP_TOK_INT && TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_FLOAT) */



/* Do a raw yield and update `self->tl_tok' in the process, then return `tl_tok.tt_id'.
 * - On EOF, automatically pop `tl_file->tf_prev' and continue reading from there
 * - On error, return one of `TPP_TOK_E*' (e.g. `TPP_TOK_EIO').
 *   Such error codes will NOT be stored in `tl_tok.tt_id'!
 *
 * NOTE: This function does *NOT* deal with:
 * - Preprocessor directives
 * - Builtin macros
 * - User-defined macros
 *
 * @return: * :                  The newly read token
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_FLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldraw(tpp_lexer *tpp_restrict self) {
	return tpp_lexer_yieldraw_at(self, &tpp_lexer_gettoken(self)->tt_end);
}




/* Same as `tpp_lexer_yieldraw()', but populate the token from a custom `*p_pos',
 * and don't pop files from the current #include-stack (unless `p_pos' is the top-
 * most file's `tf_pos')
 *
 * NOTES:
 *  - This function will *NOT* populate "tpp_lexer_gettoken(self)->tt_end",
 *    however the value it would have written there is OUT(*p_pos), meaning
 *    you can just use that instead, and call this function multiple times
 *    to yield more than 1 token
 *  - This function can be used to peek future tokens, as it will also expand
 *    the current file when `*p_pos' would go beyond its end. (in this case,
 *    `*p_pos' is updated such that it always remains valid)
 *  - Unlike `tpp_lexer_yieldraw()', this function will *not* modify the
 *    currently loaded file's `tf_pos' (unless `p_pos == &file->tf_pos'),
 *    meaning that if EOF is reached, the file's chunk will only ever be
 *    expanded, but no old data (that would appear before `tf_pos') will
 *    be deallocated
 *  - This function will also not automatically move on to the next file
 *    in line when the current one has been fully exhausted (unless the
 *    given `p_pos == &file->tf_pos'), meaning that TPP_TOK_EOF will be
 *    returned when no more data can be loaded.
 *
 * This is used to implement `tpp_lexer_yieldraw()', which simply
 * passes `p_pos = &tpp_lexer_gettoken(self)->tt_end'
 *
 * @return: * : See `tpp_lexer_yieldraw()' */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at(tpp_lexer *tpp_restrict self, tpp_char const **p_pos) {
#undef NEED_read_ch2
#if (NEED_tpp_lexer_seek_eol ||                   \
     NEED_tpp_lexer_seek_end_of_string ||         \
     NEED_tpp_lexer_seek_end_of_block_string ||   \
     NEED_tpp_lexer_seek_end_of_cxx_raw_string || \
     NEED_tpp_lexer_seek_end_of_raw_string ||     \
     TPP_HAVE_TPP_TOK_MC)
#define NEED_read_ch2 1
#else /* ... */
#define NEED_read_ch2 0
#endif /* !... */

	tpp_errno error;
	tpp_token_id result;
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char ch;
#if NEED_read_ch2
	tpp_char ch2;
#endif /* NEED_read_ch2 */
	tpp_char const *pos, *end;

	/* Relative offset from start of loaded area of file
	 * (usually `0', unless a custom "p_pos" is used) */
	tpp_size rel_start;
#if TPP_HAVE_INCLUDE_STACK || TPP_HAVE_BSE
again:
#endif /* TPP_HAVE_INCLUDE_STACK || TPP_HAVE_BSE */
	pos = *p_pos;
	end = file->tf_end;
	if tpp_unlikely(pos >= end)
		goto eof;
	rel_start = tpp_file_ptr2rel(file, pos);
again_read_from_pos:
	token->tt_start = pos;
	ch = *pos++;

	/* Primary CHARACTER -> TOKEN conversion switch */
#if TPP_HAVE_TRIGRAPHS
switch_on_ch:
#endif /* TPP_HAVE_TRIGRAPHS */
	switch (ch) {
#if NEED_read_ch2
#define read_ch2()                                    \
	do {                                              \
		error = tpp_lexer_readchar(self, &pos, &ch2); \
		if (TPP_ISERR(error))                         \
			goto return_error;                        \
	} while (0)
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
	/* Call this after 'read_ch2()' returned a
	 * (used) character that is one of: #[]^{|}~?\ */
#define warn_if_ch2_is_trigraph()                                                        \
	do {                                                                                 \
		if (pos[-1] != ch2) {                                                            \
			error = tpp_lexer_warnf_at(self, file, pos - 3, TPP_W_ENCOUNTERED_TRIGRAPH); \
			if (TPP_ISERR(error))                                                        \
				goto return_error;                                                       \
		}                                                                                \
	} while (0)
#else /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
#define warn_if_ch2_is_trigraph() (void)0
#endif /* !TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
#endif /* NEED_read_ch2 */

/************************************************************************/
	case '<': {
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE || TPP_HAVE_DIGRAPHS
		if (!tpp_lexer_has(self, DIGRAPHS) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_EXCLAIM_MINUS_MINUS_COMMENT_MINUS_MINUS_RANGLE) &&
/*[[[deemon (printHasNone from ".config")("<");]]]*/
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_MINUS) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_RANGLE)
/*[[[end]]]*/
		    )
			break;
		read_ch2();
#if TPP_HAVE_DIGRAPHS
		if (ch2 == '%') {
			if (tpp_lexer_has(self, DIGRAPHS)) {
				result = TPP_TOK_OFCHAR('{'); /* "<%" -> "{" */
				goto set_result;
			}
		} else if (ch2 == ':') {
			if (tpp_lexer_has(self, DIGRAPHS)) {
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
				read_ch2();
				if (ch2 == ':')
					break; /* "<::" -> "<", "::"  ("break" here to follow single-char path) */
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
				result = TPP_TOK_OFCHAR('['); /* "<:" -> "[" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_DIGRAPHS */
#if TPP_HAVE_TPP_TOK_LANGLE_EXCLAIM_MINUS_MINUS_COMMENT_MINUS_MINUS_RANGLE
		if (ch2 == '!') {
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_EXCLAIM_MINUS_MINUS_COMMENT_MINUS_MINUS_RANGLE)) {
				read_ch2();
				if (ch2 != '-')
					break;
				read_ch2();
				if (ch2 != '-')
					break;
				for (;;) {
					read_ch2();
continue_html_comment_with_ch2:
					if (ch2 == 0 && pos >= file->tf_end) {
#if TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF
						error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_start),
						                           TPP_W_COMMENT_TERMINATED_BY_EOF);
						if (TPP_ISERR(error))
							goto return_error;
#endif /* TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF */
						break;
					}
					if (ch2 != '-')
						continue;
					read_ch2();
					if (ch2 != '-')
						goto continue_html_comment_with_ch2;
					read_ch2();
					if (ch2 != '>')
						goto continue_html_comment_with_ch2;
					break;
				}
				result = TPP_TOK_LANGLE_EXCLAIM_MINUS_MINUS_COMMENT_MINUS_MINUS_RANGLE; /* "<!-- like this one! -->" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EXCLAIM_MINUS_MINUS_COMMENT_MINUS_MINUS_RANGLE */
/*[[[deemon (printDecoderAfterReadCh2Each from ".config")("<");]]]*/
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
		if (ch2 == '-') {
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS_LANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS_RANGLE)) {
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
				read_ch2();
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
				if (ch2 == '<') {
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS_LANGLE)) {
						result = TPP_TOK_LANGLE_MINUS_LANGLE; /* "<-<" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
				if (ch2 == '>') {
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS_RANGLE)) {
						result = TPP_TOK_LANGLE_MINUS_RANGLE; /* "<->" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
				{
				}
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
			}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS)) {
				result = TPP_TOK_LANGLE_MINUS; /* "<-" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
		} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
		if (ch2 == '<') {
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_MINUS) ||
			    tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS) ||
			    tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL) ||
			    tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_EQUAL)) {
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
				read_ch2();
				switch (ch2) {
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS
				case '-': {
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_MINUS)) {
						result = TPP_TOK_LANGLE_LANGLE_MINUS; /* "<<-" */
						goto set_result;
					}
				}	break;
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
				case '<': {
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS) ||
					    tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)) {
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
						tpp_size rel_end_of_3char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
						read_ch2();
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS
						if (ch2 == '-') {
							if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS)) {
								result = TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS; /* "<<<-" */
								goto set_result;
							}
						} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
						if (ch2 == '=') {
							if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)) {
								result = TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL; /* "<<<=" */
								goto set_result;
							}
						} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
						{
						}
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
						pos = tpp_file_rel2ptr(file, rel_end_of_3char);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
					}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE)) {
						result = TPP_TOK_LANGLE_LANGLE_LANGLE; /* "<<<" */
						goto set_result;
					}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
				}	break;
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
				case '=': {
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_EQUAL)) {
						result = TPP_TOK_LANGLE_LANGLE_EQUAL; /* "<<=" */
						goto set_result;
					}
				}	break;
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
				default: break;
				}
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
			}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE)) {
				result = TPP_TOK_LANGLE_LANGLE; /* "<<" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
		} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL || TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
		if (ch2 == '=') {
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL_LANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL_RANGLE)) {
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
				read_ch2();
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
				if (ch2 == '<') {
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL_LANGLE)) {
						result = TPP_TOK_LANGLE_EQUAL_LANGLE; /* "<=<" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
				if (ch2 == '>') {
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL_RANGLE)) {
						result = TPP_TOK_LANGLE_EQUAL_RANGLE; /* "<=>" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
				{
				}
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
			}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL)) {
				result = TPP_TOK_LANGLE_EQUAL; /* "<=" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
		} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL || TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
		if (ch2 == '>') {
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_RANGLE)) {
				result = TPP_TOK_LANGLE_RANGLE; /* "<>" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
/*[[[end]]]*/
		{
		}
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE || TPP_HAVE_DIGRAPHS */
	}	break;
/************************************************************************/



/************************************************************************/
	case '-': {
#if (TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS || TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT)
		if (!tpp_lexer_has(self, TPP_TOK_MINUS_MINUS_COMMENT) &&
/*[[[deemon (printHasNone from ".config")("-");]]]*/
		    !tpp_lexer_has(self, TPP_TOK_MINUS_MINUS) &&
		    !tpp_lexer_has(self, TPP_TOK_MINUS_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_MINUS_LANGLE_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_MINUS_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE_STAR) &&
		    !tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE)
/*[[[end]]]*/
		    )
			break;
		read_ch2();
		switch (ch2) {
#if TPP_HAVE_TPP_TOK_MINUS_MINUS || TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT
		case '-': {
#if TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT
			if (tpp_lexer_has(self, TPP_TOK_MINUS_MINUS_COMMENT)) {
				error = tpp_lexer_seek_eol(self, &pos tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_MINUS_MINUS_COMMENT));
				if (TPP_ISERR(error))
					goto return_error;
				result = TPP_TOK_MINUS_MINUS_COMMENT; // "-- like this one!"
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT */
/*[[[deemon (printDecoder from ".config")("--");]]]*/
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
			if (tpp_lexer_has(self, TPP_TOK_MINUS_MINUS)) {
				result = TPP_TOK_MINUS_MINUS; /* "--" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
/*[[[end]]]*/
		}	break;
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS || TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT */
/*[[[deemon (printDecoderAfterReadCh2Each from ".config")("-", "-", useSwitch: true);]]]*/
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE || TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE
		case '<': {
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE
			if (tpp_lexer_has(self, TPP_TOK_MINUS_LANGLE_LANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE)) {
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE */
				read_ch2();
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE
				if (ch2 == '<') {
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE
					if (tpp_lexer_has(self, TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE)) {
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE
						tpp_size rel_end_of_3char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE */
						read_ch2();
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE
						if (ch2 == '<') {
							if (tpp_lexer_has(self, TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE)) {
								result = TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE; /* "-<<<" */
								goto set_result;
							}
						} else
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE */
						{
						}
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE
						pos = tpp_file_rel2ptr(file, rel_end_of_3char);
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE */
					}
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE
					if (tpp_lexer_has(self, TPP_TOK_MINUS_LANGLE_LANGLE)) {
						result = TPP_TOK_MINUS_LANGLE_LANGLE; /* "-<<" */
						goto set_result;
					}
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE */
				} else
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE */
				{
				}
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE */
			}
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE
			if (tpp_lexer_has(self, TPP_TOK_MINUS_LANGLE)) {
				result = TPP_TOK_MINUS_LANGLE; /* "-<" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE */
		}	break;
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE || TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
		case '=': {
			if (tpp_lexer_has(self, TPP_TOK_MINUS_EQUAL)) {
				result = TPP_TOK_MINUS_EQUAL; /* "-=" */
				goto set_result;
			}
		}	break;
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR || TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE
		case '>': {
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR || TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE
			if (tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE_STAR) ||
			    tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE_RANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE)) {
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
				read_ch2();
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
				if (ch2 == '*') {
					if (tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE_STAR)) {
						result = TPP_TOK_MINUS_RANGLE_STAR; /* "->*" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE
				if (ch2 == '>') {
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE
					if (tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE)) {
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE
						tpp_size rel_end_of_3char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE */
						read_ch2();
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE
						if (ch2 == '>') {
							if (tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE)) {
								result = TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE; /* "->>>" */
								goto set_result;
							}
						} else
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE */
						{
						}
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE
						pos = tpp_file_rel2ptr(file, rel_end_of_3char);
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE */
					}
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE
					if (tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE_RANGLE)) {
						result = TPP_TOK_MINUS_RANGLE_RANGLE; /* "->>" */
						goto set_result;
					}
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE */
				} else
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE */
				{
				}
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
			}
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR || TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
			if (tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE)) {
				result = TPP_TOK_MINUS_RANGLE; /* "->" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
		}	break;
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR || TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE */
/*[[[end]]]*/
		default: break;
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '@': {
#if (TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT || TPP_HAVE_TPP_TOK_AT_AT_COMMENT)
		if (!tpp_lexer_has(self, TPP_TOK_AT_AT_COMMENT) &&
/*[[[deemon (printHasNone from ".config")("@");]]]*/
		    !tpp_lexer_has(self, TPP_TOK_AT_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_AT_AT) &&
		    !tpp_lexer_has(self, TPP_TOK_AT_AT_EQUAL)
/*[[[end]]]*/
		    )
			break;
		read_ch2();
		switch (ch2) {
#if TPP_HAVE_TPP_TOK_AT_AT || TPP_HAVE_TPP_TOK_AT_AT_COMMENT
		case '@': {
#if TPP_HAVE_TPP_TOK_AT_AT_COMMENT
			if (tpp_lexer_has(self, TPP_TOK_AT_AT_COMMENT)) {
				error = tpp_lexer_seek_eol(self, &pos tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_AT_AT_COMMENT));
				if (TPP_ISERR(error))
					goto return_error;
				result = TPP_TOK_AT_AT_COMMENT; // "@@ like this one!"
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_AT_AT_COMMENT */
/*[[[deemon (printDecoder from ".config")("@@");]]]*/
#if TPP_HAVE_TPP_TOK_AT_AT || TPP_HAVE_TPP_TOK_AT_AT_EQUAL
#if TPP_HAVE_TPP_TOK_AT_AT_EQUAL
			if (tpp_lexer_has(self, TPP_TOK_AT_AT_EQUAL)) {
#if TPP_HAVE_TPP_TOK_AT_AT
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_AT_AT */
				read_ch2();
#if TPP_HAVE_TPP_TOK_AT_AT_EQUAL
				if (ch2 == '=') {
					if (tpp_lexer_has(self, TPP_TOK_AT_AT_EQUAL)) {
						result = TPP_TOK_AT_AT_EQUAL; /* "@@=" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_AT_AT_EQUAL */
				{
				}
#if TPP_HAVE_TPP_TOK_AT_AT
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_AT_AT */
			}
#endif /* TPP_HAVE_TPP_TOK_AT_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_AT_AT
			if (tpp_lexer_has(self, TPP_TOK_AT_AT)) {
				result = TPP_TOK_AT_AT; /* "@@" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_AT_AT */
#endif /* TPP_HAVE_TPP_TOK_AT_AT || TPP_HAVE_TPP_TOK_AT_AT_EQUAL */
/*[[[end]]]*/
		}	break;
#endif /* TPP_HAVE_TPP_TOK_AT_AT || TPP_HAVE_TPP_TOK_AT_AT_COMMENT */
/*[[[deemon (printDecoderAfterReadCh2Each from ".config")("@", "@", useSwitch: true);]]]*/
#if TPP_HAVE_TPP_TOK_AT_EQUAL
		case '=': {
			if (tpp_lexer_has(self, TPP_TOK_AT_EQUAL)) {
				result = TPP_TOK_AT_EQUAL; /* "@=" */
				goto set_result;
			}
		}	break;
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL */
/*[[[end]]]*/
		default: break;
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '/': {
#if (TPP_HAVE_TPP_TOK_SLASH_COMMENT ||                 \
     TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT ||           \
     TPP_HAVE_TPP_TOK_SLASH_STAR_COMMENT_STAR_SLASH || \
     TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH)
#if TPP_HAVE_TPP_TOK_SLASH_COMMENT
		tpp_size rel_end_of_1char;
#endif /* TPP_HAVE_TPP_TOK_SLASH_COMMENT */
		if (!tpp_lexer_has(self, TPP_TOK_SLASH_SLASH_COMMENT) &&
		    !tpp_lexer_has(self, TPP_TOK_SLASH_STAR_COMMENT_STAR_SLASH) &&
		    !tpp_lexer_has(self, TPP_TOK_SLASH_COMMENT) &&
/*[[[deemon (printHasNone from ".config")("/");]]]*/
		    !tpp_lexer_has(self, TPP_TOK_SLASH_SLASH) &&
		    !tpp_lexer_has(self, TPP_TOK_SLASH_SLASH_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_SLASH_EQUAL)
/*[[[end]]]*/
		    )
			break;
#if TPP_HAVE_TPP_TOK_SLASH_COMMENT
		rel_end_of_1char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_SLASH_COMMENT */
		read_ch2();

/*[[[deemon print "#if TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT ||", (getHasPrefixCondition from ".config")("//");]]]*/
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT || TPP_HAVE_TPP_TOK_SLASH_SLASH || TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
/*[[[end]]]*/
		if (ch2 == '/') {
/*[[[deemon (printDecoder from ".config")("//",
	extraRestoreCondition: "TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT",
	shouldPrintPrefixExactMatch: false,
);]]]*/
#if TPP_HAVE_TPP_TOK_SLASH_SLASH || TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
			if (tpp_lexer_has(self, TPP_TOK_SLASH_SLASH_EQUAL)) {
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT || TPP_HAVE_TPP_TOK_SLASH_SLASH
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT || TPP_HAVE_TPP_TOK_SLASH_SLASH */
				read_ch2();
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
				if (ch2 == '=') {
					if (tpp_lexer_has(self, TPP_TOK_SLASH_SLASH_EQUAL)) {
						result = TPP_TOK_SLASH_SLASH_EQUAL; /* "//=" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
				{
				}
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT || TPP_HAVE_TPP_TOK_SLASH_SLASH
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT || TPP_HAVE_TPP_TOK_SLASH_SLASH */
			}
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH || TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
/*[[[end]]]*/
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT
			if (tpp_lexer_has(self, TPP_TOK_SLASH_SLASH_COMMENT)) {
				error = tpp_lexer_seek_eol(self, &pos tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_SLASH_SLASH_COMMENT));
				if (TPP_ISERR(error))
					goto return_error;
				result = TPP_TOK_SLASH_SLASH_COMMENT; // like this one!
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT */
/*[[[deemon (printPrefixExactMatch from ".config")("//");]]]*/
#if TPP_HAVE_TPP_TOK_SLASH_SLASH
			if (tpp_lexer_has(self, TPP_TOK_SLASH_SLASH)) {
				result = TPP_TOK_SLASH_SLASH; /* "//" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH */
/*[[[end]]]*/
		} else
/*[[[deemon print "#endif /" "* TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT ||", (getHasPrefixCondition from ".config")("//"), "*" "/";]]]*/
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT || TPP_HAVE_TPP_TOK_SLASH_SLASH || TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
/*[[[end]]]*/
#if TPP_HAVE_TPP_TOK_SLASH_STAR_COMMENT_STAR_SLASH
		if (ch2 == '*') {
			if (tpp_lexer_has(self, TPP_TOK_SLASH_STAR_COMMENT_STAR_SLASH)) {
				for (;;) {
					read_ch2();
continue_c_comment_with_ch2:
					if (ch2 == 0 && pos >= file->tf_end) {
#if TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF
						error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_start),
						                           TPP_W_COMMENT_TERMINATED_BY_EOF);
						if (TPP_ISERR(error))
							goto return_error;
#endif /* TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF */
						break;
					}
					if (ch2 != '*') {
#if TPP_HAVE_TPP_W_SLASHSTAR_INSIDE_OF_COMMENT
						if (ch2 == '/') {
							tpp_size slash_pos = tpp_file_ptr2rel(file, pos);
							read_ch2();
							if (ch2 != '*')
								continue;
							error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, slash_pos),
							                           TPP_W_SLASHSTAR_INSIDE_OF_COMMENT);
							if (TPP_ISERR(error))
								goto return_error;
						} else
#endif /* TPP_HAVE_TPP_W_SLASHSTAR_INSIDE_OF_COMMENT */
						{
							continue;
						}
					}
					read_ch2();
					if (ch2 == '/')
						break;
					goto continue_c_comment_with_ch2;
				}
				result = TPP_TOK_SLASH_STAR_COMMENT_STAR_SLASH; /* like this one! */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_SLASH_STAR_COMMENT_STAR_SLASH */
/*[[[deemon (printDecoderAfterReadCh2Each from ".config")("/", "/");]]]*/
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_SLASH_EQUAL)) {
				result = TPP_TOK_SLASH_EQUAL; /* "/=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
/*[[[end]]]*/
		{
#if TPP_HAVE_TPP_TOK_SLASH_COMMENT
			if (tpp_lexer_has(self, TPP_TOK_SLASH_COMMENT)) {
				pos = tpp_file_rel2ptr(file, rel_end_of_1char);
				error = tpp_lexer_seek_eol(self, &pos tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_SLASH_COMMENT));
				if (TPP_ISERR(error))
					goto return_error;
				result = TPP_TOK_SLASH_COMMENT; // "/ like this one!"
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_SLASH_COMMENT */
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '%': {
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT || TPP_HAVE_DIGRAPHS
		if (!tpp_lexer_has(self, DIGRAPHS) &&
/*[[[deemon (printHasNone from ".config")("%");]]]*/
		    !tpp_lexer_has(self, TPP_TOK_PERCENT_PERCENT) &&
		    !tpp_lexer_has(self, TPP_TOK_PERCENT_PERCENT_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_PERCENT_EQUAL)
/*[[[end]]]*/
		    )
			break;
		read_ch2();
#if TPP_HAVE_DIGRAPHS
		if (ch2 == '>') {
			if (tpp_lexer_has(self, DIGRAPHS)) {
				result = TPP_TOK_OFCHAR('}'); /* "%>" -> "}" */
				goto set_result;
			}
		} else
		if (ch2 == ':') {
			if (tpp_lexer_has(self, DIGRAPHS)) {
#if TPP_HAVE_TPP_TOK_POUND_POUND
				if (tpp_lexer_has(self, TPP_TOK_POUND_POUND)) {
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
					read_ch2();
					if (ch2 == '%') {
						read_ch2();
						if (ch2 == ':') {
							result = TPP_TOK_POUND_POUND; /* "%:%:" -> "##" */
							goto set_result;
						}
					}
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
				}
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
				result = TPP_TOK_OFCHAR('#'); /* "%:" -> "#" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_DIGRAPHS */
/*[[[deemon (printDecoderAfterReadCh2Each from ".config")("%");]]]*/
#if TPP_HAVE_TPP_TOK_PERCENT_PERCENT || TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL
		if (ch2 == '%') {
#if TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL
			if (tpp_lexer_has(self, TPP_TOK_PERCENT_PERCENT_EQUAL)) {
#if TPP_HAVE_TPP_TOK_PERCENT_PERCENT
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_PERCENT_PERCENT */
				read_ch2();
#if TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL
				if (ch2 == '=') {
					if (tpp_lexer_has(self, TPP_TOK_PERCENT_PERCENT_EQUAL)) {
						result = TPP_TOK_PERCENT_PERCENT_EQUAL; /* "%%=" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL */
				{
				}
#if TPP_HAVE_TPP_TOK_PERCENT_PERCENT
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_PERCENT_PERCENT */
			}
#endif /* TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_PERCENT_PERCENT
			if (tpp_lexer_has(self, TPP_TOK_PERCENT_PERCENT)) {
				result = TPP_TOK_PERCENT_PERCENT; /* "%%" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_PERCENT_PERCENT */
		} else
#endif /* TPP_HAVE_TPP_TOK_PERCENT_PERCENT || TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_PERCENT_EQUAL)) {
				result = TPP_TOK_PERCENT_EQUAL; /* "%=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
/*[[[end]]]*/
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '#': {
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_POUND || TPP_HAVE_TPP_TOK_POUND_COMMENT
/*[[[deemon (printDecoder from ".config")("#",
	extraRestoreCondition: "TPP_HAVE_TPP_TOK_POUND_COMMENT");]]]*/
#if TPP_HAVE_TPP_TOK_POUND_POUND
		if (tpp_lexer_has(self, TPP_TOK_POUND_POUND)) {
#if TPP_HAVE_TPP_TOK_POUND_COMMENT
			tpp_size rel_end_of_1char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_POUND_COMMENT */
			read_ch2();
			if (ch2 == '#') {
				warn_if_ch2_is_trigraph(); /* "??=" -> "#" */
				result = TPP_TOK_POUND_POUND; /* "##" */
				goto set_result;
			}
#if TPP_HAVE_TPP_TOK_POUND_COMMENT
			pos = tpp_file_rel2ptr(file, rel_end_of_1char);
#endif /* TPP_HAVE_TPP_TOK_POUND_COMMENT */
		}
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
/*[[[end]]]*/
#if TPP_HAVE_TPP_TOK_POUND_COMMENT
		if (tpp_lexer_has(self, TPP_TOK_POUND_COMMENT)) {
			error = tpp_lexer_seek_eol(self, &pos tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_POUND_COMMENT));
			if (TPP_ISERR(error))
				goto return_error;
			result = TPP_TOK_POUND_COMMENT; // "# like this one!"
			goto set_result;
		}
#endif /* TPP_HAVE_TPP_TOK_POUND_COMMENT */
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_POUND || TPP_HAVE_TPP_TOK_POUND_COMMENT */
	}	break;
/************************************************************************/



/************************************************************************/
	case ':': {
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON || TPP_HAVE_DIGRAPHS
		if (!tpp_lexer_has(self, DIGRAPHS) &&
/*[[[deemon (printHasNone from ".config")(":");]]]*/
		    !tpp_lexer_has(self, TPP_TOK_COLON_COLON) &&
		    !tpp_lexer_has(self, TPP_TOK_COLON_EQUAL)
/*[[[end]]]*/
		    )
			break;
		read_ch2();
#if TPP_HAVE_DIGRAPHS
		if (ch2 == '>') {
			if (tpp_lexer_has(self, DIGRAPHS)) {
				result = TPP_TOK_OFCHAR(']'); /* ":>" -> "]" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_DIGRAPHS */
/*[[[deemon (printDecoderAfterReadCh2Each from ".config")(":");]]]*/
#if TPP_HAVE_TPP_TOK_COLON_COLON
		if (ch2 == ':') {
			if (tpp_lexer_has(self, TPP_TOK_COLON_COLON)) {
				result = TPP_TOK_COLON_COLON; /* "::" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_COLON_EQUAL)) {
				result = TPP_TOK_COLON_EQUAL; /* ":=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
/*[[[end]]]*/
		{
		}
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON || TPP_HAVE_DIGRAPHS */
	}	break;
/************************************************************************/



/************************************************************************/
	case '?': {
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK || TPP_HAVE_TRIGRAPHS
#if TPP_HAVE_TRIGRAPHS
		if (tpp_lexer_has(self, TRIGRAPHS)) {
			if (pos >= file->tf_end) {
				error = tpp_file_expandchunk(file);
				if (TPP_ISERR(error))
					goto return_error;
				pos = tpp_file_rel2ptr(file, rel_start + 1);
			}
			if (pos < file->tf_end && *pos == '?') {
				if ((pos + 1) >= file->tf_end) {
					error = tpp_file_expandchunk(file);
					if (TPP_ISERR(error))
						goto return_error;
					pos = tpp_file_rel2ptr(file, rel_start + 1);
				}
				if ((pos + 1) < file->tf_end) {
					pos += 2;
					switch (pos[-1]) {
					case '=': ch = '#'; break;
					case '(': ch = '['; break;
					case '/': ch = '\\'; break;
					case ')': ch = ']'; break;
					case '\'': ch = '^'; break;
					case '<': ch = '{'; break;
					case '!': ch = '|'; break;
					case '>': ch = '}'; break;
					case '-': ch = '~'; break;
					case '?': break; /* Continue with '?'-case */
					default:
						/* Not actually a trigraph */
						pos -= 2;
						goto not_a_trigraph;
					}
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
					error = tpp_lexer_warnf_at(self, file, pos - 3, TPP_W_ENCOUNTERED_TRIGRAPH);
					if (TPP_ISERR(error))
						goto return_error;
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
					if (ch != '?')
						goto switch_on_ch;
					/* Continue with '?'-case */
				}
			}
		}
not_a_trigraph:
#endif /* TPP_HAVE_TRIGRAPHS */
/*[[[deemon (printDecoder from ".config")("?", printCondition: false);]]]*/
		if (tpp_lexer_has(self, TPP_TOK_QMARK_EQUAL) ||
		    tpp_lexer_has(self, TPP_TOK_QMARK_QMARK)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL
			if (ch2 == '=') {
				if (tpp_lexer_has(self, TPP_TOK_QMARK_EQUAL)) {
					result = TPP_TOK_QMARK_EQUAL; /* "?=" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK
			if (ch2 == '?') {
				if (tpp_lexer_has(self, TPP_TOK_QMARK_QMARK)) {
					warn_if_ch2_is_trigraph(); /* "???" -> "?" */
					result = TPP_TOK_QMARK_QMARK; /* "??" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK */
			{
			}
		}
/*[[[end]]]*/
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '.': {
#if (TPP_HAVE_TPP_TOK_FLOAT || TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT)
		if (!tpp_lexer_has(self, TPP_TOK_FLOAT) &&
/*[[[deemon (printHasNone from ".config")(".");]]]*/
		    !tpp_lexer_has(self, TPP_TOK_DOT_STAR) &&
		    !tpp_lexer_has(self, TPP_TOK_DOT_DOT) &&
		    !tpp_lexer_has(self, TPP_TOK_DOT_DOT_DOT)
/*[[[end]]]*/
		    )
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_FLOAT
		if (tpp_ascii_isdigit(ch2)) {
			if (tpp_lexer_has(self, TPP_TOK_FLOAT)) {
				result = tpp_lexer_seek_end_of_float(self, &pos, TPP_TOK_FLOAT);
				if (TPP_TOK_ISERR(result)) {
					error = TPP_TOK_ASERR(result);
					goto return_error;
				}
				tpp_assert(result == TPP_TOK_FLOAT);
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
/*[[[deemon (printDecoderAfterReadCh2Each from ".config")(".", "0123456789");]]]*/
#if TPP_HAVE_TPP_TOK_DOT_STAR
		if (ch2 == '*') {
			if (tpp_lexer_has(self, TPP_TOK_DOT_STAR)) {
				result = TPP_TOK_DOT_STAR; /* ".*" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_DOT_DOT || TPP_HAVE_TPP_TOK_DOT_DOT_DOT
		if (ch2 == '.') {
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
			if (tpp_lexer_has(self, TPP_TOK_DOT_DOT_DOT)) {
#if TPP_HAVE_TPP_TOK_DOT_DOT
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
				read_ch2();
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
				if (ch2 == '.') {
					if (tpp_lexer_has(self, TPP_TOK_DOT_DOT_DOT)) {
						result = TPP_TOK_DOT_DOT_DOT; /* "..." */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
				{
				}
#if TPP_HAVE_TPP_TOK_DOT_DOT
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
			}
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_DOT_DOT
			if (tpp_lexer_has(self, TPP_TOK_DOT_DOT)) {
				result = TPP_TOK_DOT_DOT; /* ".." */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
		} else
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT || TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
/*[[[end]]]*/
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
/*[[[deemon (printDecoderAfterReadCh2Each from ".config")("",
	"<-/%#:?.@", // first-token-characters that require custom case-es above
	useSwitch: true
);]]]*/
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM
	case '!': {
		if (tpp_lexer_has(self, TPP_TOK_EXCLAIM_EXCLAIM) ||
		    tpp_lexer_has(self, TPP_TOK_EXCLAIM_EQUAL) ||
		    tpp_lexer_has(self, TPP_TOK_EXCLAIM_EQUAL_EQUAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
			if (ch2 == '!') {
				if (tpp_lexer_has(self, TPP_TOK_EXCLAIM_EXCLAIM)) {
					result = TPP_TOK_EXCLAIM_EXCLAIM; /* "!!" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL || TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
			if (ch2 == '=') {
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
				if (tpp_lexer_has(self, TPP_TOK_EXCLAIM_EQUAL_EQUAL)) {
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
					read_ch2();
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
					if (ch2 == '=') {
						if (tpp_lexer_has(self, TPP_TOK_EXCLAIM_EQUAL_EQUAL)) {
							result = TPP_TOK_EXCLAIM_EQUAL_EQUAL; /* "!==" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
					{
					}
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
				}
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
				if (tpp_lexer_has(self, TPP_TOK_EXCLAIM_EQUAL)) {
					result = TPP_TOK_EXCLAIM_EQUAL; /* "!=" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
			} else
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL || TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
			{
			}
		}
	}	break;
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP
	case '&': {
		if (tpp_lexer_has(self, TPP_TOK_AMP_AMP) ||
		    tpp_lexer_has(self, TPP_TOK_AMP_EQUAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_AMP_AMP
			if (ch2 == '&') {
				if (tpp_lexer_has(self, TPP_TOK_AMP_AMP)) {
					result = TPP_TOK_AMP_AMP; /* "&&" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
			if (ch2 == '=') {
				if (tpp_lexer_has(self, TPP_TOK_AMP_EQUAL)) {
					result = TPP_TOK_AMP_EQUAL; /* "&=" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
			{
			}
		}
	}	break;
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR
	case '*': {
		if (tpp_lexer_has(self, TPP_TOK_STAR_STAR) ||
		    tpp_lexer_has(self, TPP_TOK_STAR_STAR_EQUAL) ||
		    tpp_lexer_has(self, TPP_TOK_STAR_DOT) ||
		    tpp_lexer_has(self, TPP_TOK_STAR_LANGLE_MINUS) ||
		    tpp_lexer_has(self, TPP_TOK_STAR_EQUAL)) {
			read_ch2();
			switch (ch2) {
#if TPP_HAVE_TPP_TOK_STAR_STAR || TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
			case '*': {
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
				if (tpp_lexer_has(self, TPP_TOK_STAR_STAR_EQUAL)) {
#if TPP_HAVE_TPP_TOK_STAR_STAR
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
					read_ch2();
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
					if (ch2 == '=') {
						if (tpp_lexer_has(self, TPP_TOK_STAR_STAR_EQUAL)) {
							result = TPP_TOK_STAR_STAR_EQUAL; /* "**=" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
					{
					}
#if TPP_HAVE_TPP_TOK_STAR_STAR
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
				}
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR
				if (tpp_lexer_has(self, TPP_TOK_STAR_STAR)) {
					result = TPP_TOK_STAR_STAR; /* "**" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
			}	break;
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR || TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_DOT
			case '.': {
				if (tpp_lexer_has(self, TPP_TOK_STAR_DOT)) {
					result = TPP_TOK_STAR_DOT; /* "*." */
					goto set_result;
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_STAR_DOT */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
			case '<': {
				if (tpp_lexer_has(self, TPP_TOK_STAR_LANGLE_MINUS)) {
					read_ch2();
					if (ch2 == '-') {
						result = TPP_TOK_STAR_LANGLE_MINUS; /* "*<-" */
						goto set_result;
					}
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
			case '=': {
				if (tpp_lexer_has(self, TPP_TOK_STAR_EQUAL)) {
					result = TPP_TOK_STAR_EQUAL; /* "*=" */
					goto set_result;
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
			default: break;
			}
		}
	}	break;
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS
	case '+': {
		if (tpp_lexer_has(self, TPP_TOK_PLUS_PLUS) ||
		    tpp_lexer_has(self, TPP_TOK_PLUS_EQUAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
			if (ch2 == '+') {
				if (tpp_lexer_has(self, TPP_TOK_PLUS_PLUS)) {
					result = TPP_TOK_PLUS_PLUS; /* "++" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
			if (ch2 == '=') {
				if (tpp_lexer_has(self, TPP_TOK_PLUS_EQUAL)) {
					result = TPP_TOK_PLUS_EQUAL; /* "+=" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
			{
			}
		}
	}	break;
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL
	case '=': {
		if (tpp_lexer_has(self, TPP_TOK_EQUAL_EXCLAIM) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_PERCENT) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_PERCENT_PERCENT) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_AMP) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_STAR) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_STAR_STAR) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_PLUS) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_MINUS) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_SLASH) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_SLASH_SLASH) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_COLON) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL_EXCLAIM) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL_EQUAL) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_QMARK) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_AT) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_AT_AT) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_HAT) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_PIPE) ||
		    tpp_lexer_has(self, TPP_TOK_EQUAL_TILDE)) {
			read_ch2();
			switch (ch2) {
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
			case '!': {
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_EXCLAIM)) {
					result = TPP_TOK_EQUAL_EXCLAIM; /* "=!" */
					goto set_result;
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT || TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT
			case '%': {
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_PERCENT_PERCENT)) {
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
					read_ch2();
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT
					if (ch2 == '%') {
						if (tpp_lexer_has(self, TPP_TOK_EQUAL_PERCENT_PERCENT)) {
							result = TPP_TOK_EQUAL_PERCENT_PERCENT; /* "=%%" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT */
					{
					}
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_PERCENT)) {
					result = TPP_TOK_EQUAL_PERCENT; /* "=%" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT || TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT */
#if TPP_HAVE_TPP_TOK_EQUAL_AMP
			case '&': {
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_AMP)) {
					result = TPP_TOK_EQUAL_AMP; /* "=&" */
					goto set_result;
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AMP */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR || TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
			case '*': {
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_STAR_STAR)) {
#if TPP_HAVE_TPP_TOK_EQUAL_STAR
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
					read_ch2();
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
					if (ch2 == '*') {
						if (tpp_lexer_has(self, TPP_TOK_EQUAL_STAR_STAR)) {
							result = TPP_TOK_EQUAL_STAR_STAR; /* "=**" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
					{
					}
#if TPP_HAVE_TPP_TOK_EQUAL_STAR
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_STAR)) {
					result = TPP_TOK_EQUAL_STAR; /* "=*" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR || TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_PLUS
			case '+': {
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_PLUS)) {
					result = TPP_TOK_EQUAL_PLUS; /* "=+" */
					goto set_result;
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PLUS */
#if TPP_HAVE_TPP_TOK_EQUAL_MINUS
			case '-': {
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_MINUS)) {
					result = TPP_TOK_EQUAL_MINUS; /* "=-" */
					goto set_result;
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_MINUS */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH || TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
			case '/': {
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_SLASH_SLASH)) {
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH */
					read_ch2();
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
					if (ch2 == '/') {
						if (tpp_lexer_has(self, TPP_TOK_EQUAL_SLASH_SLASH)) {
							result = TPP_TOK_EQUAL_SLASH_SLASH; /* "=//" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
					{
					}
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH */
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_SLASH)) {
					result = TPP_TOK_EQUAL_SLASH; /* "=/" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH */
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH || TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_COLON
			case ':': {
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_COLON)) {
					result = TPP_TOK_EQUAL_COLON; /* "=:" */
					goto set_result;
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_COLON */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
			case '<': {
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE) ||
				    tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE)) {
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
					read_ch2();
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
					if (ch2 == '<') {
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
						if (tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE)) {
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
							tpp_size rel_end_of_3char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
							read_ch2();
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
							if (ch2 == '<') {
								if (tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE)) {
									result = TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE; /* "=<<<" */
									goto set_result;
								}
							} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
							{
							}
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
							pos = tpp_file_rel2ptr(file, rel_end_of_3char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
						}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
						if (tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE)) {
							result = TPP_TOK_EQUAL_LANGLE_LANGLE; /* "=<<" */
							goto set_result;
						}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
					} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
					{
					}
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE)) {
					result = TPP_TOK_EQUAL_LANGLE; /* "=<" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM || TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
			case '=': {
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM || TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL_EXCLAIM) ||
				    tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL_EQUAL)) {
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
					read_ch2();
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
					if (ch2 == '!') {
						if (tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL_EXCLAIM)) {
							result = TPP_TOK_EQUAL_EQUAL_EXCLAIM; /* "==!" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
					if (ch2 == '=') {
						if (tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL_EQUAL)) {
							result = TPP_TOK_EQUAL_EQUAL_EQUAL; /* "===" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
					{
					}
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM || TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL)) {
					result = TPP_TOK_EQUAL_EQUAL; /* "==" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM || TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
			case '>': {
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE) ||
				    tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE)) {
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
					read_ch2();
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
					if (ch2 == '>') {
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
						if (tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE)) {
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
							tpp_size rel_end_of_3char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
							read_ch2();
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
							if (ch2 == '>') {
								if (tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE)) {
									result = TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE; /* "=>>>" */
									goto set_result;
								}
							} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
							{
							}
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
							pos = tpp_file_rel2ptr(file, rel_end_of_3char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
						}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
						if (tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE)) {
							result = TPP_TOK_EQUAL_RANGLE_RANGLE; /* "=>>" */
							goto set_result;
						}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
					} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
					{
					}
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE)) {
					result = TPP_TOK_EQUAL_RANGLE; /* "=>" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_QMARK
			case '?': {
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_QMARK)) {
					warn_if_ch2_is_trigraph(); /* "???" -> "?" */
					result = TPP_TOK_EQUAL_QMARK; /* "=?" */
					goto set_result;
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_QMARK */
#if TPP_HAVE_TPP_TOK_EQUAL_AT || TPP_HAVE_TPP_TOK_EQUAL_AT_AT
			case '@': {
#if TPP_HAVE_TPP_TOK_EQUAL_AT_AT
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_AT_AT)) {
#if TPP_HAVE_TPP_TOK_EQUAL_AT
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT */
					read_ch2();
#if TPP_HAVE_TPP_TOK_EQUAL_AT_AT
					if (ch2 == '@') {
						if (tpp_lexer_has(self, TPP_TOK_EQUAL_AT_AT)) {
							result = TPP_TOK_EQUAL_AT_AT; /* "=@@" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT_AT */
					{
					}
#if TPP_HAVE_TPP_TOK_EQUAL_AT
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT */
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT_AT */
#if TPP_HAVE_TPP_TOK_EQUAL_AT
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_AT)) {
					result = TPP_TOK_EQUAL_AT; /* "=@" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT */
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT || TPP_HAVE_TPP_TOK_EQUAL_AT_AT */
#if TPP_HAVE_TPP_TOK_EQUAL_HAT
			case '^': {
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_HAT)) {
					warn_if_ch2_is_trigraph(); /* "??'" -> "^" */
					result = TPP_TOK_EQUAL_HAT; /* "=^" */
					goto set_result;
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_HAT */
#if TPP_HAVE_TPP_TOK_EQUAL_PIPE
			case '|': {
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_PIPE)) {
					warn_if_ch2_is_trigraph(); /* "??!" -> "|" */
					result = TPP_TOK_EQUAL_PIPE; /* "=|" */
					goto set_result;
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PIPE */
#if TPP_HAVE_TPP_TOK_EQUAL_TILDE
			case '~': {
				if (tpp_lexer_has(self, TPP_TOK_EQUAL_TILDE)) {
					warn_if_ch2_is_trigraph(); /* "??-" -> "~" */
					result = TPP_TOK_EQUAL_TILDE; /* "=~" */
					goto set_result;
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_TILDE */
			default: break;
			}
		}
	}	break;
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE
	case '>': {
		if (tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS) ||
		    tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS_LANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS_RANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_RANGLE_LANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL) ||
		    tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL_LANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL_RANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_MINUS) ||
		    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_EQUAL) ||
		    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE) ||
		    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS) ||
		    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)) {
			read_ch2();
			switch (ch2) {
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
			case '-': {
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
				if (tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS_LANGLE) ||
				    tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS_RANGLE)) {
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
					read_ch2();
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
					if (ch2 == '<') {
						if (tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS_LANGLE)) {
							result = TPP_TOK_RANGLE_MINUS_LANGLE; /* ">-<" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
					if (ch2 == '>') {
						if (tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS_RANGLE)) {
							result = TPP_TOK_RANGLE_MINUS_RANGLE; /* ">->" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
					{
					}
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
				}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
				if (tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS)) {
					result = TPP_TOK_RANGLE_MINUS; /* ">-" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
			}	break;
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
			case '<': {
				if (tpp_lexer_has(self, TPP_TOK_RANGLE_LANGLE)) {
					result = TPP_TOK_RANGLE_LANGLE; /* "><" */
					goto set_result;
				}
			}	break;
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL || TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
			case '=': {
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
				if (tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL_LANGLE) ||
				    tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL_RANGLE)) {
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
					read_ch2();
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
					if (ch2 == '<') {
						if (tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL_LANGLE)) {
							result = TPP_TOK_RANGLE_EQUAL_LANGLE; /* ">=<" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
					if (ch2 == '>') {
						if (tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL_RANGLE)) {
							result = TPP_TOK_RANGLE_EQUAL_RANGLE; /* ">=>" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
					{
					}
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
				}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
				if (tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL)) {
					result = TPP_TOK_RANGLE_EQUAL; /* ">=" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
			}	break;
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL || TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
			case '>': {
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
				if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_MINUS) ||
				    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_EQUAL) ||
				    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE) ||
				    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS) ||
				    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)) {
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
					read_ch2();
					switch (ch2) {
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS
					case '-': {
						if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_MINUS)) {
							result = TPP_TOK_RANGLE_RANGLE_MINUS; /* ">>-" */
							goto set_result;
						}
					}	break;
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
					case '=': {
						if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_EQUAL)) {
							result = TPP_TOK_RANGLE_RANGLE_EQUAL; /* ">>=" */
							goto set_result;
						}
					}	break;
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
					case '>': {
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
						if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS) ||
						    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)) {
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
							tpp_size rel_end_of_3char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
							read_ch2();
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS
							if (ch2 == '-') {
								if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS)) {
									result = TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS; /* ">>>-" */
									goto set_result;
								}
							} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
							if (ch2 == '=') {
								if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)) {
									result = TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL; /* ">>>=" */
									goto set_result;
								}
							} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
							{
							}
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
							pos = tpp_file_rel2ptr(file, rel_end_of_3char);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
						}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
						if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE)) {
							result = TPP_TOK_RANGLE_RANGLE_RANGLE; /* ">>>" */
							goto set_result;
						}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
					}	break;
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
					default: break;
					}
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
				}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
				if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE)) {
					result = TPP_TOK_RANGLE_RANGLE; /* ">>" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
			}	break;
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
			default: break;
			}
		}
	}	break;
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT
	case '^': {
		if (tpp_lexer_has(self, TPP_TOK_HAT_EQUAL) ||
		    tpp_lexer_has(self, TPP_TOK_HAT_HAT)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
			if (ch2 == '=') {
				if (tpp_lexer_has(self, TPP_TOK_HAT_EQUAL)) {
					result = TPP_TOK_HAT_EQUAL; /* "^=" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_HAT
			if (ch2 == '^') {
				if (tpp_lexer_has(self, TPP_TOK_HAT_HAT)) {
					warn_if_ch2_is_trigraph(); /* "??'" -> "^" */
					result = TPP_TOK_HAT_HAT; /* "^^" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT */
			{
			}
		}
	}	break;
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE
	case '|': {
		if (tpp_lexer_has(self, TPP_TOK_PIPE_EQUAL) ||
		    tpp_lexer_has(self, TPP_TOK_PIPE_PIPE)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
			if (ch2 == '=') {
				if (tpp_lexer_has(self, TPP_TOK_PIPE_EQUAL)) {
					result = TPP_TOK_PIPE_EQUAL; /* "|=" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
			if (ch2 == '|') {
				if (tpp_lexer_has(self, TPP_TOK_PIPE_PIPE)) {
					warn_if_ch2_is_trigraph(); /* "??!" -> "|" */
					result = TPP_TOK_PIPE_PIPE; /* "||" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE */
			{
			}
		}
	}	break;
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE
	case '~': {
		if (tpp_lexer_has(self, TPP_TOK_TILDE_EQUAL) ||
		    tpp_lexer_has(self, TPP_TOK_TILDE_TILDE)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL
			if (ch2 == '=') {
				if (tpp_lexer_has(self, TPP_TOK_TILDE_EQUAL)) {
					result = TPP_TOK_TILDE_EQUAL; /* "~=" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE
			if (ch2 == '~') {
				if (tpp_lexer_has(self, TPP_TOK_TILDE_TILDE)) {
					warn_if_ch2_is_trigraph(); /* "??-" -> "~" */
					result = TPP_TOK_TILDE_TILDE; /* "~~" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE */
			{
			}
		}
	}	break;
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE */
/*[[[end]]]*/
/************************************************************************/



/************************************************************************/
	case '(': {
#if TPP_HAVE_TPP_TOK_LPAREN_STAR_COMMENT_STAR_RPAREN
		if (!tpp_lexer_has(self, TPP_TOK_LPAREN_STAR_COMMENT_STAR_RPAREN))
			break;
		read_ch2(); // "(*like this one!*)"
		if (ch2 != '*')
			break;
		for (;;) {
			read_ch2();
continue_pascal_comment_with_ch2:
			if (ch2 == 0 && pos >= file->tf_end) {
#if TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF
				error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_start),
				                           TPP_W_COMMENT_TERMINATED_BY_EOF);
				if (TPP_ISERR(error))
					goto return_error;
#endif /* TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF */
				break;
			}
			if (ch2 != '*')
				continue;
			read_ch2();
			if (ch2 == ')')
				break;
			goto continue_pascal_comment_with_ch2;
		}
		result = TPP_TOK_LPAREN_STAR_COMMENT_STAR_RPAREN;
		goto set_result;
#endif /* TPP_HAVE_TPP_TOK_LPAREN_STAR_COMMENT_STAR_RPAREN */
	}	break;
/************************************************************************/



/************************************************************************/
	case '\\': {
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS
		if (tpp_lexer_has(self, ESCAPE_IN_IDENTIFIERS)) {
			tpp_char const *npos;
			tpp_size rel_before, rel_after;
			npos = tpp_file_rel2ptr(file, rel_start);
			rel_before = tpp_file_ptr2rel(file, npos);
			error = tpp_lexer_skip_bsi(self, &npos);
			if (TPP_ISERR(error))
				goto return_error;
			rel_after = tpp_file_ptr2rel(file, npos);
			tpp_assert(rel_before <= rel_after);
			if (rel_before < rel_after) {
				pos = npos;
				goto handle_keyword_with_esc;
#define WANT_handle_keyword_with_esc
			}
		}
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */
		{
#if TPP_HAVE_BSE
			tpp_char const *npos;
			tpp_size rel_before, rel_after;
			npos = tpp_file_rel2ptr(file, rel_start);
			rel_before = tpp_file_ptr2rel(file, npos);
			error = tpp_lexer_skip_bse(self, &npos);
			if (TPP_ISERR(error))
				goto return_error;
			rel_after = tpp_file_ptr2rel(file, npos);
			tpp_assert(rel_before <= rel_after);
			if (rel_before >= rel_after)
				break; /* No BSE -> regular backslash */

			/* BSE was skipped -> read whatever comes after... */
			*p_pos = npos;
			goto again;
#endif /* TPP_HAVE_BSE */
		}
	}	break;
/************************************************************************/



/************************************************************************/
	case '\'': {
#if TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL || TPP_HAVE_TPP_TOK_C_CHAR
#if TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
		if (tpp_lexer_has(self, TPP_TOK_BLOCK_CHAR_LITERAL)) {
#if TPP_HAVE_TPP_TOK_C_CHAR
			tpp_size rel_end_of_1char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_C_CHAR */
			read_ch2();
			if (ch2 == '\'') {
				read_ch2();
				if (ch2 == '\'') {
					error = tpp_lexer_seek_end_of_block_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_BLOCK_CHAR_LITERAL; /* '''foo''' */
					goto set_result;
				}
			}
#if TPP_HAVE_TPP_TOK_C_CHAR
			pos = tpp_file_rel2ptr(file, rel_end_of_1char);
#endif /* TPP_HAVE_TPP_TOK_C_CHAR */
		}
#endif /* TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_C_CHAR
		if (tpp_lexer_has(self, TPP_TOK_C_CHAR)) {
			error = tpp_lexer_seek_end_of_string(self, &pos, '\'');
			if (TPP_ISERR(error))
				goto return_error;
			result = TPP_TOK_C_CHAR; /* 'foo' */
			goto set_result;
		}
#endif /* TPP_HAVE_TPP_TOK_C_CHAR */
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '"': {
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_C_STRING
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
		if (tpp_lexer_has(self, TPP_TOK_BLOCK_STRING_LITERAL)) {
#if TPP_HAVE_TPP_TOK_C_STRING
			tpp_size rel_end_of_1char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_C_STRING */
			read_ch2();
			if (ch2 == '"') {
				read_ch2();
				if (ch2 == '"') {
					error = tpp_lexer_seek_end_of_block_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_BLOCK_STRING_LITERAL; /* """foo""" */
					goto set_result;
				}
			}
#if TPP_HAVE_TPP_TOK_C_STRING
			pos = tpp_file_rel2ptr(file, rel_end_of_1char);
#endif /* TPP_HAVE_TPP_TOK_C_STRING */
		}
#endif /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_C_STRING
		if (tpp_lexer_has(self, TPP_TOK_C_STRING)) {
			error = tpp_lexer_seek_end_of_string(self, &pos, '"');
			if (TPP_ISERR(error))
				goto return_error;
			result = TPP_TOK_C_STRING; /* "foo" */
			goto set_result;
		}
#endif /* TPP_HAVE_TPP_TOK_C_STRING */
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
#if (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL ||     \
     TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL)
	case 'R': {
		if (tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_RAW_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_RAW_CHAR_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
			if (ch2 == '"') {
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
				if (tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_RAW_STRING_LITERAL; /* R"AB(foo)AB" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
				if (tpp_lexer_has(self, TPP_TOK_RAW_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_raw_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_RAW_STRING_LITERAL; /* R"foo" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
			if (ch2 == '\'') {
#if TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
				if (tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_RAW_CHAR_LITERAL; /* R'AB(f)AB' */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
				if (tpp_lexer_has(self, TPP_TOK_RAW_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_raw_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_RAW_CHAR_LITERAL; /* R'foo' */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if (TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL)
	case 'r': {
		if (tpp_lexer_has(self, TPP_TOK_RAW_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_RAW_CHAR_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
			if (ch2 == '"') {
				if (tpp_lexer_has(self, TPP_TOK_RAW_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_raw_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_RAW_STRING_LITERAL; /* r"foo"  */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
			if (ch2 == '\'') {
				if (tpp_lexer_has(self, TPP_TOK_RAW_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_raw_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_RAW_CHAR_LITERAL; /* r'foo' */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
	case 'L': {
		if (tpp_lexer_has(self, TPP_TOK_CXX_WIDE_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_CXX_WIDE_CHAR_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
			if (ch2 == '"') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_WIDE_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_WIDE_STRING_LITERAL; /* L"foo" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
			if (ch2 == '\'') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_WIDE_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_WIDE_CHAR_LITERAL; /* L'f' */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */
#if ((TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || \
     (TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL))
			if (ch2 == 'R') {
				if ((tpp_lexer_has(self, TPP_TOK_CXX_WIDE_STRING_LITERAL) &&
				     tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) ||
				    (tpp_lexer_has(self, TPP_TOK_CXX_WIDE_CHAR_LITERAL) &&
				     tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL))) {
					read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
					if (ch2 == '"') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_WIDE_STRING_LITERAL) &&
						    tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) {
							error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos, '"');
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL; /* LR"AB(foo)AB" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
					if (ch2 == '\'') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_WIDE_CHAR_LITERAL) &&
						    tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL)) {
							error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos, '\'');
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_RAW_WIDE_CHAR_LITERAL; /* LR'AB(f)AB' */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
					{
					}
				}
			} else
#endif /* ... */
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if (TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL)
	case 'u': {
		if (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_CXX_UTF16_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_CXX_UTF8_CHAR_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_CXX_UTF16_CHAR_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
			if (ch2 == '8') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_STRING_LITERAL) ||
				    tpp_lexer_has(self, TPP_TOK_CXX_UTF8_CHAR_LITERAL)) {
					read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
					if (ch2 == '"') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_STRING_LITERAL)) {
							error = tpp_lexer_seek_end_of_string(self, &pos, '"');
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_UTF8_STRING_LITERAL; /* u8"foo" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
					if (ch2 == '\'') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_CHAR_LITERAL)) {
							error = tpp_lexer_seek_end_of_string(self, &pos, '\'');
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_UTF8_CHAR_LITERAL; /* u8'f' */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */
#if ((TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || \
     (TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL))
					if (ch2 == 'R') {
						if ((tpp_lexer_has(self, TPP_TOK_CXX_UTF8_STRING_LITERAL) &&
						     tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) ||
						    (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_CHAR_LITERAL) &&
						     tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL))) {
							read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
							if (ch2 == '"') {
								if (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_STRING_LITERAL) &&
								    tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) {
									error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos, '"');
									if (TPP_ISERR(error))
										goto return_error;
									result = TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL; /* u8R"AB(foo)AB" */
									goto set_result;
								}
							} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
							if (ch2 == '\'') {
								if (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_CHAR_LITERAL) &&
								    tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL)) {
									error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos, '\'');
									if (TPP_ISERR(error))
										goto return_error;
									result = TPP_TOK_CXX_RAW_UTF8_CHAR_LITERAL; /* u8R'AB(f)AB' */
									goto set_result;
								}
							} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
							{
							}
						}
					} else
#endif /* ... */
					{
					}
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
			if (ch2 == '"') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_UTF16_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_UTF16_STRING_LITERAL; /* u"foo" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
			if (ch2 == '\'') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_UTF16_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_UTF16_CHAR_LITERAL; /* u'f' */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL */
#if ((TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || \
     (TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL))
			if (ch2 == 'R') {
				if ((tpp_lexer_has(self, TPP_TOK_CXX_UTF16_STRING_LITERAL) &&
				     tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) ||
				    (tpp_lexer_has(self, TPP_TOK_CXX_UTF16_CHAR_LITERAL) &&
				     tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL))) {
					read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
					if (ch2 == '"') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_UTF16_STRING_LITERAL) &&
						    tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) {
							error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos, '"');
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL; /* uR"AB(foo)AB" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
					if (ch2 == '\'') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_UTF16_CHAR_LITERAL) &&
						    tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL)) {
							error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos, '\'');
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_RAW_UTF16_CHAR_LITERAL; /* uR'AB(f)AB' */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
					{
					}
				}
			} else
#endif /* ... */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
	case 'U': {
		if (tpp_lexer_has(self, TPP_TOK_CXX_UTF32_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_CXX_UTF32_CHAR_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
			if (ch2 == '"') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_UTF32_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_UTF32_STRING_LITERAL; /* U"foo" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
			if (ch2 == '\'') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_UTF32_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_UTF32_CHAR_LITERAL; /* U'f' */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */
#if ((TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || \
     (TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL))
			if (ch2 == 'R') {
				if ((tpp_lexer_has(self, TPP_TOK_CXX_UTF32_STRING_LITERAL) &&
				     tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) ||
				    (tpp_lexer_has(self, TPP_TOK_CXX_UTF32_CHAR_LITERAL) &&
				     tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL))) {
					read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
					if (ch2 == '"') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_UTF32_STRING_LITERAL) &&
						    tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) {
							error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos, '"');
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL; /* UR"AB(foo)AB" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
					if (ch2 == '\'') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_UTF32_CHAR_LITERAL) &&
						    tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL)) {
							error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos, '\'');
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_RAW_UTF32_CHAR_LITERAL; /* UR'AB(f)AB' */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
					{
					}
				}
			} else
#endif /* ... */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_DOLLAR
	case '$':
#if TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_DOLLAR)
		if (tpp_lexer_has(self, TPP_TOK_DOLLAR))
			break; /* Follow single-char code-branch */
		goto handle_keyword;
#define WANT_handle_keyword
#else /* TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_DOLLAR) */
		break; /* Follow single-char code-branch */
#endif /* !TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_DOLLAR) */
#endif /* TPP_HAVE_TPP_TOK_DOLLAR */
/************************************************************************/


#if TPP_HAVE_ASSUME_ASCII_CTYPE
#define tpp_ascii_maybe_test(x) 0
#else /* TPP_HAVE_ASSUME_ASCII_CTYPE */
#define tpp_ascii_maybe_test(x) x
	default:
#endif /* !TPP_HAVE_ASSUME_ASCII_CTYPE */
	{
		/* Check for utf-8 sequence */
#if TPP_HAVE_UNICODE
		if (tpp_ascii_maybe_test(tpp_ascii_ismb(ch))) {
#if TPP_HAVE_ASSUME_ASCII_CTYPE
#if tpp_ascii_ismb(0x80)
	case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
	case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
	case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
	case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
	case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
	case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
	case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
	case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
#else /* tpp_ascii_ismb(0x80) */
#if !TPP_IGNORE_INVALID_CONFIGURATION && tpp_ascii_ismb(0xbf)
#error "Unsupported 'tpp_ascii_ismb' configuration. Try re-building with -DTPP_HAVE_ASSUME_ASCII_CTYPE=0"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && tpp_ascii_ismb(0xbf) */
#endif /* !tpp_ascii_ismb(0x80) */
	case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
	case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
	case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
	case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
	case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
	case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
	case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
	case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
#endif /* TPP_HAVE_ASSUME_ASCII_CTYPE */
			if (tpp_file_isutf8(file)) {
				tpp_unichar uc;
				--pos;
				error = tpp_lexer_readutf8(self, &pos, &uc);
				if (TPP_ISERR(error))
					goto return_error;
				if tpp_unlikely(uc == 0 && !tpp_file_isutf8(file)) {
					++pos; /* Malformed utf-8 sequence caused unicode to be disabled */
					break;
				}

				/* Handle unicode character traits */
				if (tpp_unicode_islf(uc))
					goto handle_linefeed;
#define WANT_handle_linefeed
				if (tpp_unicode_isspace(uc))
					goto handle_space;
#define WANT_handle_space
				if (tpp_unicode_issymstrt(uc))
					goto handle_keyword;
#define WANT_handle_keyword
				result = TPP_TOK_UNICHAR;
				goto set_result;
			}
		}
#endif /* TPP_HAVE_UNICODE */

		if (tpp_ascii_maybe_test(tpp_ascii_islf(ch))) {
			/* Deal with \r\n linefeed sequences */
#if TPP_HAVE_ASSUME_ASCII_CTYPE
	case TPP_ASCII_CR:
#endif /* TPP_HAVE_ASSUME_ASCII_CTYPE */
#if TPP_HAVE_CR_LF_DETECTION
#if !TPP_HAVE_ASSUME_ASCII_CTYPE
			if (ch == TPP_ASCII_CR)
#endif /* !TPP_HAVE_ASSUME_ASCII_CTYPE */
			{
				if tpp_unlikely(pos >= end) {
					error = tpp_file_expandchunk(file);
					if (TPP_ISERR(error))
						goto return_error;
					pos = tpp_file_rel2ptr(file, rel_start + 1);
					end = file->tf_end;
					if (pos >= end)
						goto handle_linefeed; /* \r followed by EOF */
#define WANT_handle_linefeed
				}
				if (*pos == TPP_ASCII_LF)
					++pos; /* 2-byte linefeed sequence */
			}
#if TPP_HAVE_ASSUME_ASCII_CTYPE
			TPP_FALLTHRU
#endif /* TPP_HAVE_ASSUME_ASCII_CTYPE */
#endif /* TPP_HAVE_CR_LF_DETECTION */
#if TPP_HAVE_ASSUME_ASCII_CTYPE
	case TPP_ASCII_LF:
#endif /* TPP_HAVE_ASSUME_ASCII_CTYPE */
#ifdef WANT_handle_linefeed
#undef WANT_handle_linefeed
handle_linefeed:
#endif /* WANT_handle_linefeed */
			result = TPP_TOK_LF;
			goto set_result;
		}

		/* Check for space */
		if (tpp_ascii_maybe_test(tpp_ascii_isspace(ch))) {
	case (tpp_char)(unsigned int)TPP_TOK_EOF: /* Important: 0 must *always* be treated as whitespace. Else, we might accidentally indicate EOF */
#if TPP_HAVE_ASSUME_ASCII_CTYPE
	case (tpp_char)1: case (tpp_char)2: case (tpp_char)3:
	case (tpp_char)4: case (tpp_char)5: case (tpp_char)6:
	case (tpp_char)7: case (tpp_char)8: case (tpp_char)9:
/*	case (tpp_char)10: * TPP_ASCII_LF */
	case (tpp_char)11: case (tpp_char)12:
/*	case (tpp_char)13: * TPP_ASCII_CR */
	case (tpp_char)14: case (tpp_char)15: case (tpp_char)16:
	case (tpp_char)17: case (tpp_char)18: case (tpp_char)19:
	case (tpp_char)20: case (tpp_char)21: case (tpp_char)22:
	case (tpp_char)23: case (tpp_char)24: case (tpp_char)25:
	case (tpp_char)26: case (tpp_char)27: case (tpp_char)28:
	case (tpp_char)29: case (tpp_char)30: case (tpp_char)31:
	case (tpp_char)32:
#endif /* TPP_HAVE_ASSUME_ASCII_CTYPE */
#ifdef WANT_handle_space
#undef WANT_handle_space
handle_space:
#endif /* WANT_handle_space */
			error = tpp_lexer_skipspace_nolf(self, &pos);
			if (TPP_ISERR(error))
				goto return_error;
#if TPP_HAVE_BSE
			while (pos < file->tf_end && *pos == '\\') {
				tpp_char const *npos;
				tpp_size rel_before, rel_after;
				npos = pos;
				error = tpp_lexer_skip_bse(self, &npos);
				if (TPP_ISERR(error))
					goto return_error;
				if (npos == pos)
					break;
				rel_before = tpp_file_ptr2rel(file, npos);
				error = tpp_lexer_skipspace_nolf(self, &npos);
				if (TPP_ISERR(error))
					goto return_error;
				rel_after  = tpp_file_ptr2rel(file, npos);
				tpp_assert(rel_before <= rel_after);
				if (rel_before >= rel_after)
					break;
				pos = npos;
			}
#endif /* TPP_HAVE_BSE */

			result = TPP_TOK_SPACE;
			goto set_result;
		}

		/* Check for keywords... */
		if (tpp_ascii_maybe_test(tpp_ascii_issymstrt(ch))) {
			tpp_char const *kwd_start;
			tpp_size kwd_len;
			tpp_hash kwd_hash;
			tpp_keyword const *kwd;
#if TPP_HAVE_ESCAPED_KEYWORDS
			bool uses_esc;
			tpp_size rel_kwd_end;
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */
#if TPP_HAVE_ASSUME_ASCII_CTYPE
		case 'a': case 'b': case 'c': case 'd': case 'e':
		case 'f': case 'g': case 'h': case 'i': case 'j':
		case 'k': case 'l': case 'm': case 'n': case 'o':
		case 'p': case 'q':
#if !TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL && !TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
		case 'r':
#endif /* !TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL && !TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
		case 's': case 't':
#if (!TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && !TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && \
     !TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && !TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL)
		case 'u':
#endif /* ... */
		case 'v': case 'w': case 'x': case 'y': case 'z':
		case 'A': case 'B': case 'C': case 'D': case 'E':
		case 'F': case 'G': case 'H': case 'I': case 'J':
		case 'K':
#if !TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && !TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
		case 'L':
#endif /* !TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && !TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */
		case 'M': case 'N': case 'O': case 'P': case 'Q':
#if (!TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL && !TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL && \
     !TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL && !TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL)
		case 'R':
#endif /* ... */
		case 'S': case 'T':
#if !TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && !TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
		case 'U':
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && !TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */
		case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case '_':
#if !TPP_HAVE_TPP_TOK_DOLLAR
		case '$':
#endif /* !TPP_HAVE_TPP_TOK_DOLLAR */
#endif /* TPP_HAVE_ASSUME_ASCII_CTYPE */

#ifdef WANT_handle_keyword
#undef WANT_handle_keyword
handle_keyword:
#endif /* WANT_handle_keyword */
#if TPP_HAVE_ESCAPED_KEYWORDS
			uses_esc = false;
#ifdef WANT_handle_keyword_with_esc
#undef WANT_handle_keyword_with_esc
			if (0) {
handle_keyword_with_esc:
				uses_esc = true;
			}
#endif /* WANT_handle_keyword_with_esc */
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */
			error = tpp_lexer_seek_end_of_keyword(self, &pos);
			if (TPP_ISERR(error))
				goto return_error;
#if TPP_HAVE_ESCAPED_KEYWORDS
			rel_kwd_end = tpp_file_ptr2rel(file, pos);
			for (;;) {
				if (pos >= file->tf_end) {
					tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
					error = tpp_file_expandchunk(file);
					if (TPP_ISERR(error))
						goto return_error;
					pos = tpp_file_rel2ptr(file, rel_pos);
					if (pos >= file->tf_end)
						break;
				}
				if (*pos == '\\') {
					/* Backslash */
				} else
#if TPP_HAVE_TRIGRAPHS
				if (*pos == '?' && tpp_lexer_has(self, TRIGRAPHS)) {
					if ((pos + 1) >= file->tf_end) {
						tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
						error = tpp_file_expandchunk(file);
						if (TPP_ISERR(error))
							goto return_error;
						pos = tpp_file_rel2ptr(file, rel_pos);
					}
					if ((pos + 1) < file->tf_end && pos[1] == '?') {
						if ((pos + 2) >= file->tf_end) {
							tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
							error = tpp_file_expandchunk(file);
							if (TPP_ISERR(error))
								goto return_error;
							pos = tpp_file_rel2ptr(file, rel_pos);
						}
						if ((pos + 2) < file->tf_end && pos[2] == '/') {
							/* Trigraph backslash */
						} else {
							break;
						}
					} else {
						break;
					}
				} else
#endif /* TPP_HAVE_TRIGRAPHS */
				{
					break; /* No backslash */
				}
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS
				if (tpp_lexer_has(self, ESCAPE_IN_IDENTIFIERS)) {
					tpp_char const *npos = pos;
					tpp_size rel_pos;
					rel_pos = tpp_file_ptr2rel(file, pos);
					error = tpp_lexer_skip_bsi(self, &npos);
					pos = tpp_file_rel2ptr(file, rel_pos);
					if (TPP_ISERR(error))
						goto return_error;
					tpp_assert(pos <= npos);
					if (pos < npos) {
						pos = npos;
						uses_esc = true;
						error = tpp_lexer_seek_end_of_keyword(self, &pos);
						if (TPP_ISERR(error))
							goto return_error;
						rel_kwd_end = tpp_file_ptr2rel(file, pos);
						continue;
					}
				}
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */
				{
#if TPP_HAVE_BSE
					tpp_char const *npos = pos;
					tpp_size rel_pos;
					rel_pos = tpp_file_ptr2rel(file, pos);
					error = tpp_lexer_skip_bse(self, &npos);
					pos = tpp_file_rel2ptr(file, rel_pos);
					if (TPP_ISERR(error))
						goto return_error;
					tpp_assert(pos <= npos);
					if (pos < npos) {
						pos = npos;
						rel_pos = tpp_file_ptr2rel(file, pos);
						error = tpp_lexer_seek_end_of_keyword(self, &npos);
						pos = tpp_file_rel2ptr(file, rel_pos);
						if (TPP_ISERR(error))
							goto return_error;
						tpp_assert(pos <= npos);
						if (pos < npos) {
							pos = npos;
							uses_esc = true;
							rel_kwd_end = tpp_file_ptr2rel(file, pos);
						} else {
							/* Still continue in case there's more to be found... */
#if !TPP_HAVE_ESCAPE_IN_IDENTIFIERS
							break; /* No need to continue without \u-identifiers */
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */
						}
						continue;
					}
#endif /* TPP_HAVE_BSE */
				}
				break;
			}
			pos = tpp_file_rel2ptr(file, rel_kwd_end);
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */

			/* Lookup/create keyword */
			kwd_start = tpp_file_rel2ptr(file, rel_start);
			kwd_len   = (tpp_size)(pos - kwd_start);
#if TPP_HAVE_ESCAPED_KEYWORDS
			if (uses_esc) {
				kwd_hash = tpp_hashof_esc(kwd_start, kwd_len, file);
#if TPP_HAVE_USER_KEYWORDS
				kwd = tpp_lexer_kwds_newkeyword_esc(self, kwd_start, kwd_len, kwd_hash, file);
#else /* TPP_HAVE_USER_KEYWORDS */
				kwd = tpp_lexer_kwds_getkeyword_esc(self, kwd_start, kwd_len, kwd_hash, file);
#endif /* !TPP_HAVE_USER_KEYWORDS */
			} else
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */
			{
				kwd_hash = tpp_hashof(kwd_start, kwd_len);
#if TPP_HAVE_USER_KEYWORDS
				kwd = tpp_lexer_kwds_newkeyword(self, kwd_start, kwd_len, kwd_hash);
#else /* TPP_HAVE_USER_KEYWORDS */
				kwd = tpp_lexer_kwds_getkeyword(self, kwd_start, kwd_len, kwd_hash);
#endif /* !TPP_HAVE_USER_KEYWORDS */
			}
#if TPP_HAVE_USER_KEYWORDS
			if tpp_unlikely(!kwd) {
				error = TPP_ENOMEM;
				goto return_error;
			}
#else /* TPP_HAVE_USER_KEYWORDS */
			if (!kwd) {
				token->tt_kwd = NULL;
				result = TPP_TOK_USERKEYWORD;
				goto set_result;
			}
#endif /* !TPP_HAVE_USER_KEYWORDS */
			token->tt_kwd = kwd;
			result = kwd->tk_id;
			goto set_result;
		}

		/* Check for digits */
#if TPP_HAVE_TPP_TOK_INT || TPP_HAVE_TPP_TOK_FLOAT
		if (tpp_ascii_maybe_test(tpp_ascii_isdigit(ch))) {
#if TPP_HAVE_ASSUME_ASCII_CTYPE
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
#endif /* TPP_HAVE_ASSUME_ASCII_CTYPE */
#if TPP_HAVE_TPP_TOK_FLOAT
			if (tpp_lexer_has(self, TPP_TOK_FLOAT)) {
#if TPP_HAVE_TPP_TOK_INT
				result = TPP_TOK_INT;
#else /* TPP_HAVE_TPP_TOK_INT */
				result = TPP_TOK_EOF;
#endif /* !TPP_HAVE_TPP_TOK_INT */
				result = tpp_lexer_seek_end_of_float(self, &pos, result);
				if (TPP_TOK_ISERR(result)) {
					error = TPP_TOK_ASERR(result);
					goto return_error;
				}
#if TPP_HAVE_TPP_TOK_INT
				tpp_assert(result == TPP_TOK_INT || result == TPP_TOK_FLOAT);
#if TPP_CONF_IS_RT(TPP_HAVE_TPP_TOK_INT)
				if (!tpp_lexer_has(self, TPP_TOK_INT))
					result = TPP_TOK_FLOAT;
#endif /* TPP_CONF_IS_RT(TPP_HAVE_TPP_TOK_INT) */
#else /* TPP_HAVE_TPP_TOK_INT */
				tpp_assert(result == TPP_TOK_EOF || result == TPP_TOK_FLOAT);
				result = TPP_TOK_FLOAT;
#endif /* !TPP_HAVE_TPP_TOK_INT */
				goto set_result;
			} else
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
#if TPP_HAVE_TPP_TOK_INT && TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_FLOAT)
			if (tpp_lexer_has(self, TPP_TOK_INT)) {
				error = tpp_lexer_seek_end_of_int(self, &pos);
				if (TPP_ISERR(error))
					goto return_error;
				result = TPP_TOK_INT;
				goto set_result;
			} else
#endif /* TPP_HAVE_TPP_TOK_INT && TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_FLOAT) */
			{
			}
		}
#endif /* TPP_HAVE_TPP_TOK_INT || TPP_HAVE_TPP_TOK_FLOAT */

		/* Other trait-based character checks would go here... */
	}	break;

#if TPP_HAVE_ASSUME_ASCII_CTYPE
	default: break; /* Everything else is a single-character token */
#endif /* TPP_HAVE_ASSUME_ASCII_CTYPE */
#undef tpp_ascii_maybe_test
	}

	/* Fallback: single-character token */
	result = TPP_TOK_OFCHAR(ch);
	pos    = tpp_file_rel2ptr(file, rel_start + 1);
set_result:
	token->tt_id    = result;
	token->tt_start = tpp_file_rel2ptr(file, rel_start);
	*p_pos = pos; /* This also updates "file->tf_pos" (if "p_pos == &token->tt_end") */
	return result;
eof:
	/* Check if we can read some more data from the file */
	rel_start = tpp_file_ptr2rel(file, pos);
	error = tpp_file_expandchunk(file);
	if (TPP_ISERR(error))
		goto return_error;
	pos = tpp_file_rel2ptr(file, rel_start);
	end = file->tf_end;
	if (pos < end)
		goto again_read_from_pos;

	/* XXX: Warning if current file is a IO/TEXT file and doesn't end with a trailing linefeed */

	/* Check if we can pop to another file */
#if TPP_HAVE_INCLUDE_STACK
	if (file->tf_prev && p_pos == &file->tf_pos) {
		/* Warn if the file still has an active #ifdef-stack
		 * Only do this when we're actually going to pop the
		 * file off the #include-stack. In those cases where
		 * we're not allowed to do so, it's up to the caller
		 * to emit these sorts of warnings! */
#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
		error = tpp_lexer_warn_nonempty_ifdef(self);
		if (TPP_ISERR(error))
			goto return_error;
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */

		/* Actually pop the file! */
		tpp_lexer_popfile(self);
		goto again;
	}
#endif /* TPP_HAVE_INCLUDE_STACK */

	/* EOF reached */
	tpp_assert(pos == end);
	token->tt_id    = TPP_TOK_EOF;
	token->tt_start = pos;
	*p_pos = pos;
	return TPP_TOK_EOF;

return_error:
	/* Fix the caller's `p_pos' (unless it's the one from the file) */
	if (p_pos != &file->tf_pos)
		*p_pos = tpp_file_rel2ptr(file, rel_start);
	return TPP_TOK_OFERR(error);
#undef read_ch2
#undef warn_if_ch2_is_trigraph
#undef NEED_read_ch2
}

#undef NEED_tpp_lexer_seek_eol
#undef NEED_tpp_lexer_seek_end_of_string
#undef NEED_tpp_lexer_seek_end_of_block_string
#undef NEED_tpp_lexer_seek_end_of_cxx_raw_string
#undef NEED_tpp_lexer_seek_end_of_raw_string

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_YIELDRAW_C */
