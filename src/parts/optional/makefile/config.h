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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_CONFIG_H
#define GUARD_TPP_OPTIONAL_MAKEFILE_CONFIG_H 1

#include "api.h"

/*[[[tpp-begin]]]*/

/* When enabled, only include dependencies if
 * `#include`-stack doesn't contain any system
 * headers */
#ifndef TPP_MAKEFILE_HAVE_USER_DEPENDENCIES
#define TPP_MAKEFILE_HAVE_USER_DEPENDENCIES (TPP_CONF_FEAT0)
#endif /* !TPP_MAKEFILE_HAVE_USER_DEPENDENCIES */


/************************************************************************/
/* MAKEFILE CLI CONFIG                                                  */
/************************************************************************/

/* Enable support for `tpp_makefile_cli_loader` */
#ifndef TPP_MAKEFILE_HAVE_CLI
#define TPP_MAKEFILE_HAVE_CLI TPP_HAVE_CLI
#endif /* !TPP_MAKEFILE_HAVE_CLI */

/* Enable support for `tpp_makefile_cli_loader_help`, which exposes a small
 * database of supported commandline flags in a human-readable format that
 * can also be rendered (fairly) easily. */
#ifndef TPP_MAKEFILE_HAVE_CLI_HELP
#define TPP_MAKEFILE_HAVE_CLI_HELP (TPP_HAVE_PROFILE_ALL && TPP_MAKEFILE_HAVE_CLI)
#endif /* !TPP_MAKEFILE_HAVE_CLI_HELP */

/* Include extra spellings (i.e.: in addition to the primary spelling) of CLI options. */
#ifndef TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS
#define TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS (TPP_MAKEFILE_HAVE_CLI_HELP && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS */

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_CONFIG_H */
