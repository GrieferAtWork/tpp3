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

TPP_ASSERT(!defined(IN_HEADER_FOO_H))
TPP_ASSERT(!defined(IN_HEADER_BAR_H))

/* With remapping turned off, nothing special should happen */
#pragma TPP extension("-fno-remap")
#include "misc/header/foo.h"
TPP_ASSERT(defined(IN_HEADER_FOO_H))
TPP_ASSERT(!defined(IN_HEADER_BAR_H))
#undef IN_HEADER_FOO_H

#include "misc/header/bar.h"
TPP_ASSERT(!defined(IN_HEADER_FOO_H))
TPP_ASSERT(defined(IN_HEADER_BAR_H))
#undef IN_HEADER_BAR_H

TPP_ASSERT(!__has_include("misc/what/is/this/override-from-parent-foo.h"))
TPP_ASSERT(!__has_include("misc/what/is/this/override-from-parent-bar.h"))
TPP_ASSERT(!__has_include("misc/header/baz.h"))
TPP_ASSERT(!__has_include("misc/header/BAZ.h"))
TPP_ASSERT(!__has_include("misc/header/NAZ.h"))
TPP_ASSERT(!__has_include("misc/header/subdir/parent-foo"))
TPP_ASSERT(!__has_include("misc/header/subdir/parent-bar"))

/* When remapping is turned on, includes should be remapped according to `header.gcc` files */
#pragma TPP extension("-fremap")

TPP_ASSERT(__has_include("misc/what/is/this/override-from-parent-foo.h"))
TPP_ASSERT(__has_include("misc/what/is/this/override-from-parent-bar.h"))
TPP_ASSERT(__has_include("misc/header/baz.h"))
TPP_ASSERT(__has_include("misc/header/BAZ.h"))
TPP_ASSERT(!__has_include("misc/header/NAZ.h")) /* This one's actually mapped to "non-existent-file.h", but that file doesn't exist */
TPP_ASSERT(__has_include("misc/header/subdir/parent-foo"))
TPP_ASSERT(__has_include("misc/header/subdir/parent-bar"))


/* Because of remapping, "foo.h" and "bar.h" have switched places! */
#include "misc/header/foo.h"
TPP_ASSERT(!defined(IN_HEADER_FOO_H))
TPP_ASSERT(defined(IN_HEADER_BAR_H))
#undef IN_HEADER_BAR_H

#include "misc/header/bar.h"
TPP_ASSERT(defined(IN_HEADER_FOO_H))
TPP_ASSERT(!defined(IN_HEADER_BAR_H))
#undef IN_HEADER_FOO_H

/* Because of remapping, "baz.h" points to "foo.h" */
#include "misc/header/baz.h"
TPP_ASSERT(defined(IN_HEADER_FOO_H))
TPP_ASSERT(!defined(IN_HEADER_BAR_H))
#undef IN_HEADER_FOO_H

/* Because of remapping, "BAZ.h" points to "bar.h"
 * Yes: using `header.gcc`, you can fake case-sensitive includes on windows! */
#include "misc/header/BAZ.h"
TPP_ASSERT(!defined(IN_HEADER_FOO_H))
TPP_ASSERT(defined(IN_HEADER_BAR_H))
#undef IN_HEADER_BAR_H

/* Duplicate slashes must still be handled as expected... */
#include "misc/header///BAZ.h"
TPP_ASSERT(!defined(IN_HEADER_FOO_H))
TPP_ASSERT(defined(IN_HEADER_BAR_H))
#undef IN_HEADER_BAR_H

#include "misc/what/is/this/override-from-parent-foo.h"
TPP_ASSERT(defined(IN_HEADER_FOO_H))
TPP_ASSERT(!defined(IN_HEADER_BAR_H))
#undef IN_HEADER_FOO_H

#include "misc/what/is/this/override-from-parent-bar.h"
TPP_ASSERT(!defined(IN_HEADER_FOO_H))
TPP_ASSERT(defined(IN_HEADER_BAR_H))
#undef IN_HEADER_BAR_H

/* ... however: duplicate slashes in the replaced parts aren't normalized.
 * A filename like "foo///bar.h" would require its own entry in some "header.gcc"
 * file, and won't automatically use the same entry as "foo/bar.h" */
TPP_ASSERT(__has_include("misc/what/is/this/override-from-parent-foo.h"))
TPP_ASSERT(__has_include("misc///what/is/this/override-from-parent-foo.h"))
TPP_ASSERT(!__has_include("misc///what///is/this/override-from-parent-foo.h"))
TPP_ASSERT(!__has_include("misc///what/is///this/override-from-parent-foo.h"))
TPP_ASSERT(!__has_include("misc///what/is/this///override-from-parent-foo.h"))


/* Include "foo.h" and "bar.h" via sub-directory "header.gcc" files, too.
 * Even in that case, their __FILE__ (which both those files assert to be
 * valid) must be normalized to not contain mention of the sub-directory */
#include "misc/header/subdir/parent-foo"
TPP_ASSERT(defined(IN_HEADER_FOO_H))
TPP_ASSERT(!defined(IN_HEADER_BAR_H))
#undef IN_HEADER_FOO_H
#include "misc/header/subdir/parent-bar"
TPP_ASSERT(!defined(IN_HEADER_FOO_H))
TPP_ASSERT(defined(IN_HEADER_BAR_H))
#undef IN_HEADER_BAR_H



TPP_ASSERT(!defined(IN_HEADER_FOO_H))
TPP_ASSERT(!defined(IN_HEADER_BAR_H))

#pragma TPP extension(pop)
