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

#pragma once

#ifdef WAS_HERE
#error "Shouldn't have gotten here more than once!"
#endif
#define WAS_HERE

/* include ourselves with some really abstract path.
 * #pragma once will prevent infinite recursion */
#include "include-path.h"
#include "./include-path.h"


/* Sanity check: last 14 characters of __FILE__ should be "include-path.h" */
TPP_ASSERT(__FILE__[-14:] == "include-path.h")

/* NOTE: These here actually only work if you invoke TPP such that
 *       it knowns that our current file is inside a folder "test":
 *       >> tpp test/include-path.h          #  OK
 *       >> cd test && tpp include-path.h    #  Test would fail without __FILE__-check
 */
#if __FILE__[-19:-15] == "test"
#include "./../test/include-path.h"
#include "./../test/include-path.h"
#include "./../test/./missing_folder/../include-path.h"
#endif

#undef WAS_HERE
