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
#ifndef GUARD_TPP_OPTIONAL_EMITTER_EMITTER_IO_H
#define GUARD_TPP_OPTIONAL_EMITTER_EMITTER_IO_H 1

#include "api.h"

#include "config.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_EMITTER_HAVE_IO_PRINTPWD
#ifndef tpp_io_printpwd
/* Print the hosting process's current-working-directory ($PWD,
 * aka `getcwd()`) to the given `printer`. Needed primarily to
 * implement `TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY`
 *
 * @return: * :  Sum of positive return values of `printer`
 * @return: < 0: First negative return value of `printer`
 * @return: TPP_SSIZE_OFERR(TPP_EIO):    I/O error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM): Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_io_printpwd(tpp_formatprinter printer, void *arg);
#endif /* !tpp_io_printpwd */
#endif /* TPP_EMITTER_HAVE_IO_PRINTPWD */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EMITTER_IO_H */
