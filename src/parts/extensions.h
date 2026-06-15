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
#ifndef GUARD_TPP_EXTENSIONS_H
#define GUARD_TPP_EXTENSIONS_H 1

#include "api.h"
#include "config.h"
#include "error.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_EXTENSIONS
typedef enum tpp_extension_id {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) id,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	TPP_EXT_COUNT
} tpp_extension_id;

/* Default extension state */
typedef union tpp_extensions_state {
	struct {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) unsigned int tef_##id: 1;
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	} tes_flags; /* Consider this one as "TPP_INTERNAL", too! */
	unsigned char TPP_INTERNAL(tes_bitset)[TPP_EXT_COUNT ? ((TPP_EXT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];
} tpp_extensions_state;
TPP_CONST_DECL tpp_extensions_state const tpp_extensions_state_default;

#define tpp_extensions_state_getid(self, id) \
	((self)->TPP_INTERNAL(tes_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_extensions_state_get(self, id) _tpp_extensions_state_get_##id(self)
#define tpp_extensions_state_enable(self, id) \
	(void)((self)->TPP_INTERNAL(tes_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_extensions_state_disable(self, id) \
	(void)((self)->TPP_INTERNAL(tes_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_extensions_state_setid(self, id, enabled) \
	((enabled) ? tpp_extensions_state_enable(self, id) : tpp_extensions_state_disable(self, id))


typedef struct tpp_extensions {
	tpp_extensions_state   TPP_INTERNAL(te_state);   /* [const_if(te_pushcnt > 0)] Enabled-extensions state */
#if TPP_HAVE_EXTENSIONS_PUSH_POP
	tpp_size               TPP_INTERNAL(te_pushcnt); /* # of times extensions were pushed since last modified */
	struct tpp_extensions *TPP_INTERNAL(te_prev);    /* [0..1][owned] Old extension state. */
#endif /* TPP_HAVE_EXTENSIONS_PUSH_POP */
} tpp_extensions;

#if TPP_HAVE_EXTENSIONS_PUSH_POP
#define tpp_extensions_init(self)                                           \
	(void)((self)->TPP_INTERNAL(te_state)   = tpp_extensions_state_default, \
	       (self)->TPP_INTERNAL(te_pushcnt) = 0,                            \
	       (self)->TPP_INTERNAL(te_prev)    = NULL)
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_extensions_fini(tpp_extensions *tpp_restrict self);

/* Push the current extensions state */
#define tpp_extensions_push(self) (void)(++(self)->TPP_INTERNAL(te_pushcnt))

/* Pop the current extensions state (may only be called when `tpp_extensions_canpop(self)') */
TPP_DECL TPP_NONNULL((1)) void TPPCALL tpp_extensions_pop(tpp_extensions *tpp_restrict self);
#define tpp_extensions_canpop(self) ((self)->TPP_INTERNAL(te_pushcnt) != 0 || (self)->TPP_INTERNAL(te_prev) != NULL)

/* When true, `tpp_extensions_setid()' must first copy the extension
 * state (which requires heap memory, and may thus fail) */
#define tpp_extensions_mustcopy(self) ((self)->TPP_INTERNAL(te_pushcnt) != 0)

/* @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: OOM */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_extensions_setid(tpp_extensions *tpp_restrict self,
                     tpp_extension_id id, bool enabled);
#else /* TPP_HAVE_EXTENSIONS_PUSH_POP */
#define tpp_extensions_init(self) (void)((self)->TPP_INTERNAL(te_state) = tpp_extensions_state_default)
#define tpp_extensions_fini(self) (void)0
#define tpp_extensions_setid(self, id, enabled) \
	(tpp_extensions_state_setid(&(self)->TPP_INTERNAL(te_state), id, enabled), TPP_EOK)
#endif /* !TPP_HAVE_EXTENSIONS_PUSH_POP */
#define tpp_extensions_get(self, id)   tpp_extensions_state_get(&(self)->TPP_INTERNAL(te_state), id)
#define tpp_extensions_getid(self, id) tpp_extensions_state_getid(&(self)->TPP_INTERNAL(te_state), id)


/* Convert between extension IDs and their human-readable names. */
TPP_DECL TPP_WUNUSED char const *TPPCALL
tpp_extension_getname(tpp_extension_id id);

/* @return: TPP_EXT_COUNT: No such extension */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_extension_id TPPCALL
tpp_extension_byname_ex(char const *tpp_restrict name, tpp_size name_maxlen);
#define tpp_extension_byname(name) tpp_extension_byname_ex(name, TPP_SIZE_MAX)

/* Returns the ID of the extension with the name that is closest to "name"
 * When no extensions are defined (at all), this will return "TPP_EXT_COUNT" */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_extension_id TPPCALL
tpp_extension_nearest_ex(char const *tpp_restrict name, tpp_size name_maxlen);
#define tpp_extension_nearest(name) tpp_extension_nearest_ex(name, TPP_SIZE_MAX)
#else /* TPP_HAVE_EXTENSIONS */
#define tpp_extensions_state_get(self, id) 1
#define tpp_extensions_get(self, id)       1
#endif /* !TPP_HAVE_EXTENSIONS */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_EXTENSIONS_H */
