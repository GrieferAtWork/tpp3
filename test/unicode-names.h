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

TPP_ASSERT_EXPANDS("\"foobar\"", __TPP_EVAL("foo\N{LATIN SMALL LETTER B}ar"))
TPP_ASSERT_EXPANDS("\"foobar\"", __TPP_EVAL("foo\N{  LATIN_SMALL   LETTER___B  }ar"))
TPP_ASSERT_EXPANDS("\"foobar\"", __TPP_EVAL("foo\N{LATIN SMALL LETTER B, LATIN SMALL LETTER A}r"))
TPP_ASSERT_EXPANDS("\"Happy 🎂\"", __TPP_EVAL("Happy \N{BIRTHDAY CAKE}"))
TPP_ASSERT_EXPANDS("\"🇩🇪\"", __TPP_EVAL("\N{flag: Germany}"))

/* Certain characters must be escaped when a string
 * must be re-encoded (see `tpp_token_encodestring()`)
 *
 * Here's the full list of all of them, and what they must be encoded as:
 * - Can't have NUL-characters (might confuse strlen() or similar...)
 * - Can't have ' or " characters (neither is allowed since the string
 *   encoder function is also used for the "#@"-operator)
 * - Any type of line-feed (both ASCII and Unicode ones)
 */
TPP_ASSERT_EXPANDS("\"\\000\"", __TPP_EVAL("\N{NULL}"))
TPP_ASSERT_EXPANDS("\"\\\"\"", __TPP_EVAL("\N{QUOTATION MARK}"))
TPP_ASSERT_EXPANDS("\"\\\'\"", __TPP_EVAL("\N{APOSTROPHE}"))
TPP_ASSERT_EXPANDS("\"\\r\"", __TPP_EVAL("\N{CR}"))
TPP_ASSERT_EXPANDS("\"\\n\"", __TPP_EVAL("\N{LF}"))
TPP_ASSERT_EXPANDS("\"\\302\\205\"", __TPP_EVAL("\N{NEXT LINE}"))
TPP_ASSERT_EXPANDS("\"\\302\\205\"", __TPP_EVAL("\N{NEL}")) /* Alias... */
TPP_ASSERT_EXPANDS("\"\\342\\200\\250\"", __TPP_EVAL("\N{LINE SEPARATOR}"))
TPP_ASSERT_EXPANDS("\"\\342\\200\\251\"", __TPP_EVAL("\N{PARAGRAPH SEPARATOR}"))

/* Also assert that trigraph sequences are escaped correctly */

/* During string-escape, trigraphs are always escaped by prefixing the first ? with \ */
TPP_ASSERT_EXPANDS("\"\\\??=\"", __TPP_EVAL("??" "="))
TPP_ASSERT_EXPANDS("\"\\\??=\"", __TPP_EVAL("\??="))
TPP_ASSERT_EXPANDS("\"\\\??=\"", __TPP_EVAL("?\?="))

/* However, only valid trigraph sequences are escaped (remember that `???` *isn't* a trigraph) */
TPP_ASSERT_EXPANDS("3", __TPP_EVAL(#"???"))
TPP_ASSERT_EXPANDS("\"???\"", __TPP_EVAL("???"))
TPP_ASSERT_EXPANDS("\"??a\"", __TPP_EVAL("??a"))

/* These cases are special edge-cases, since the encoder must be prepared
 * for anything to be written following a string-chunk that's being encoded
 * As such, when strings *end* with ?-characters, those characters still
 * need to be escaped */
TPP_ASSERT_EXPANDS("\"\\?\"", __TPP_EVAL("?"))
TPP_ASSERT_EXPANDS("\"?\\?\"", __TPP_EVAL("??"))

/* When the input string itself contains a trigraph *while* `__TPP_EVAL()`
 * reads its arguments, then that trigraph gets decoded *before* the output
 * strings gets encoded again (so in this case the fact that the output string
 * contains a '#' has nothing to do with the string encoder, but the fact that
 * the input string it is given contains a '#'-character) */
#pragma TPP extension(push, "-ftrigraphs")
TPP_ASSERT_WARNING_BEGIN("-Wtrigraphs")
TPP_ASSERT_EXPANDS("\"#\"", __TPP_EVAL("??="))
TPP_ASSERT_WARNING_END("-Wtrigraphs")
TPP_ASSERT_EXPANDS("\"#\"", __TPP_EVAL("#"))
#pragma TPP extension(pop)


/* Also test XML-style encoding */
TPP_ASSERT_EXPANDS("\"<\"", __TPP_EVAL("\&lt;"))
TPP_ASSERT_EXPANDS("\"<Specs say that some...\"", __TPP_EVAL("\&ltSpecs say that some..."))
TPP_ASSERT_EXPANDS("\">\"", __TPP_EVAL("\N{&gt;}"))
TPP_ASSERT_EXPANDS("\">...XML character don\\'t need a trailing `;`\"", __TPP_EVAL("\N{&gt}...XML character don't need a trailing `;`"))
TPP_ASSERT_EXPANDS("\"<But if one\\'s there...\"", __TPP_EVAL("\&lt;But if one's there..."))
TPP_ASSERT_EXPANDS("\">... then it\\'s always consumed\"", __TPP_EVAL("\N{&gt;}... then it's always consumed"))

/* Test the XML character with the longest name */
TPP_ASSERT_EXPANDS("\"∳\"", __TPP_EVAL("\&CounterClockwiseContourIntegral;"))
TPP_ASSERT_EXPANDS("\"∳\"", __TPP_EVAL("\N{&CounterClockwiseContourIntegral;}"))

