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

/* GENERATED FILE -- DO NOT EDIT */

#ifndef GUARD_TPP_CTYPE_NAMES_C
#define GUARD_TPP_CTYPE_NAMES_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "ctype.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_UNICODE_BYNAME_LOOKUP
#ifndef tpp_unicode_byname_lookup
TPP_STATIC_ASSERT(TPP_UNICODE_BYNAME_LOOKUP_MAXUC == 10);
#define TPP_UNICODE_BYNAME_LOOKUP_MINTOKENS 1
#define TPP_UNICODE_BYNAME_LOOKUP_MAXTOKENS 19




// "CJK COMPATIBILITY IDEOGRAPH - "      U+F900, U+FA6D (366), f900, true
// "CJK COMPATIBILITY IDEOGRAPH - "      U+FA70, U+FAD9 (106), fa70, true
// "VARIATION SELECTOR - "      U+FE00, U+FE0F (16), 1, false
// "VS"      U+FE00, U+FE0F (16), 1, false
// "LINEAR B IDEOGRAM B"      U+100AC, U+100B4 (9), 164, false
// "LINEAR B IDEOGRAM B"      U+100B8, U+100C0 (9), 177, false
// "LINEAR B IDEOGRAM VESSEL B"      U+100DF, U+100F2 (20), 200, false
// "LINEAR A SIGN AB"      U+10600, U+1060A (11), 1, false
// "LINEAR A SIGN AB"      U+10625, U+1062C (8), 44, false
// "LINEAR A SIGN AB"      U+1062D, U+10635 (9), 53, false
// "LINEAR A SIGN AB"      U+1063D, U+10643 (7), 76, false
// "LINEAR A SIGN A"      U+10655, U+1065C (8), 301, true
// "LINEAR A SIGN A"      U+10666, U+1069F (58), 314, false
// "LINEAR A SIGN A"      U+106C3, U+106CC (10), 523, false
// "LINEAR A SIGN A"      U+106CD, U+106D5 (9), 534, false
// "LINEAR A SIGN A"      U+106D7, U+106E1 (11), 547, false
// "LINEAR A SIGN A"      U+106E7, U+106FC (22), 568, false
// "LINEAR A SIGN A"      U+10709, U+10719 (17), 608, false
// "LINEAR A SIGN A"      U+10729, U+10736 (14), 651, false
// "LINEAR A SIGN "      U+10760, U+10767 (8), a800, true
// "SIDETIC LETTER N"      U+10940, U+10959 (26), 1, false
// "CYPRO - MINOAN SIGN CM"      U+12F92, U+12F9A (9), 4, false
// "CYPRO - MINOAN SIGN CM"      U+12FA1, U+12FA8 (8), 23, false
// "CYPRO - MINOAN SIGN CM"      U+12FA9, U+12FB1 (9), 33, false
// "CYPRO - MINOAN SIGN CM"      U+12FB5, U+12FBC (8), 49, false
// "CYPRO - MINOAN SIGN CM"      U+12FBD, U+12FC3 (7), 58, false
// "CYPRO - MINOAN SIGN CM"      U+12FC4, U+12FCD (10), 66, false
// "CYPRO - MINOAN SIGN CM"      U+12FD0, U+12FDE (15), 78, false
// "CYPRO - MINOAN SIGN CM"      U+12FDF, U+12FEA (12), 94, false
// "EGYPTIAN HIEROGLYPH A"      U+13009, U+13010 (8), 7, false
// "EGYPTIAN HIEROGLYPH A"      U+13016, U+13024 (15), 18, false
// "EGYPTIAN HIEROGLYPH A"      U+13026, U+1302D (8), 33, false
// "EGYPTIAN HIEROGLYPH A"      U+13037, U+1304F (25), 46, false
// "EGYPTIAN HIEROGLYPH C"      U+1305F, U+13066 (8), 3, false
// "EGYPTIAN HIEROGLYPH C"      U+13068, U+13075 (14), 11, false
// "EGYPTIAN HIEROGLYPH D"      U+13076, U+1307D (8), 1, true
// "EGYPTIAN HIEROGLYPH D"      U+1307F, U+13091 (19), 9, false
// "EGYPTIAN HIEROGLYPH D"      U+1309C, U+130A7 (12), 35, false
// "EGYPTIAN HIEROGLYPH D"      U+130BD, U+130C9 (13), 55, false
// "EGYPTIAN HIEROGLYPH E"      U+130D2, U+130D9 (8), 1, true
// "EGYPTIAN HIEROGLYPH E"      U+130DD, U+130E3 (7), 10, true
// "EGYPTIAN HIEROGLYPH E"      U+130EB, U+130F2 (8), 21, true
// "EGYPTIAN HIEROGLYPH F"      U+13100, U+1310B (12), 2, false
// "EGYPTIAN HIEROGLYPH F"      U+1310D, U+13114 (8), 14, false
// "EGYPTIAN HIEROGLYPH F"      U+13116, U+1311F (10), 22, false
// "EGYPTIAN HIEROGLYPH F"      U+1312A, U+13130 (7), 39, false
// "EGYPTIAN HIEROGLYPH G"      U+1314E, U+13156 (9), 12, false
// "EGYPTIAN HIEROGLYPH G"      U+1315F, U+13168 (10), 27, false
// "EGYPTIAN HIEROGLYPH G"      U+13176, U+1317E (9), 46, false
// "EGYPTIAN HIEROGLYPH K"      U+1319B, U+131A2 (8), 1, true
// "EGYPTIAN HIEROGLYPH M"      U+131B3, U+131B9 (7), 4, false
// "EGYPTIAN HIEROGLYPH M"      U+131E3, U+131E9 (7), 34, false
// "EGYPTIAN HIEROGLYPH N"      U+131EF, U+13200 (18), 1, false
// "EGYPTIAN HIEROGLYPH N"      U+13203, U+13209 (7), 19, false
// "EGYPTIAN HIEROGLYPH N"      U+1320B, U+13212 (8), 26, false
// "EGYPTIAN HIEROGLYPH NL"      U+13226, U+13231 (12), 6, false
// "EGYPTIAN HIEROGLYPH NU"      U+13236, U+1323F (10), 1, false
// "EGYPTIAN HIEROGLYPH NU"      U+13243, U+13249 (7), 12, true
// "EGYPTIAN HIEROGLYPH O"      U+13265, U+1326D (9), 11, true
// "EGYPTIAN HIEROGLYPH O"      U+1328A, U+13297 (14), 37, false
// "EGYPTIAN HIEROGLYPH P"      U+132A0, U+132A7 (8), 4, false
// "EGYPTIAN HIEROGLYPH Q"      U+132A8, U+132AE (7), 1, true
// "EGYPTIAN HIEROGLYPH R"      U+132B5, U+132BB (7), 4, false
// "EGYPTIAN HIEROGLYPH R"      U+132C4, U+132D0 (13), 17, false
// "EGYPTIAN HIEROGLYPH S"      U+132D9, U+132E0 (8), 7, false
// "EGYPTIAN HIEROGLYPH S"      U+132E7, U+132EF (9), 18, false
// "EGYPTIAN HIEROGLYPH S"      U+132F2, U+132FA (9), 27, false
// "EGYPTIAN HIEROGLYPH S"      U+132FC, U+13306 (11), 36, false
// "EGYPTIAN HIEROGLYPH T"      U+1331D, U+1332C (16), 17, false
// "EGYPTIAN HIEROGLYPH U"      U+1333B, U+1334B (17), 7, false
// "EGYPTIAN HIEROGLYPH U"      U+13358, U+13361 (10), 33, false
// "EGYPTIAN HIEROGLYPH V"      U+1337F, U+13386 (8), 13, false
// "EGYPTIAN HIEROGLYPH Y"      U+133DD, U+133E3 (7), 2, true
// "EGYPTIAN HIEROGLYPH Z"      U+133F1, U+133FA (10), 6, false
// "EGYPTIAN HIEROGLYPH AA"      U+1340D, U+13413 (7), 1, true
// "EGYPTIAN HIEROGLYPH AA"      U+13416, U+1342E (25), 8, false
// "EGYPTIAN HIEROGLYPH - "      U+13460, U+143FA (3995), 13460, true
// "ANATOLIAN HIEROGLYPH A"      U+14400, U+14409 (10), 1, false
// "ANATOLIAN HIEROGLYPH A"      U+1440B, U+1441A (16), 11, false
// "ANATOLIAN HIEROGLYPH A"      U+1441C, U+14428 (13), 27, false
// "ANATOLIAN HIEROGLYPH A"      U+14435, U+14448 (20), 47, false
// "ANATOLIAN HIEROGLYPH A"      U+1444C, U+1446A (31), 67, false
// "ANATOLIAN HIEROGLYPH A"      U+1448D, U+14496 (10), 116, false
// "ANATOLIAN HIEROGLYPH A"      U+14498, U+144A1 (10), 126, false
// "ANATOLIAN HIEROGLYPH A"      U+144A3, U+144E5 (67), 136, false
// "ANATOLIAN HIEROGLYPH A"      U+144FA, U+14504 (11), 217, false
// "ANATOLIAN HIEROGLYPH A"      U+14506, U+1452D (40), 228, false
// "ANATOLIAN HIEROGLYPH A"      U+1452F, U+14544 (22), 268, false
// "ANATOLIAN HIEROGLYPH A"      U+14552, U+1455B (10), 300, true
// "ANATOLIAN HIEROGLYPH A"      U+1455D, U+14570 (20), 310, false
// "ANATOLIAN HIEROGLYPH A"      U+1457E, U+14594 (23), 337, false
// "ANATOLIAN HIEROGLYPH A"      U+145A5, U+145AE (10), 372, false
// "ANATOLIAN HIEROGLYPH A"      U+145BE, U+145CD (16), 394, false
// "ANATOLIAN HIEROGLYPH A"      U+145D0, U+145F7 (40), 411, false
// "ANATOLIAN HIEROGLYPH A"      U+145F9, U+145FF (7), 451, true
// "ANATOLIAN HIEROGLYPH A"      U+14601, U+14628 (40), 458, false
// "ANATOLIAN HIEROGLYPH A"      U+14629, U+14646 (30), 501, false
// "MIAO LETTER TONE - "      U+16F93, U+16F99 (7), 2, true
// "TANGUT COMPONENT - "      U+18800, U+18AFF (768), 1, false
// "KHITAN SMALL SCRIPT CHARACTER - "      U+18B00, U+18CD5 (470), 18b00, true
// "TANGUT COMPONENT - "      U+18D80, U+18DF2 (115), 769, false
// "HENTAIGANA LETTER KA - "      U+1B017, U+1B021 (11), 1, false
// "HENTAIGANA LETTER KI - "      U+1B023, U+1B02A (8), 1, true
// "HENTAIGANA LETTER KU - "      U+1B02B, U+1B031 (7), 1, true
// "HENTAIGANA LETTER SA - "      U+1B03C, U+1B043 (8), 1, true
// "HENTAIGANA LETTER SU - "      U+1B04A, U+1B051 (8), 1, true
// "HENTAIGANA LETTER SO - "      U+1B057, U+1B05D (7), 1, true
// "HENTAIGANA LETTER TI - "      U+1B062, U+1B068 (7), 1, true
// "HENTAIGANA LETTER TE - "      U+1B06E, U+1B076 (9), 1, true
// "HENTAIGANA LETTER NA - "      U+1B07E, U+1B086 (9), 1, true
// "HENTAIGANA LETTER NI - "      U+1B087, U+1B08D (7), 1, true
// "HENTAIGANA LETTER HA - "      U+1B09E, U+1B0A8 (11), 1, false
// "HENTAIGANA LETTER HI - "      U+1B0A9, U+1B0AF (7), 1, true
// "HENTAIGANA LETTER HE - "      U+1B0B3, U+1B0B9 (7), 1, true
// "HENTAIGANA LETTER HO - "      U+1B0BA, U+1B0C1 (8), 1, true
// "HENTAIGANA LETTER MA - "      U+1B0C2, U+1B0C8 (7), 1, true
// "HENTAIGANA LETTER MI - "      U+1B0C9, U+1B0CF (7), 1, true
// "HENTAIGANA LETTER RI - "      U+1B0F1, U+1B0F7 (7), 1, true
// "HENTAIGANA LETTER WO - "      U+1B116, U+1B11C (7), 1, true
// "NUSHU CHARACTER - "      U+1B170, U+1B2FB (396), 1b170, true
// "MUSICAL SYMBOL ORNAMENT STROKE - "      U+1D19B, U+1D1A5 (11), 1, false
// "GREEK VOCAL NOTATION SYMBOL - "      U+1D200, U+1D217 (24), 1, false
// "GREEK INSTRUMENTAL NOTATION SYMBOL - "      U+1D23A, U+1D241 (8), 47, false
// "SIGNWRITING LIMB LENGTH - "      U+1DA77, U+1DA7D (7), 1, true
// "SIGNWRITING ROTATION MODIFIER - "      U+1DAA1, U+1DAAF (15), 2, false
// "DOMINO TILE HORIZONTAL - 00 - "      U+1F031, U+1F037 (7), 0, true
// "DOMINO TILE HORIZONTAL - 01 - "      U+1F038, U+1F03E (7), 0, true
// "DOMINO TILE HORIZONTAL - 02 - "      U+1F03F, U+1F045 (7), 0, true
// "DOMINO TILE HORIZONTAL - 03 - "      U+1F046, U+1F04C (7), 0, true
// "DOMINO TILE HORIZONTAL - 04 - "      U+1F04D, U+1F053 (7), 0, true
// "DOMINO TILE HORIZONTAL - 05 - "      U+1F054, U+1F05A (7), 0, true
// "DOMINO TILE HORIZONTAL - 06 - "      U+1F05B, U+1F061 (7), 0, true
// "DOMINO TILE VERTICAL - 00 - "      U+1F063, U+1F069 (7), 0, true
// "DOMINO TILE VERTICAL - 01 - "      U+1F06A, U+1F070 (7), 0, true
// "DOMINO TILE VERTICAL - 02 - "      U+1F071, U+1F077 (7), 0, true
// "DOMINO TILE VERTICAL - 03 - "      U+1F078, U+1F07E (7), 0, true
// "DOMINO TILE VERTICAL - 04 - "      U+1F07F, U+1F085 (7), 0, true
// "DOMINO TILE VERTICAL - 05 - "      U+1F086, U+1F08C (7), 0, true
// "DOMINO TILE VERTICAL - 06 - "      U+1F08D, U+1F093 (7), 0, true
// "PLAYING CARD TRUMP - "      U+1F0E1, U+1F0F5 (21), 1, false
// "CJK COMPATIBILITY IDEOGRAPH - "      U+2F800, U+2FA1D (542), 2f800, true
// "VARIATION SELECTOR - "      U+E0100, U+E01EF (240), 17, false
// "VS"      U+E0100, U+E01EF (240), 17, false
#endif /* !tpp_unicode_byname_lookup */
#endif /* TPP_HAVE_UNICODE_BYNAME_LOOKUP */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CTYPE_NAMES_C */
