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
#include "utils/test.h"

/* Sanity test: valid utf-8 is kept as-is */
TPP_ASSERT_EXPANDS("\"ß\"", __TPP_EVAL("ß"))
TPP_ASSERT_EXPANDS("\"ß\"", __TPP_EVAL("\xC3\x9F"))
TPP_ASSERT_EXPANDS("\"ß\"", __TPP_EVAL("\u00DF"))
TPP_ASSERT_EXPANDS("\"ß\"", __TPP_EVAL("\N{LATIN SMALL LETTER SHARP S}"))
TPP_ASSERT_EXPANDS("\"\x7F\"", __TPP_EVAL("\u007F"))
TPP_ASSERT_EXPANDS("\"\xC2\x80\"", __TPP_EVAL("\u0080"))
TPP_ASSERT_EXPANDS("\"\xC2\x81\"", __TPP_EVAL("\u0081"))
TPP_ASSERT_EXPANDS("\"\xDF\xBF\"", __TPP_EVAL("\u07FF"))
TPP_ASSERT_EXPANDS("\"\xE0\xA0\x80\"", __TPP_EVAL("\u0800"))
TPP_ASSERT_EXPANDS("\"\xE0\xA0\x81\"", __TPP_EVAL("\u0801"))
TPP_ASSERT_EXPANDS("\"\xEF\xBF\xBF\"", __TPP_EVAL("\uFFFF"))
TPP_ASSERT_EXPANDS("\"\xF0\x90\x80\x80\"", __TPP_EVAL("\U00010000"))
TPP_ASSERT_EXPANDS("\"\xF4\x8F\xBF\xBD\"", __TPP_EVAL("\U0010FFFD"))
TPP_ASSERT_EXPANDS("\"\xF4\x8F\xBF\xBE\"", __TPP_EVAL("\U0010FFFE"))
TPP_ASSERT_EXPANDS("\"\xF4\x8F\xBF\xBF\"", __TPP_EVAL("\U0010FFFF"))
TPP_ASSERT_EXPANDS("\"\xF7\xBF\xBF\xBF\"", __TPP_EVAL("\U001FFFFF")) /* TPP allows all 5-byte utf-8 chars */

/* TPP can encode all of UCS4, but only accepts 5-byte utf-8 during parsing
 * (meaning that everything starting with the first 6-byte utf-8 character
 * gets escaped when re-encoded as a string) */
TPP_ASSERT_EXPANDS("\"\\370\\210\\200\\200\\200\"", __TPP_EVAL("\U00200000"))
TPP_ASSERT_EXPANDS("\"\\373\\277\\277\\277\\277\"", __TPP_EVAL("\U03FFFFFF"))
TPP_ASSERT_EXPANDS("\"\\374\\204\\200\\200\\200\\200\"", __TPP_EVAL("\U04000000"))
TPP_ASSERT_EXPANDS("\"\\374\\204\\200\\200\\200\\201\"", __TPP_EVAL("\U04000001"))
TPP_ASSERT_EXPANDS("\"\\375\\277\\277\\277\\277\\277\"", __TPP_EVAL("\U7FFFFFFF"))
TPP_ASSERT_EXPANDS("\"\\376\\202\\200\\200\\200\\200\\200\"", __TPP_EVAL("\U80000000"))
TPP_ASSERT_EXPANDS("\"\\376\\202\\200\\200\\200\\200\\201\"", __TPP_EVAL("\U80000001"))
TPP_ASSERT_EXPANDS("\"\\376\\203\\277\\277\\277\\277\\277\"", __TPP_EVAL("\UFFFFFFFF"))

/* Special case: unicode line-feeds must still be escaped as though they were invalid utf-8 */
TPP_ASSERT_EXPANDS("\"\\302\\205\"", __TPP_EVAL("\N{NEXT LINE}"))
TPP_ASSERT_EXPANDS("\"\\302\\205\"", __TPP_EVAL("\N{NEL}")) /* Alias... */
TPP_ASSERT_EXPANDS("\"\\342\\200\\250\"", __TPP_EVAL("\N{LINE SEPARATOR}"))
TPP_ASSERT_EXPANDS("\"\\342\\200\\251\"", __TPP_EVAL("\N{PARAGRAPH SEPARATOR}"))


/* Ensure that any invalid utf-8 is escaped during string escape */
TPP_ASSERT_EXPANDS("\"\\300\\000\"", __TPP_EVAL("\xC0\x00"))
TPP_ASSERT_EXPANDS("\"\\300\177\"",  __TPP_EVAL("\xC0\x7f")) /* [INVALID][VALID] */
TPP_ASSERT_EXPANDS("\"\\300\\200\"", __TPP_EVAL("\xC0\x80"))
TPP_ASSERT_EXPANDS("\"\\300\\377\"", __TPP_EVAL("\xC0\xff"))
TPP_ASSERT_EXPANDS("\"\\301\\000\"", __TPP_EVAL("\xC1\x00"))
TPP_ASSERT_EXPANDS("\"\\301\177\"",  __TPP_EVAL("\xC1\x7f")) /* [INVALID][VALID] */
TPP_ASSERT_EXPANDS("\"\\301\\200\"", __TPP_EVAL("\xC1\x80"))
TPP_ASSERT_EXPANDS("\"\\301\\377\"", __TPP_EVAL("\xC1\xff"))
TPP_ASSERT_EXPANDS("\"\\302\\000\"", __TPP_EVAL("\xC2\x00"))
TPP_ASSERT_EXPANDS("\"\\302\177\"",  __TPP_EVAL("\xC2\x7f")) /* [INVALID][VALID] */
TPP_ASSERT_EXPANDS("\"\302\200\"",   __TPP_EVAL("\xC2\x80")) /* [VALIDx2] */
TPP_ASSERT_EXPANDS("\"\\302\\377\"", __TPP_EVAL("\xC2\xff"))

TPP_ASSERT_EXPANDS("\"\\337\\000\"", __TPP_EVAL("\xDF\x00"))
TPP_ASSERT_EXPANDS("\"\\337\177\"",  __TPP_EVAL("\xDF\x7f")) /* [INVALID][VALID] */
TPP_ASSERT_EXPANDS("\"\337\200\"",   __TPP_EVAL("\xDF\x80")) /* [VALIDx2] */
TPP_ASSERT_EXPANDS("\"\\337\\377\"", __TPP_EVAL("\xDF\xff"))

TPP_ASSERT_EXPANDS("\"\\340\\237\\000\"", __TPP_EVAL("\xE0\x9F\x00"))
TPP_ASSERT_EXPANDS("\"\\340\\237\177\"",  __TPP_EVAL("\xE0\x9F\x7f")) /* [INVALIDx2][VALID] */
TPP_ASSERT_EXPANDS("\"\\340\\237\\200\"", __TPP_EVAL("\xE0\x9F\x80")) /* [INVALIDx3] */
TPP_ASSERT_EXPANDS("\"\\340\\237\\377\"", __TPP_EVAL("\xE0\x9F\xff"))
TPP_ASSERT_EXPANDS("\"\\340\\240\\000\"", __TPP_EVAL("\xE0\xA0\x00"))
TPP_ASSERT_EXPANDS("\"\\340\\240\177\"",  __TPP_EVAL("\xE0\xA0\x7f")) /* [INVALIDx2][VALID] */
TPP_ASSERT_EXPANDS("\"\340\240\200\"",    __TPP_EVAL("\xE0\xA0\x80")) /* [VALIDx3] */
TPP_ASSERT_EXPANDS("\"\\340\\240\\377\"", __TPP_EVAL("\xE0\xA0\xff"))
TPP_ASSERT_EXPANDS("\"\\357\\277\\000\"", __TPP_EVAL("\xEF\xBF\x00"))
TPP_ASSERT_EXPANDS("\"\\357\\277\177\"",  __TPP_EVAL("\xEF\xBF\x7f")) /* [INVALIDx2][VALID] */
TPP_ASSERT_EXPANDS("\"\357\277\200\"",    __TPP_EVAL("\xEF\xBF\x80")) /* [VALIDx3] */
TPP_ASSERT_EXPANDS("\"\\357\\277\\377\"", __TPP_EVAL("\xEF\xBF\xff"))
TPP_ASSERT_EXPANDS("\"\\357\\300\\000\"", __TPP_EVAL("\xEF\xC0\x00"))
TPP_ASSERT_EXPANDS("\"\\357\\300\177\"",  __TPP_EVAL("\xEF\xC0\x7f")) /* [INVALIDx2][VALID] */
TPP_ASSERT_EXPANDS("\"\\357\\300\\200\"", __TPP_EVAL("\xEF\xC0\x80")) /* [INVALIDx3] */
TPP_ASSERT_EXPANDS("\"\\357\\300\\377\"", __TPP_EVAL("\xEF\xC0\xff"))
TPP_ASSERT_EXPANDS("\"\\360\\277\\000\"", __TPP_EVAL("\xF0\xBF\x00"))
TPP_ASSERT_EXPANDS("\"\\360\\277\177\"",  __TPP_EVAL("\xF0\xBF\x7f"))
TPP_ASSERT_EXPANDS("\"\\360\\277\\200\"", __TPP_EVAL("\xF0\xBF\x80"))
TPP_ASSERT_EXPANDS("\"\\360\\277\\377\"", __TPP_EVAL("\xF0\xBF\xff"))
TPP_ASSERT_EXPANDS("\"\\360\\300\\000\"", __TPP_EVAL("\xF0\xC0\x00"))
TPP_ASSERT_EXPANDS("\"\\360\\300\177\"",  __TPP_EVAL("\xF0\xC0\x7f"))
TPP_ASSERT_EXPANDS("\"\\360\\300\\200\"", __TPP_EVAL("\xF0\xC0\x80"))
TPP_ASSERT_EXPANDS("\"\\360\\300\\377\"", __TPP_EVAL("\xF0\xC0\xff"))
TPP_ASSERT_EXPANDS("\"\\377\\277\\000\"", __TPP_EVAL("\xFF\xBF\x00"))
TPP_ASSERT_EXPANDS("\"\\377\\277\177\"",  __TPP_EVAL("\xFF\xBF\x7f"))
TPP_ASSERT_EXPANDS("\"\\377\\277\\200\"", __TPP_EVAL("\xFF\xBF\x80"))
TPP_ASSERT_EXPANDS("\"\\377\\277\\377\"", __TPP_EVAL("\xFF\xBF\xff"))
TPP_ASSERT_EXPANDS("\"\\377\\300\\000\"", __TPP_EVAL("\xFF\xC0\x00"))
TPP_ASSERT_EXPANDS("\"\\377\\300\177\"",  __TPP_EVAL("\xFF\xC0\x7f"))
TPP_ASSERT_EXPANDS("\"\\377\\300\\200\"", __TPP_EVAL("\xFF\xC0\x80"))
TPP_ASSERT_EXPANDS("\"\\377\\300\\377\"", __TPP_EVAL("\xFF\xC0\xff"))


TPP_ASSERT_EXPANDS("\"\\360\\217\177\\000\"",  __TPP_EVAL("\xF0\x8F\x7f\x00"))
TPP_ASSERT_EXPANDS("\"\\360\\217\177\177\"",   __TPP_EVAL("\xF0\x8F\x7f\x7f"))
TPP_ASSERT_EXPANDS("\"\\360\\217\177\\200\"",  __TPP_EVAL("\xF0\x8F\x7f\x80"))
TPP_ASSERT_EXPANDS("\"\\360\\217\177\\377\"",  __TPP_EVAL("\xF0\x8F\x7f\xff"))
TPP_ASSERT_EXPANDS("\"\\360\\217\\200\\000\"", __TPP_EVAL("\xF0\x8F\x80\x00"))
TPP_ASSERT_EXPANDS("\"\\360\\217\\200\177\"",  __TPP_EVAL("\xF0\x8F\x80\x7f"))
TPP_ASSERT_EXPANDS("\"\\360\\217\\200\\200\"", __TPP_EVAL("\xF0\x8F\x80\x80"))
TPP_ASSERT_EXPANDS("\"\\360\\217\\200\\377\"", __TPP_EVAL("\xF0\x8F\x80\xff"))
TPP_ASSERT_EXPANDS("\"\\360\\220\177\\000\"",  __TPP_EVAL("\xF0\x90\x7f\x00"))
TPP_ASSERT_EXPANDS("\"\\360\\220\177\177\"",   __TPP_EVAL("\xF0\x90\x7f\x7f"))
TPP_ASSERT_EXPANDS("\"\\360\\220\177\\200\"",  __TPP_EVAL("\xF0\x90\x7f\x80"))
TPP_ASSERT_EXPANDS("\"\\360\\220\177\\377\"",  __TPP_EVAL("\xF0\x90\x7f\xff"))
TPP_ASSERT_EXPANDS("\"\\360\\220\\200\\000\"", __TPP_EVAL("\xF0\x90\x80\x00"))
TPP_ASSERT_EXPANDS("\"\\360\\220\\200\177\"",  __TPP_EVAL("\xF0\x90\x80\x7f")) /* [INVALIDx3][VALID] */
TPP_ASSERT_EXPANDS("\"\360\220\200\200\"",     __TPP_EVAL("\xF0\x90\x80\x80")) /* [VALIDx4] */
TPP_ASSERT_EXPANDS("\"\\360\\220\\200\\377\"", __TPP_EVAL("\xF0\x90\x80\xff"))
TPP_ASSERT_EXPANDS("\"\\367\\277\\277\\000\"", __TPP_EVAL("\xF7\xBF\xBF\x00"))
TPP_ASSERT_EXPANDS("\"\\367\\277\\277\177\"",  __TPP_EVAL("\xF7\xBF\xBF\x7f")) /* [INVALIDx3][VALID] */
TPP_ASSERT_EXPANDS("\"\367\277\277\200\"",     __TPP_EVAL("\xF7\xBF\xBF\x80")) /* [VALIDx4] */
TPP_ASSERT_EXPANDS("\"\\367\\277\\277\\377\"", __TPP_EVAL("\xF7\xBF\xBF\xff"))
TPP_ASSERT_EXPANDS("\"\\370\\220\\200\\000\"", __TPP_EVAL("\xF8\x90\x80\x00"))
TPP_ASSERT_EXPANDS("\"\\370\\220\\200\177\"",  __TPP_EVAL("\xF8\x90\x80\x7f"))
TPP_ASSERT_EXPANDS("\"\\370\\220\\200\\200\"", __TPP_EVAL("\xF8\x90\x80\x80"))
TPP_ASSERT_EXPANDS("\"\\370\\220\\200\\377\"", __TPP_EVAL("\xF8\x90\x80\xff"))
TPP_ASSERT_EXPANDS("\"\\377\\220\\200\\000\"", __TPP_EVAL("\xFF\x90\x80\x00"))
TPP_ASSERT_EXPANDS("\"\\377\\220\\200\177\"",  __TPP_EVAL("\xFF\x90\x80\x7f"))
TPP_ASSERT_EXPANDS("\"\\377\\220\\200\\200\"", __TPP_EVAL("\xFF\x90\x80\x80"))
TPP_ASSERT_EXPANDS("\"\\377\\220\\200\\377\"", __TPP_EVAL("\xFF\x90\x80\xff"))

/* Assert that under-long 2-byte utf-8 sequences are escaped */
TPP_ASSERT_EXPANDS("\"\\300\\200\"", __TPP_EVAL("\xC0\x80")) /* U+0000 */
TPP_ASSERT_EXPANDS("\"\\300\\277\"", __TPP_EVAL("\xC0\xBF")) /* U+003F */
TPP_ASSERT_EXPANDS("\"\\301\\200\"", __TPP_EVAL("\xC1\x80")) /* U+0040 */
TPP_ASSERT_EXPANDS("\"\\301\\277\"", __TPP_EVAL("\xC1\xBF")) /* U+007F */
TPP_ASSERT_EXPANDS("\"\xC2\x80\"",   __TPP_EVAL("\xC2\x80")) /* U+0080 -- correct for 2-byte */
TPP_ASSERT_EXPANDS("\"\xDF\xBF\"",   __TPP_EVAL("\xDF\xBF")) /* U+07FF -- correct for 2-byte */

/* Assert that under-long 3-byte utf-8 sequences are escaped */
TPP_ASSERT_EXPANDS("\"\\340\\200\\200\"", __TPP_EVAL("\xE0\x80\x80")) /* U+0000 */
TPP_ASSERT_EXPANDS("\"\\340\\200\\277\"", __TPP_EVAL("\xE0\x80\xBF")) /* U+003F */
TPP_ASSERT_EXPANDS("\"\\340\\237\\200\"", __TPP_EVAL("\xE0\x9F\x80")) /* U+07C0 */
TPP_ASSERT_EXPANDS("\"\\340\\237\\277\"", __TPP_EVAL("\xE0\x9F\xBF")) /* U+07FF */
TPP_ASSERT_EXPANDS("\"\xE0\xA0\x80\"",    __TPP_EVAL("\xE0\xA0\x80")) /* U+0800 -- correct for 3-byte */
TPP_ASSERT_EXPANDS("\"\xEF\xBF\xBF\"",    __TPP_EVAL("\xEF\xBF\xBF")) /* U+FFFF -- correct for 3-byte */

/* Assert that under-long 4-byte utf-8 sequences are escaped */
TPP_ASSERT_EXPANDS("\"\\360\\200\\200\\200\"", __TPP_EVAL("\xF0\x80\x80\x80")) /* U+0000 */
TPP_ASSERT_EXPANDS("\"\\360\\217\\200\\200\"", __TPP_EVAL("\xF0\x8F\x80\x80")) /* U+F000 */
TPP_ASSERT_EXPANDS("\"\\360\\217\\200\\277\"", __TPP_EVAL("\xF0\x8F\x80\xBF")) /* U+F03F */
TPP_ASSERT_EXPANDS("\"\\360\\217\\277\\200\"", __TPP_EVAL("\xF0\x8F\xBF\x80")) /* U+FFC0 */
TPP_ASSERT_EXPANDS("\"\\360\\217\\277\\277\"", __TPP_EVAL("\xF0\x8F\xBF\xBF")) /* U+FFFF */
TPP_ASSERT_EXPANDS("\"\xF0\x90\x80\x80\"",     __TPP_EVAL("\xF0\x90\x80\x80")) /* U+10000 -- correct for 4-byte */
TPP_ASSERT_EXPANDS("\"\xF7\xBF\xBF\xBF\"",     __TPP_EVAL("\xF7\xBF\xBF\xBF")) /* U+1FFFFF -- correct for 4-byte */


/* Assert that incomplete utf-8 (where the last byte is missing) is escaped */
TPP_ASSERT_EXPANDS("\"\\300\"",    __TPP_EVAL("\xC0"))
TPP_ASSERT_EXPANDS("\"\\302\"",    __TPP_EVAL("\xC2"))
TPP_ASSERT_EXPANDS("\"\xC2\x80\"", __TPP_EVAL("\xC2\x80"))
TPP_ASSERT_EXPANDS("\"\\337\"",    __TPP_EVAL("\xDF"))
TPP_ASSERT_EXPANDS("\"\xDF\xBF\"", __TPP_EVAL("\xDF\xBF"))

TPP_ASSERT_EXPANDS("\"\\340\\240\"",      __TPP_EVAL("\xE0\xA0"))
TPP_ASSERT_EXPANDS("\"\xE0\xA0\x80\"",    __TPP_EVAL("\xE0\xA0\x80"))
TPP_ASSERT_EXPANDS("\"\\357\\277\"",      __TPP_EVAL("\xEF\xBF"))
TPP_ASSERT_EXPANDS("\"\xEF\xBF\xBF\"",    __TPP_EVAL("\xEF\xBF\xBF"))

TPP_ASSERT_EXPANDS("\"\\360\\220\\200\"",      __TPP_EVAL("\xF0\x90\x80"))
TPP_ASSERT_EXPANDS("\"\xF0\x90\x80\x80\"",     __TPP_EVAL("\xF0\x90\x80\x80"))
TPP_ASSERT_EXPANDS("\"\\367\\277\\277\"",      __TPP_EVAL("\xF7\xBF\xBF"))
TPP_ASSERT_EXPANDS("\"\xF7\xBF\xBF\xBF\"",     __TPP_EVAL("\xF7\xBF\xBF\xBF"))

TPP_ASSERT_EXPANDS("\"\\340\"",    __TPP_EVAL("\xE0"))
TPP_ASSERT_EXPANDS("\"\\357\"",    __TPP_EVAL("\xEF"))
TPP_ASSERT_EXPANDS("\"\\360\"",    __TPP_EVAL("\xF0"))
TPP_ASSERT_EXPANDS("\"\\367\"",    __TPP_EVAL("\xF7"))
TPP_ASSERT_EXPANDS("\"\\370\"",    __TPP_EVAL("\xF8"))
TPP_ASSERT_EXPANDS("\"\\377\"",    __TPP_EVAL("\xFF"))

/* Assert that any utf-8 continuation byte is escaped */
TPP_ASSERT_EXPANDS("\"\\200SUFFIX\"", __TPP_EVAL("\x80" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\201SUFFIX\"", __TPP_EVAL("\x81" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\207SUFFIX\"", __TPP_EVAL("\x87" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\210SUFFIX\"", __TPP_EVAL("\x88" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\211SUFFIX\"", __TPP_EVAL("\x89" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\217SUFFIX\"", __TPP_EVAL("\x8F" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\220SUFFIX\"", __TPP_EVAL("\x90" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\221SUFFIX\"", __TPP_EVAL("\x91" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\227SUFFIX\"", __TPP_EVAL("\x97" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\230SUFFIX\"", __TPP_EVAL("\x98" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\231SUFFIX\"", __TPP_EVAL("\x99" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\237SUFFIX\"", __TPP_EVAL("\x9F" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\240SUFFIX\"", __TPP_EVAL("\xA0" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\241SUFFIX\"", __TPP_EVAL("\xA1" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\247SUFFIX\"", __TPP_EVAL("\xA7" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\250SUFFIX\"", __TPP_EVAL("\xA8" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\251SUFFIX\"", __TPP_EVAL("\xA9" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\257SUFFIX\"", __TPP_EVAL("\xAF" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\260SUFFIX\"", __TPP_EVAL("\xB0" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\261SUFFIX\"", __TPP_EVAL("\xB1" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\267SUFFIX\"", __TPP_EVAL("\xB7" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\270SUFFIX\"", __TPP_EVAL("\xB8" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\271SUFFIX\"", __TPP_EVAL("\xB9" "SUFFIX"))
TPP_ASSERT_EXPANDS("\"\\277SUFFIX\"", __TPP_EVAL("\xBF" "SUFFIX"))
