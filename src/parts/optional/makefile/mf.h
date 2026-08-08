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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_MF_H
#define GUARD_TPP_OPTIONAL_MAKEFILE_MF_H 1

#include "api.h"

#include "config.h"
#include "mf-features.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#undef TPP_MAKEFILE_HAVE_FLAGS
#if 0 /* TODO */
#define TPP_MAKEFILE_HAVE_FLAGS 1
#else /* ... */
#define TPP_MAKEFILE_HAVE_FLAGS 0
#endif /* !... */

#if TPP_MAKEFILE_HAVE_FLAGS
#define tpp_makefile_flags uint_least8_t
#define TPP_MAKEFILE_FLAG_NORMAL UINT8_C(0x00)
#endif /* TPP_MAKEFILE_HAVE_FLAGS */


typedef struct tpp_makefile {
#if TPP_HAVE_HOOK_COOKIES && !defined(TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER)
	tpp_lexer            *TPP_MAKEFILE_INTERNAL(tmf_lexer);  /* [1..1][const] The lexer whose tokens are being emitted */
#define _tpp_makefile_init_lexer(self, lexer) (self)->TPP_MAKEFILE_INTERNAL(tmf_lexer) = (lexer)
#else /* TPP_HAVE_HOOK_COOKIES && !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#define _tpp_makefile_init_lexer(self, lexer) tpp_assert(tpp_makefile_getlexer(self) == (lexer))
#endif /* !TPP_HAVE_HOOK_COOKIES || TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
	tpp_formatprinter     TPP_MAKEFILE_INTERNAL(tmf_output); /* [1..1][const] Makefile output printer (the makefile itself will be passed as argument) */
#if TPP_MAKEFILE_HAVE_FEATURES
	tpp_makefile_features TPP_MAKEFILE_INTERNAL(tmf_feat);   /* Makefile feature configuration */
#define _tpp_makefile_init_feat(self) , tpp_makefile_features_init(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat))
#define _tpp_makefile_fini_feat(self) , tpp_makefile_features_fini(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat))
#else /* TPP_MAKEFILE_HAVE_FEATURES */
#define _tpp_makefile_init_feat(self) /* nothing */
#define _tpp_makefile_fini_feat(self) /* nothing */
#endif /* !TPP_MAKEFILE_HAVE_FEATURES */
} tpp_makefile;

/* Initialize (after `tpp_lexer_init()` was called) or finalize
 * (before `tpp_lexer_fini()` is called) a given makefile.
 *
 * @param: lexer:  The lexer whose tokens are being emitted
 * @param: output: Default output printer. On error, must return one of `TPP_SSIZE_OFERR(*)` */
#define tpp_makefile_init(self, lexer, output)                  \
	(void)(_tpp_makefile_init_lexer(self, lexer),               \
	       (self)->TPP_MAKEFILE_INTERNAL(tmf_output) = (output) \
	       _tpp_makefile_init_feat(self))
#define tpp_makefile_fini(self) \
	tpp_dbg_memset(self, sizeof(tpp_makefile))

/* Retrieve components of the makefile. */
#define tpp_makefile_getoutput(self) (self)->TPP_MAKEFILE_INTERNAL(tmf_output)
#if TPP_HAVE_HOOK_COOKIES && !defined(TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER)
#define tpp_makefile_getlexer(self)   ((self)->TPP_MAKEFILE_INTERNAL(tmf_lexer))
#define tpp_makefile_ofcookie(cookie) ((tpp_makefile *)(cookie))
#else /* TPP_HAVE_HOOK_COOKIES && !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#ifndef TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER
#error "Invalid configuration: under '-DTPP_HAVE_HOOK_COOKIES=0' you must specify a macro '#define TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER (offsetof(MY_CONTAINER, makefile) - offsetof(MY_CONTAINER, lexer))' to specify how to retrieve the makefile from a lexer"
#endif /* !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#define tpp_makefile_getlexer(self)   ((tpp_lexer *)((char *)(self) - TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER))
#define tpp_makefile_ofcookie(cookie) ((tpp_makefile *)((char *)(self) + TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER))
#endif /* !TPP_HAVE_HOOK_COOKIES || TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */

/* Helpers for quickly printing stuff to the makefile's output.
 * WARNING: Careless use of these functions may result in the makefile's
 *          idea of its current output file/line/column becoming invalid. */
#define tpp_makefile_output_printraw(self, text, len) \
	tpp_formatprinter_print(tpp_makefile_getoutput(self), self, text, len)
#define tpp_makefile_output_printraw_cstr(self, text, len) \
	tpp_formatprinter_print_cstr(tpp_makefile_getoutput(self), self, text, len)
#define tpp_makefile_output_printraw_conststr(self, CONSTstr) \
	tpp_formatprinter_print_conststr(tpp_makefile_getoutput(self), self, CONSTstr)

/* Check if a runtime-configurable config option `conf` in `TPP_MAKEFILE_HAVE_<conf>` is currently enabled.
 * When `TPP_MAKEFILE_HAVE_<conf>` is configured as `TPP_CONF_ISCONST()`, return that constant instead. */
#define tpp_makefile_has(self, conf) _tpp_makefile_has_##conf(self)

/* Features... */
#if TPP_MAKEFILE_HAVE_FEATURES
#define tpp_makefile_getfeature(self, TPP_MAKEFILE_FEAT_x)          tpp_makefile_features_getid(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat), TPP_MAKEFILE_FEAT_x)
#define tpp_makefile_setfeature(self, TPP_MAKEFILE_FEAT_x, enabled) tpp_makefile_features_setid(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat), TPP_MAKEFILE_FEAT_x, enabled)
#define tpp_makefile_enablefeature(self, TPP_MAKEFILE_FEAT_x)       tpp_makefile_features_enable(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat), TPP_MAKEFILE_FEAT_x)
#define tpp_makefile_disablefeature(self, TPP_MAKEFILE_FEAT_x)      tpp_makefile_features_disable(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat), TPP_MAKEFILE_FEAT_x)
#define tpp_makefile_resetfeatures(self)                            tpp_makefile_features_reset(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat))
#else /* TPP_MAKEFILE_HAVE_FEATURES */
#define tpp_makefile_getfeature(self, TPP_MAKEFILE_FEAT_x) 0
#define tpp_makefile_resetfeatures(self)                   (void)0
#endif /* !TPP_MAKEFILE_HAVE_FEATURES */

/* TODO: API to enable/disable the makefile's callback into the lexer's `NEW_DEPENDENCY` hook */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_H */
