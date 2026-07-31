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


TPP_ASSERT_EXPANDS("105,95,97,109,95,98,105,110,46,100,97,116",
#embed "misc/bin.dat"
)
TPP_ASSERT_EXPANDS("",
#embed "misc/blank.dat"
)

/* What a perfect place to show off `__TPP_EXEC()`! */
#define embed(x) __TPP_EXEC("#embed " #x)
TPP_ASSERT_EXPANDS("105,95,97,109,95,98,105,110,46,100,97,116", embed("misc/bin.dat"))
TPP_ASSERT_EXPANDS("105,95,97,109,95,98,105", embed("misc/bin.dat" limit(7)))
TPP_ASSERT_EXPANDS("105,95,97,109,95,98,105,110,46,100,97,116", embed("misc/bin.dat" offset(0)))
TPP_ASSERT_EXPANDS("95,97,109,95,98,105,110,46,100,97,116", embed("misc/bin.dat" offset(1)))
TPP_ASSERT_EXPANDS("116", embed("misc/bin.dat" offset(11)))
TPP_ASSERT_EXPANDS("97", embed("misc/bin.dat" offset(10) limit(1)))
TPP_ASSERT_EXPANDS("97", embed("misc/bin.dat" limit(1) offset(10)))
TPP_ASSERT_EXPANDS("", embed("misc/blank.dat"))
#undef embed

TPP_ASSERT_EXPANDS("\"i_am_bin.dat\"", __TPP_STR_PACK(
#embed "misc/bin.dat" if_empty("WRONG")
))

TPP_ASSERT_EXPANDS("\"t\"", __TPP_STR_PACK(
#embed "misc/bin.dat" offset(11) if_empty("WRONG")
))

TPP_ASSERT_EXPANDS("\"NOW_IM_EMPTY\"", __TPP_STR_PACK(
#embed "misc/bin.dat" offset(12) if_empty("NOW_IM_EMPTY")
))

#define LIMIT 10
TPP_ASSERT_EXPANDS("\"i_am_bin.d\"", __TPP_STR_PACK(
#embed "misc/bin.dat" limit(LIMIT) if_empty("WRONG")
))

#define MY_LIMIT(x) __limit__(x)
TPP_ASSERT_EXPANDS("\"i_am_bin.\"", __TPP_STR_PACK(
#embed "misc/bin.dat" MY_LIMIT(LIMIT - 1) if_empty("WRONG")
))
#undef MY_LIMIT
#undef LIMIT

#define limit     WHAT_NOW /* Ooops -- can't say that one */
#define __limit__ WHAT_NOW /* Ooops -- can't say that one */
#define MY_LIMIT ___limit_(7) /* TPP Actually accepts any number of leading/trailing _ */
TPP_ASSERT_EXPANDS("\"i_am_bi\"", __TPP_STR_PACK(
#embed "misc/bin.dat" MY_LIMIT if_empty("WRONG")
))
#undef MY_LIMIT
#undef __limit__
#undef limit

TPP_ASSERT_EXPANDS("\"\"", __TPP_STR_PACK(
#embed "misc/blank.dat"
))

TPP_ASSERT_EXPANDS("\"IAM EMPTY\"", __TPP_STR_PACK(
#embed "misc/blank.dat" if_empty("I", "AM EMPTY")
))
TPP_ASSERT_EXPANDS("\"i_am_bin.dat\"", __TPP_STR_PACK(
#embed "misc/bin.dat" if_empty("I", "AM EMPTY")
))


TPP_ASSERT_WARNING_BEGIN("-Wenviron") /* no such file: `"misc/missing.dat"` */
TPP_ASSERT_EXPANDS("\"\"", __TPP_STR_PACK(
#embed "misc/missing.dat"
))
TPP_ASSERT_WARNING_END("-Wenviron")


TPP_ASSERT_EXPANDS("\"PREFIX2i_aSUFFIX\"", __TPP_STR_PACK(
#embed "misc/bin.dat" limit___(3) \
	prefix("IGNORED") suffix("SUFFIX") \
	prefix("PREFIX2") if_empty("but i'm not")
))

TPP_ASSERT_EXPANDS("\"but i am\"", __TPP_STR_PACK(
#embed "misc/blank.dat" limit___(3) \
	prefix("IGNORED") suffix("SUFFIX") \
	prefix("PREFIX2") if_empty("but i am")
))



TPP_ASSERT_EXPANDS("0", __has_embed("misc/missing.dat"))
TPP_ASSERT_EXPANDS("0", __has_embed("misc/missing.dat" limit(0)))
TPP_ASSERT_EXPANDS("0", __has_embed("misc/missing.dat" offset(1)))
TPP_ASSERT_EXPANDS("1", __has_embed("misc/bin.dat"))
TPP_ASSERT_EXPANDS("2", __has_embed("misc/bin.dat" limit(0)))
TPP_ASSERT_EXPANDS("1", __has_embed("misc/bin.dat" offset(1)))
TPP_ASSERT_EXPANDS("1", __has_embed("misc/bin.dat" offset(11)))
TPP_ASSERT_EXPANDS("2", __has_embed("misc/bin.dat" offset(12)))
TPP_ASSERT_EXPANDS("2", __has_embed("misc/blank.dat"))
TPP_ASSERT_EXPANDS("2", __has_embed("misc/blank.dat" limit(0)))
TPP_ASSERT_EXPANDS("2", __has_embed("misc/blank.dat" offset(1)))
TPP_ASSERT_EXPANDS("0", __STDC_EMBED_NOT_FOUND__)
TPP_ASSERT_EXPANDS("1", __STDC_EMBED_FOUND__)
TPP_ASSERT_EXPANDS("2", __STDC_EMBED_EMPTY__)

/* TPP also recognizes `gnu::` and `clang::` namespace-prefixed `offset` parameters */
TPP_ASSERT_EXPANDS("1", __has_embed("misc/bin.dat" gnu::offset(11)))
TPP_ASSERT_EXPANDS("2", __has_embed("misc/bin.dat" gnu::offset(12)))
TPP_ASSERT_EXPANDS("1", __has_embed("misc/bin.dat" clang::offset(11)))
TPP_ASSERT_EXPANDS("2", __has_embed("misc/bin.dat" clang::offset(12)))
TPP_ASSERT_EXPANDS("1", __has_embed("misc/bin.dat" __gnu_::__offset_(11)))
TPP_ASSERT_EXPANDS("2", __has_embed("misc/bin.dat" __gnu_::__offset_(12)))
TPP_ASSERT_EXPANDS("1", __has_embed("misc/bin.dat" __clang_::__offset_(11)))
TPP_ASSERT_EXPANDS("2", __has_embed("misc/bin.dat" __clang_::__offset_(12)))

TPP_ASSERT_EXPANDS("0", __has_include("misc/missing.dat"))
TPP_ASSERT_EXPANDS("1", __has_include("misc/bin.dat"))
TPP_ASSERT_EXPANDS("1", __has_include("misc/blank.dat"))

/* TPP also implements `__cpp_pp_embed` with correct expansions based on support-level */
#pragma extension(push)
TPP_ASSERT_EXPANDS("202606", __cpp_pp_embed)
#pragma extension("-fno-embed-directives-offset")
TPP_ASSERT_EXPANDS("202502", __cpp_pp_embed)
#pragma extension("-fno-embed-directives")
TPP_ASSERT_EXPANDS("__cpp_pp_embed", __cpp_pp_embed)
#pragma extension(pop)



TPP_ASSERT_WARNING_BEGIN("-Wenviron") /* no such file: `<misc/bin.dat>` */
TPP_ASSERT_EXPANDS("\"NOT_FOUND\"", __TPP_STR_PACK(
#embed <misc/bin.dat> if_empty("NOT_FOUND")
))
TPP_ASSERT_WARNING_END("-Wenviron")

/* Add directory containing the current file to `#embed`-path.
 * Then, trying to open `<misc/bin.dat>` must become possible. */
#pragma TPP include_path(push, embed: ".")
TPP_ASSERT_EXPANDS("\"i_am_bin.dat\"", __TPP_STR_PACK(
#embed <misc/bin.dat> if_empty("NOT_FOUND")
))
#pragma TPP include_path(pop)

