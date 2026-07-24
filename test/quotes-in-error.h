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

#pragma TPP extension(push)

/* Enable all different types of  */
#pragma TPP extension("-ftok-cxx-raw-string-literal")
#pragma TPP extension("-ftok-cxx-raw-char-literal")
#pragma TPP extension("-ftok-block-string-literal")
#pragma TPP extension("-ftok-block-char-literal")
#pragma TPP extension("-fno-character-literals")

TPP_ASSERT("foo\nbar" == R"(foo
bar)")

TPP_ASSERT("foo\nbar" == R'(foo
bar)')

TPP_ASSERT("foo\nbar" == """
	foo
	bar""")

TPP_ASSERT("foo\nbar" == '''
	foo
	bar''')

TPP_ASSERT_EXPANDS("BEFORE", BEFORE)
TPP_ASSERT(!defined(FIRST_BLOCK_ENTERED))
TPP_ASSERT(!defined(FIRST_BLOCK_LEFT))
TPP_ASSERT(!defined(SECOND_BLOCK_ENTERED))
TPP_ASSERT(!defined(SECOND_BLOCK_LEFT))

#if 0

#define FIRST_BLOCK_ENTERED

#error An incomplete " in a disabled #error directive
#error An incomplete """ in a disabled #error directive
#error An incomplete R"( in a disabled #error directive
#error An incomplete ' in a disabled #error directive
#error An incomplete ''' in a disabled #error directive
#error An incomplete R'( in a disabled #error directive
#error An incomplete /* in a disabled #error directive

#define FIRST_BLOCK_LEFT

#else

#define SECOND_BLOCK_ENTERED

#pragma TPP warning(push, "-Wno-user") /* Hide warnings */
#warning An incomplete " in an enabled #warning directive
#warning An incomplete """ in an enabled #warning directive
#warning An incomplete R"( in an enabled #warning directive
#warning An incomplete ' in an enabled #warning directive
#warning An incomplete ''' in an enabled #warning directive
#warning An incomplete R'( in an enabled #warning directive
#warning An incomplete /* in an enabled #warning directive
#pragma TPP warning(pop)

#define SECOND_BLOCK_LEFT

// Only to fix display in your editor (TPP doesn't need this):
// */ )' ''' )" """ */ )' ''' )" """

#endif

TPP_ASSERT(!defined(FIRST_BLOCK_ENTERED))
TPP_ASSERT(!defined(FIRST_BLOCK_LEFT))
TPP_ASSERT(defined(SECOND_BLOCK_ENTERED))
TPP_ASSERT(defined(SECOND_BLOCK_LEFT))

#undef SECOND_BLOCK_ENTERED
#undef SECOND_BLOCK_LEFT

TPP_ASSERT_EXPANDS("AFTER", AFTER)

#pragma TPP extension(pop)
