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
#ifndef GUARD_TPP_WARNINGS_H
#define GUARD_TPP_WARNINGS_H 1

#include "api.h"
#include "config.h"
#include "error.h"
#include "tuple.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_WARNING_NUMBERS
#define TPP_WARNING_NUMBER_INVALID UINT_MAX
#endif /* TPP_HAVE_WARNING_NUMBERS */

#if TPP_HAVE_WARNINGS


/************************************************************************/
/* WARNING STATE                                                        */
/************************************************************************/

/* Configuration options for warnings.
 *
 * NOTE: When a warning has multiple groups (and/or a number), and is triggered,
 *       then the state used for emission is the one with the lowest, numerical
 *       value (e.g.: if at least one group is "TPP_WSTATE_DISABLED", then the
 *       warning is disabled). The used group/number will be printed however. */
typedef enum tpp_warning_state {
	TPP_WSTATE_DISABLED = 0, /* Warning exists, but is disabled */
	TPP_WSTATE_WARN     = 1, /* Regular warning */
#if TPP_HAVE_WARNING_ERROR
	TPP_WSTATE_ERROR    = 2, /* Error (s.a. `tpp_lexer::tl_maxerrors') */
#define TPP_WSTATE_ERROR_OR_FATAL TPP_WSTATE_ERROR
#else /* TPP_HAVE_WARNING_ERROR */
#define TPP_WSTATE_ERROR_OR_FATAL TPP_WSTATE_FATAL
#endif /* !TPP_HAVE_WARNING_ERROR */
	TPP_WSTATE_FATAL    = 3, /* Immediately cause a `TPP_ELEXERROR' error */
#define TPP_WSTATE_UNDEFINED TPP_WSTATE_FATAL /* Intended for numbers-definitions: refer to linked groups */

#if TPP_HAVE_WARNING_SUPPRESS
	TPP_WSTATE_SUPPRESS = -1, /* Treat as `TPP_WSTATE_DISABLED' a couple of times, then switch to old state
	                           * HINT: In "tpp_warnings_state", this is represented as "TPP_WSTATE_FATAL". */
#endif /* TPP_HAVE_WARNING_SUPPRESS */

#if TPP_HAVE_WARNING_DEFAULT
	TPP_WSTATE_DEFAULT = 99, /* Only for `tpp_warnings_setctx()': restore behavior from `tpp_warnings_state_default' */
#endif /* TPP_HAVE_WARNING_DEFAULT */
} tpp_warning_state;

/* Check if "lhs" is more important than "rhs".
 *
 * When a warning is emitted, the context that is configured to the greatest
 * importance is used to determine how that specific warning should be treated. */
#define tpp_warning_state_ismoreimportant(lhs, rhs) \
	((int)(lhs) < (int)(rhs))

/* Check if the warning state specified by "self" will cause something to be emitted. */
#if TPP_HAVE_WARNING_SUPPRESS
#if 1
#define tpp_warning_state_willemit(self) ((int)(self) > 0)
#else
#define tpp_warning_state_willemit(self) \
	((self) != TPP_WSTATE_DISABLED && (self) != TPP_WSTATE_SUPPRESS)
#endif
#else /* TPP_HAVE_WARNING_SUPPRESS */
#define tpp_warning_state_willemit(self) \
	((self) != TPP_WSTATE_DISABLED)
#endif /* !TPP_HAVE_WARNING_SUPPRESS */



/************************************************************************/
/* WARNING GROUPS                                                       */
/************************************************************************/

typedef enum tpp_warning_group_id {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) wgroup_id,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	TPP_WG_COUNT
} tpp_warning_group_id;

/* Return a pointer to a \0\0-terminated list of strings describing the names
 * of for the given warning group "id". Returns "NULL" if "id" is "TPP_WG_COUNT"
 * or some other invalid warning group ID. */
TPP_DECL TPP_WUNUSED char const *TPPCALL
tpp_warning_group_getnames(tpp_warning_group_id id);

/* @return: TPP_WG_COUNT: No such warning_group */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_group_id TPPCALL
tpp_warning_group_byname_ex(char const *tpp_restrict name, tpp_size name_maxlen);
#define tpp_warning_group_byname(name) tpp_warning_group_byname_ex(name, TPP_SIZE_MAX)

#if TPP_HAVE_TPP_WARNING_GROUP_NEAREST
/* Returns the ID of the warning group with the name that is closest to "name"
 * When no warning groups are defined (at all), this will return "TPP_WG_COUNT" */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_group_id TPPCALL
tpp_warning_group_nearest_ex(char const *tpp_restrict name, tpp_size name_maxlen);
#define tpp_warning_group_nearest(name) tpp_warning_group_nearest_ex(name, TPP_SIZE_MAX)
#endif /* TPP_HAVE_TPP_WARNING_GROUP_NEAREST */



/************************************************************************/
/* WARNING ID                                                           */
/************************************************************************/

typedef enum tpp_warning_id {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) warning_id,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	TPP_W_COUNT
} tpp_warning_id;

/* Returns a TPP_WG_COUNT-terminated list of group IDs associated with the given warning "id".
 * When the given "id" is "TPP_W_COUNT" or invalid, return a pointer to an empty warning-group-id-list. */
TPP_DECL TPP_RETNONNULL TPP_WUNUSED tpp_warning_group_id const *TPPCALL
tpp_warning_getgroups(tpp_warning_id id);

/* Returns the "tpp_lexer_printf_warning"-style format string assigned with "id".
 * When "id" is TPP_W_COUNT, invalid, or declared as "TPP_WARNING_EX", return "NULL" instead. */
TPP_DECL TPP_WUNUSED char const *TPPCALL
tpp_warning_getformat(tpp_warning_id id);

#if TPP_HAVE_WARNING_NUMBERS
/* Returns the warning ID linked to a given "number", or "TPP_W_COUNT" if "number" is unknown */
TPP_DECL TPP_WUNUSED tpp_warning_id TPPCALL
tpp_warning_ofnumber(unsigned int number);

/* Returns the TPP_WARNING_NUMBER_INVALID-terminated list of warning numbers
 * associated with "warning_id". If "warning_id" doesn't have any warning
 * numbers, return a pointer to "TPP_WARNING_NUMBER_INVALID". */
TPP_DECL TPP_RETNONNULL TPP_WUNUSED unsigned int const *TPPCALL
tpp_warning_getnumbers(tpp_warning_id warning_id);
#else /* TPP_HAVE_WARNING_NUMBERS */
#define tpp_warning_ofnumber(number) TPP_W_COUNT
#endif /* !TPP_HAVE_WARNING_NUMBERS */




/************************************************************************/
/* WARNING CONTEXT ID                                                   */
/************************************************************************/

/* Warning context ID (used internally to keep track
 * of warning states for groups & numbered warnings) */
typedef enum tpp_warning_context_id {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) TPP_WC_##wgroup_id,
#include TPP_CONFIG_DEFS_FILENAME

#if TPP_HAVE_WARNING_NUMBERS
	TPP_WC_NUMBER_MIN,
	TPP_INTERNAL(_TPP_WC_NUMBER_MIN) = TPP_WC_NUMBER_MIN - 1,
#define TPP_DECLARE_NUMBERED_WARNING(warning_id) TPP_WC_##warning_id,
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_IF_NONEMPTY(numbers, TPP_DECLARE_NUMBERED_WARNING, warning_id)
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DECLARE_NUMBERED_WARNING
#endif /* TPP_HAVE_WARNING_NUMBERS */
#undef TPP_DEFS
	TPP_WC_COUNT
} tpp_warning_context_id;

/* Check what kind of context is being described by "self" */
#if TPP_HAVE_WARNING_NUMBERS
#define tpp_warning_context_id_isgroup(self) \
	(!tpp_warning_context_id_isnumber(self))
#define tpp_warning_context_id_isnumber(self) \
	((unsigned int)(self) >= (unsigned int)TPP_WC_NUMBER_MIN)
#else /* TPP_HAVE_WARNING_NUMBERS */
#define tpp_warning_context_id_isgroup(self)  1
#define tpp_warning_context_id_isnumber(self) 0
#endif /* !TPP_HAVE_WARNING_NUMBERS */

/* Return the context-id of a given "tpp_warning_group_id wgroup_id" */
#define tpp_warning_context_id_ofgroup(wgroup_id) \
	((tpp_warning_context_id)(unsigned int)(wgroup_id))

/* Return the group-id of a given "tpp_warning_context_id ctx_id".
 * Caller must ensure that `tpp_warning_context_id_isgroup(ctx_id)' */
#define tpp_warning_context_id_asgroup(ctx_id) \
	((tpp_warning_group_id)(unsigned int)(ctx_id))

#if TPP_HAVE_WARNING_NUMBERS
/* Returns the context-id of a given (should-be) "numbered" warning.
 * When "warning_id" is invalid, "TPP_W_COUNT", or not numbered, this
 * function will return "TPP_WC_COUNT" instead. */
TPP_DECL TPP_WUNUSED tpp_warning_context_id TPPCALL
tpp_warning_context_id_ofwarning(tpp_warning_id warning_id);

/* Returns the warning-id linked to "ctx_id", when `tpp_warning_context_id_isnumber(ctx_id)'.
 * When no warning is linked to "ctx_id" ("ctx_id" is either linked to a warning group, or
 * is "TPP_WC_COUNT" or some other invalid ID), return "TPP_W_COUNT" instead. */
TPP_DECL TPP_WUNUSED tpp_warning_id TPPCALL
tpp_warning_context_id_aswarning(tpp_warning_context_id ctx_id);
#else /* TPP_HAVE_WARNING_NUMBERS */
#define tpp_warning_context_id_ofwarning(warning_id) TPP_WC_COUNT
#define tpp_warning_context_id_aswarning(ctx_id)     TPP_W_COUNT
#endif /* !TPP_HAVE_WARNING_NUMBERS */




/************************************************************************/
/* WARNINGS STATE                                                       */
/************************************************************************/
typedef union tpp_warnings_state {
	struct {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) \
	unsigned int TPP_INTERNAL(twsg_##wgroup_id): 2; /* One of `tpp_warning_state' */
#include TPP_CONFIG_DEFS_FILENAME
#if TPP_HAVE_WARNING_NUMBERS
#define TPP_DECLARE_NUMBERED_WARNING(warning_id) \
	unsigned int TPP_INTERNAL(twsn_##warning_id): 2; /* One of `tpp_warning_state' */
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_IF_NONEMPTY(numbers, TPP_DECLARE_NUMBERED_WARNING, warning_id)
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DECLARE_NUMBERED_WARNING
#endif /* TPP_HAVE_WARNING_NUMBERS */
#undef TPP_DEFS
	} TPP_INTERNAL(tws_state);
	unsigned char TPP_INTERNAL(tws_bitset)[TPP_WC_COUNT ? (((TPP_WC_COUNT * 2) + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];
} tpp_warnings_state;

TPP_DECL tpp_warnings_state const tpp_warnings_state_default;

#define _tpp_warnings_state_bitindx(ctx_id) (((unsigned int)((unsigned int)(ctx_id) / (TPP_CHAR_BIT >> 1))))
#define _tpp_warnings_state_bitshft(ctx_id) (((unsigned int)((unsigned int)(ctx_id) % (TPP_CHAR_BIT >> 1))) << 1)

/* Get/set the warning state of a given "tpp_warning_context_id ctx_id" */
#define tpp_warnings_state_get(self, ctx_id) \
	((tpp_warning_state)(((self)->TPP_INTERNAL(tws_bitset)[_tpp_warnings_state_bitindx(ctx_id)] >> _tpp_warnings_state_bitshft(ctx_id)) & 3))
#define tpp_warnings_state_set(self, ctx_id, value)                                                                                \
	(void)((self)->TPP_INTERNAL(tws_bitset)[_tpp_warnings_state_bitindx(ctx_id)] =                                                 \
	       ((self)->TPP_INTERNAL(tws_bitset)[_tpp_warnings_state_bitindx(ctx_id)] & ~(3 << _tpp_warnings_state_bitshft(ctx_id))) | \
	       (((unsigned char)(unsigned int)(value)) << _tpp_warnings_state_bitshft(ctx_id)))



#if TPP_HAVE_WARNING_SUPPRESS
typedef struct tpp_warning_suppress_item {
	tpp_warning_context_id TPP_INTERNAL(twsi_ctx_id);  /* Context ID of this suppression item */
	tpp_warning_state      TPP_INTERNAL(twsi_restore); /* Warning state to restore once the warning is no longer being suppressed */
	tpp_size               TPP_INTERNAL(twsi_count);   /* # of times to suppress this warning before restoring its previous state */
} tpp_warning_suppress_item;

typedef struct tpp_warning_suppressions {
	tpp_size                   TPP_INTERNAL(tws_ctxc); /* # of warnings that are being suppressed right now */
	tpp_size                   TPP_INTERNAL(tws_ctxa); /* Allocated size of `tws_ctxv' */
	tpp_warning_suppress_item *TPP_INTERNAL(tws_ctxv); /* [0..tws_ctxc|alloc(tws_ctxa)][owned] Vector of suppressions (sorted by `twsi_ctx_id') */
} tpp_warning_suppressions;
#define tpp_warning_suppressions_init(self)    \
	(void)((self)->TPP_INTERNAL(tws_ctxc) = 0, \
	       (self)->TPP_INTERNAL(tws_ctxa) = 0, \
	       (self)->TPP_INTERNAL(tws_ctxv) = NULL)
#define tpp_warning_suppressions_fini(self)    \
	(tpp_free((self)->TPP_INTERNAL(tws_ctxv)), \
	 tpp_dbg_memset(self, sizeof(tpp_warning_suppressions)))
#endif /* TPP_HAVE_WARNING_SUPPRESS */


/* Lexer warnings configuration */
typedef struct tpp_warnings {
	tpp_warnings_state       TPP_INTERNAL(tw_state);        /* [const_if(tw_pushcnt > 0)] Warning state */
#if TPP_HAVE_WARNING_SUPPRESS
	tpp_warning_suppressions TPP_INTERNAL(tw_suppressions); /* [const_if(tw_pushcnt > 0)] Information about suppressed warnings */
#define _tpp_warnings_init_suppressions(self) , tpp_warning_suppressions_init(&(self)->TPP_INTERNAL(tw_suppressions))
#else /* TPP_HAVE_WARNING_SUPPRESS */
#define _tpp_warnings_init_suppressions(self) /* nothing */
#endif /* !TPP_HAVE_WARNING_SUPPRESS */
#if TPP_HAVE_WARNINGS_PUSH_POP
	tpp_size                 TPP_INTERNAL(tw_pushcnt);      /* # of times warnings pushed were since last modified */
	struct tpp_warnings     *TPP_INTERNAL(tw_prev);         /* [0..1][owned] Old warning state. */
#define _tpp_warnings_init_push(self) , (self)->TPP_INTERNAL(tw_pushcnt) = 0, (self)->TPP_INTERNAL(tw_prev) = NULL
#else /* TPP_HAVE_WARNINGS_PUSH_POP */
#define _tpp_warnings_init_push(self) /* nothing */
#endif /* !TPP_HAVE_WARNINGS_PUSH_POP */
} tpp_warnings;

/* Initialize a given warnings context "self" */
#define tpp_warnings_init(self)                                        \
	(void)((self)->TPP_INTERNAL(tw_state) = tpp_warnings_state_default \
	       _tpp_warnings_init_suppressions(self)                       \
	       _tpp_warnings_init_push(self))

#undef TPP_HAVE_WARNINGS_FINI
#define TPP_HAVE_WARNINGS_FINI \
	(TPP_HAVE_WARNING_SUPPRESS || TPP_HAVE_WARNINGS_PUSH_POP)

/* Finalize a given warnings context "self" */
#if TPP_HAVE_WARNINGS_FINI
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_warnings_fini(tpp_warnings *tpp_restrict self);

#if TPP_HAVE_LEXER_COPY
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_warnings_copy(tpp_warnings *tpp_restrict self,
                  tpp_warnings const *tpp_restrict from);
#endif /* TPP_HAVE_LEXER_COPY */
#else /* TPP_HAVE_WARNINGS_FINI */
#define tpp_warnings_fini(self) tpp_dbg_memset(self, sizeof(tpp_warnings))
#if TPP_HAVE_LEXER_COPY
#define tpp_warnings_copy(self, from) (*(self) = *(from), TPP_EOK)
#endif /* TPP_HAVE_LEXER_COPY */
#endif /* !TPP_HAVE_WARNINGS_FINI */


#if TPP_HAVE_WARNINGS_PUSH_POP
#define _tpp_warnings_alloc() ((tpp_warnings *)tpp_malloc(sizeof(tpp_warnings)))
#define _tpp_warnings_free(p) tpp_free(p)

/* Push the current warnings state */
#define tpp_warnings_push(self) (void)(++(self)->TPP_INTERNAL(tw_pushcnt))

/* Pop the current warnings state (may only be called when `tpp_warnings_canpop(self)') */
TPP_DECL TPP_NONNULL((1)) void TPPCALL tpp_warnings_pop(tpp_warnings *tpp_restrict self);
#define tpp_warnings_canpop(self)             \
	((self)->TPP_INTERNAL(tw_pushcnt) != 0 || \
	 (self)->TPP_INTERNAL(tw_prev) != NULL)

/* When true, `tpp_warnings_setctx()' must first copy the extension
 * state (which requires heap memory, and may thus fail) */
#define tpp_warnings_mustcopy(self) ((self)->TPP_INTERNAL(tw_pushcnt) != 0)
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */

/* Return the state of "ctx_id". The caller is
 * responsible to ensure that "ctx_id" is valid. */
#if TPP_HAVE_WARNING_SUPPRESS
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_state TPPCALL
tpp_warnings_getctx(tpp_warnings const *tpp_restrict self,
                    tpp_warning_context_id ctx_id);
#else /* TPP_HAVE_WARNING_SUPPRESS */
#define tpp_warnings_getctx(self, ctx_id) \
	tpp_warnings_state_get(&(self)->TPP_INTERNAL(tw_state), ctx_id)
#endif /* !TPP_HAVE_WARNING_SUPPRESS */


#undef TPP_HAVE_WARNINGS_SETCTX_MAYFAIL
#define TPP_HAVE_WARNINGS_SETCTX_MAYFAIL \
	(TPP_HAVE_WARNING_SUPPRESS || TPP_HAVE_WARNINGS_PUSH_POP)

/* Set the state of "ctx_id" to "state".
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HAVE_WARNINGS_SETCTX_MAYFAIL
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_warnings_setctx(tpp_warnings *tpp_restrict self,
                    tpp_warning_context_id ctx_id,
                    tpp_warning_state state);
#else /* TPP_HAVE_WARNINGS_SETCTX_MAYFAIL */
#define tpp_warnings_setctx(self, ctx_id, state) \
	(tpp_warnings_setctx_(self, ctx_id, state), TPP_EOK)
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_warnings_setctx_(tpp_warnings *tpp_restrict self,
                     tpp_warning_context_id ctx_id,
                     tpp_warning_state state);
#endif /* !TPP_HAVE_WARNINGS_SETCTX_MAYFAIL */



typedef struct tpp_warning_invokeinfo {
	tpp_warning_state      twii_state;  /* State with which the warning should be invoked.
	                                     * Always one of:
	                                     * - TPP_WSTATE_DISABLED
	                                     * - TPP_WSTATE_WARN
	                                     * - TPP_WSTATE_ERROR      #if TPP_HAVE_WARNING_ERROR
	                                     * - TPP_WSTATE_FATAL */
	tpp_warning_context_id twii_ctx_id; /* Context ID that for error messages */
} tpp_warning_invokeinfo;

#undef TPP_HAVE_WARNINGS_INVOKE_MAYFAIL
#define TPP_HAVE_WARNINGS_INVOKE_MAYFAIL \
	(TPP_HAVE_WARNING_SUPPRESS && TPP_HAVE_WARNINGS_PUSH_POP)

/* Invoke "warning_id" (updating suppression counters if necessary) and
 * returning information about the context/state with which the warning
 * should be processed.
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (only when "TPP_HAVE_WARNINGS_INVOKE_MAYFAIL") */
#if TPP_HAVE_WARNINGS_INVOKE_MAYFAIL
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_warnings_invoke(tpp_warnings *tpp_restrict self, tpp_warning_id warning_id,
                    tpp_warning_invokeinfo *tpp_restrict result);
#else /* TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */
#define tpp_warnings_invoke(self, warning_id, result) \
	(tpp_warnings_invoke_nofail(self, warning_id, result), TPP_EOK)
TPP_DECL TPP_NONNULL((1, 3)) void TPPCALL
tpp_warnings_invoke_nofail(tpp_warnings const *tpp_restrict self, tpp_warning_id warning_id,
                           tpp_warning_invokeinfo *tpp_restrict result);
#endif /* !TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */

#endif /* TPP_HAVE_WARNINGS */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_WARNINGS_H */
