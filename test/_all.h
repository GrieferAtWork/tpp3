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
/*[[[deemon
import fs;
for (local e: fs.dir(".").sorted()) {
	if (e !in ["utils", "_all.h", "misc"])
		print(f'#include "{e}"');
}
]]]*/
#include "arguments-as-macro.h"
#include "big-character-literal.h"
#include "clone-linefeed.h"
#include "count-tokens.h"
#include "cxx-comments-in-macro.h"
#include "define-directives.h"
#include "directives-in-macros.h"
#include "directives-start-of-line.h"
#include "double-counter.h"
#include "glue.h"
#include "has-include.h"
#include "include-path.h"
#include "macro-argument-whitespace.h"
#include "macro-call-conv.h"
#include "macro-escape-lf.h"
#include "macro-expansion.h"
#include "magic-whitespace.h"
#include "null-directive.h"
#include "pound-xclaim.h"
#include "pragma-tpp_exec.h"
#include "prevent-macro-expansion.h"
#include "push-pop-macro.h"
#include "quotes-in-error.h"
#include "raw-string-bse.h"
#include "reserved-macro-name.h"
#include "self-redef.h"
#include "stdc-6.10.3.5_5.h"
#include "stdc-6.10.3.5_6.h"
#include "stdc-6.10.3.5_7.h"
#include "stdc-6.10.3.5_9.h"
#include "stdc-6.10.3_5+6.h"
#include "string-escape-invalid-utf-8.h"
#include "stringize.h"
#include "strings-in-expressions.h"
#include "traditional-macros.h"
#include "undef-current-macro.h"
#include "undef-macro-cexpr.h"
#include "unicode-names.h"
#include "unterminated-in-directive.h"
#include "user-error.h"
#include "varargs-empty.h"
#include "varargs-named.h"
#include "varargs-va_comma.h"
#include "varargs-va_nargs.h"
#include "varargs-va_opt.h"
/*[[[end]]]*/

