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
#ifndef GUARD_TPP_BUILTINS_C
#define GUARD_TPP_BUILTINS_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "keyword.h"
#include "lexer.h"
#include "macro.h"
#include "string.h"
#include "token.h"
#include "tuple.h"
#include "warnings.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_MACROS
#define _TPP_BUILTIN_KEYWORD_tk_macro_DEF  tpp_macro *tk_macro;
#define _TPP_BUILTIN_KEYWORD_tk_macro_INIT NULL,
#else /* TPP_HAVE_CPP_MACROS */
#define _TPP_BUILTIN_KEYWORD_tk_macro_DEF  /* nothing */
#define _TPP_BUILTIN_KEYWORD_tk_macro_INIT /* nothing */
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
#define _TPP_BUILTIN_KEYWORD_tk_misc_DEF  tpp_keyword_misc *tk_misc;
#define _TPP_BUILTIN_KEYWORD_tk_misc_INIT NULL,
#else /* TPP_HAVE_KEYWORD_MISC */
#define _TPP_BUILTIN_KEYWORD_tk_misc_DEF  /* nothing */
#define _TPP_BUILTIN_KEYWORD_tk_misc_INIT /* nothing */
#endif /* !TPP_HAVE_KEYWORD_MISC */
#if TPP_HAVE_KEYWORD_ASSTRING
#define _TPP_BUILTIN_KEYWORD_tk_refcnt_DEF  tpp_refcnt_atomic tk_refcnt;
#define _TPP_BUILTIN_KEYWORD_tk_refcnt_INIT TPP_REFCNT_ATOMIC_INIT(1),
#else /* TPP_HAVE_KEYWORD_ASSTRING */
#define _TPP_BUILTIN_KEYWORD_tk_refcnt_DEF  /* nothing */
#define _TPP_BUILTIN_KEYWORD_tk_refcnt_INIT /* nothing */
#endif /* !TPP_HAVE_KEYWORD_ASSTRING */


#if TPP_HAVE_EXTENSIONS
static struct tpp_extension_names_struct {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) char ten_##id[sizeof(name) / sizeof(char)];
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
} const tpp_extension_names = {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) /* .ten_##id = */ name,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
};

static tpp_size const tpp_extension_name_offsets_byid[TPP_EXT_COUNT] = {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) \
	/* [id] = */ tpp_offsetof(struct tpp_extension_names_struct, ten_##id),
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
};
#define tpp_extension_getname_fast(id) \
	((char const *)&tpp_extension_names + tpp_extension_name_offsets_byid[id])
#endif /* TPP_HAVE_EXTENSIONS */



#if TPP_HAVE_WARNINGS

static struct tpp_warning_group_names_struct {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	char twgn_##wgroup_id##_##index[sizeof(value) / sizeof(char)];
#define TPP_WGROUP(wgroup_id, names, default)                                                  \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id) \
	char twgn_tail_##wgroup_id;
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
} const tpp_warning_group_names = {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	/* .twgn_##wgroup_id##_##index = */ value,
#define TPP_WGROUP(wgroup_id, names, default)                                                  \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id) \
	/* .twgn_tail_##wgroup_id = */ 0,
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
};

static tpp_size const tpp_warning_group_name_offsets_byid[TPP_WG_COUNT] = {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) \
	/* [wgroup_id] = */ tpp_offsetof(struct tpp_warning_group_names_struct, twgn_##wgroup_id##_0),
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
};
#define tpp_warning_group_getname_fast(id) \
	((char const *)&tpp_warning_group_names + tpp_warning_group_name_offsets_byid[id])


/* Return a pointer to a \0\0-terminated list of strings describing the names
 * of for the given warning group "id". Returns "NULL" if "id" is "TPP_WG_COUNT"
 * or some other invalid warning group ID. */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_warning_group_getnames(tpp_warning_group_id id) {
	if ((unsigned int)id >= (unsigned int)TPP_WG_COUNT)
		return NULL;
	return tpp_warning_group_getname_fast((unsigned int)id);
}


static struct tpp_warning_groups_struct {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define _TPP_EXPAND_WARNING_GROUP_IDS(warning_id, index, value) \
	tpp_warning_group_id twig_##warning_id##_##index;
#define _TPP_EXPAND_WARNING_GROUP_TAIL(warning_id) \
	tpp_warning_group_id twig_tail_##warning_id;
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format)                             \
	TPP_TUPLE_FOREACH(wgroup_ids, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WARNING_GROUP_IDS, warning_id) \
	TPP_TUPLE_IF_NONEMPTY(wgroup_ids, _TPP_EXPAND_WARNING_GROUP_TAIL, warning_id)
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WARNING_GROUP_IDS
#undef _TPP_EXPAND_WARNING_GROUP_TAIL
#undef TPP_DEFS
} const tpp_warning_groups = {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define _TPP_EXPAND_WARNING_GROUP_IDS(warning_id, index, value) \
	/* .twig_##warning_id##_##index = */ value,
#define _TPP_EXPAND_WARNING_GROUP_TAIL(warning_id) \
	/* .twig_tail_##warning_id = */ TPP_WG_COUNT,
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format)                             \
	TPP_TUPLE_FOREACH(wgroup_ids, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WARNING_GROUP_IDS, warning_id) \
	TPP_TUPLE_IF_NONEMPTY(wgroup_ids, _TPP_EXPAND_WARNING_GROUP_TAIL, warning_id)
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WARNING_GROUP_IDS
#undef _TPP_EXPAND_WARNING_GROUP_TAIL
#undef TPP_DEFS
};

static tpp_size const tpp_warning_group_offsets_byid[TPP_W_COUNT] = {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define _TPP_WARNING_GROUPS_NONEMPTY(warning_id) \
	/* [warning_id] = */ tpp_offsetof(struct tpp_warning_groups_struct, twig_##warning_id##_0),
#define _TPP_WARNING_GROUPS_EMPTY(warning_id) \
	/* [warning_id] = */ sizeof(struct tpp_warning_groups_struct) - sizeof(tpp_warning_group_id),
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format)   \
	TPP_TUPLE_IF_NONEMPTY(wgroup_ids, _TPP_WARNING_GROUPS_NONEMPTY, warning_id) \
	TPP_TUPLE_IF_EMPTY(wgroup_ids, _TPP_WARNING_GROUPS_EMPTY, warning_id)
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_WARNING_GROUPS_NONEMPTY
#undef _TPP_WARNING_GROUPS_EMPTY
#undef TPP_DEFS
};
#define tpp_warning_groups_fast(id) \
	((tpp_warning_group_id const *)((char const *)&tpp_warning_groups + tpp_warning_group_offsets_byid[id]))


/* Returns a TPP_WG_COUNT-terminated list of group IDs associated with the given warning "id".
 * When the given "id" is "TPP_W_COUNT" or invalid, return a pointer to an empty warning-group-id-list. */
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED tpp_warning_group_id const *TPPCALL
tpp_warning_getgroups(tpp_warning_id id) {
	if ((unsigned int)id >= (unsigned int)TPP_W_COUNT) {
		return (tpp_warning_group_id const *)((char const *)&tpp_warning_groups +
		                                      sizeof(tpp_warning_groups) -
		                                      sizeof(tpp_warning_group_id));
	}
	return tpp_warning_groups_fast((unsigned int)id);
}

TPP_IMPL tpp_warnings_state const tpp_warnings_state_default = {
	/* .tws_state = */ {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) \
		/* .twsg_##wgroup_id  = */ (unsigned int)default,
#include TPP_CONFIG_DEFS_FILENAME
#if TPP_HAVE_WARNING_NUMBERS
#define TPP_DECLARE_NUMBERED_WARNING(numbers_default) \
		/* .twsn_##warning_id = */ (unsigned int)numbers_default,
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
		TPP_TUPLE_IF_NONEMPTY(numbers, TPP_DECLARE_NUMBERED_WARNING, numbers_default)
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DECLARE_NUMBERED_WARNING
#endif /* TPP_HAVE_WARNING_NUMBERS */
#undef TPP_DEFS
	}
};

#endif /* TPP_HAVE_WARNINGS */


/* Define this macro as "tpp-amalgamation-builtins.inl" */
#ifndef TPP_CONFIG_BUILTINS_FILENAME

#if TPP_HAVE_EXTENSIONS
static tpp_size tpp_extension_name_offsets_byname[TPP_EXT_COUNT] = {};
static int tpp_extension_name_offset_compare(void const *lhs, void const *rhs) {
	tpp_size lhs_value = *(tpp_size const *)lhs;
	tpp_size rhs_value = *(tpp_size const *)rhs;
	char const *lhs_string = (char const *)&tpp_extension_names + lhs_value;
	char const *rhs_string = (char const *)&tpp_extension_names + rhs_value;
	return tpp_strcmp(lhs_string, rhs_string);
}

static void tpp_init_extension_name_offsets_byname_impl(void) {
	tpp_memcpy(tpp_extension_name_offsets_byname,
	           tpp_extension_name_offsets_byid,
	           sizeof(tpp_extension_name_offsets_byid));
	qsort(tpp_extension_name_offsets_byname, TPP_EXT_COUNT, sizeof(tpp_size),
	      &tpp_extension_name_offset_compare);
}

static void tpp_init_extension_name_offsets_byname(void) {
	tpp_once({
		tpp_init_extension_name_offsets_byname_impl();
	});
}
#endif /* TPP_HAVE_EXTENSIONS */


#if TPP_HAVE_WARNINGS
enum {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	_TPP_WGN_##wgroup_id##_##index,
#define TPP_WGROUP(wgroup_id, names, default) \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id)
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
	TPP_WGN_COUNT
};

static tpp_size const tpp_warning_group_name_offsets_bynameid[TPP_WGN_COUNT] = {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	/* [_TPP_WGN_##wgroup_id##_##index] = */ tpp_offsetof(struct tpp_warning_group_names_struct, twgn_##wgroup_id##_##index),
#define TPP_WGROUP(wgroup_id, names, default) \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id)
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
};

static tpp_size tpp_warning_group_name_offsets_byname[TPP_WGN_COUNT] = {};
static int tpp_warning_group_name_offset_compare(void const *lhs, void const *rhs) {
	tpp_size lhs_value = *(tpp_size const *)lhs;
	tpp_size rhs_value = *(tpp_size const *)rhs;
	char const *lhs_string = (char const *)&tpp_warning_group_names + lhs_value;
	char const *rhs_string = (char const *)&tpp_warning_group_names + rhs_value;
	return tpp_strcmp(lhs_string, rhs_string);
}

static void tpp_init_warning_group_name_offsets_byname_impl(void) {
	tpp_memcpy(tpp_warning_group_name_offsets_byname,
	           tpp_warning_group_name_offsets_bynameid,
	           sizeof(tpp_warning_group_name_offsets_bynameid));
	qsort(tpp_warning_group_name_offsets_byname, TPP_WG_COUNT, sizeof(tpp_size),
	      &tpp_warning_group_name_offset_compare);
}

static void tpp_init_warning_group_name_offsets_byname(void) {
	tpp_once({
		tpp_init_warning_group_name_offsets_byname_impl();
	});
}
#endif /* TPP_HAVE_WARNINGS */

/* Implement TPP's hashing algorithm for constant strings, using only the preprocessor!
 * -> THIS! Is the power that TPP can wield: Full functional programming */
#if defined(__TPP_VERSION__) && (TPP_SIZEOF_tpp_hash == 4 || TPP_SIZEOF_tpp_hash == 8)
#define TPP_PRIVATE_PP_CAT2(a,b) a##b
#define TPP_PRIVATE_PP_CAT(a,b) TPP_PRIVATE_PP_CAT2(a,b)
#define TPP_PRIVATE_HASHOF_0(result,str) result
#if __TPP_VERSION__ < 300
#pragma warning(disable: 108) /* Index out-of-bounds in `__TPP_EVAL' */
#endif /* __TPP_VERSION__ < 300 */

/* Using some sick-a$$ TPP extensions, we can actually
 * calculate keyword hashes within the preprocessor! */
#pragma extension(push,"-fmacro-recursion")
#if TPP_SIZEOF_tpp_hash == 4
#define TPP_PRIVATE_HASHOF_1(result,str) TPP_PRIVATE_HASHOF2(__TPP_EVAL((result*263+str[0])&TPP_HASH_C(0xffffffff)),__TPP_EVAL(str[1:]))
#elif TPP_SIZEOF_tpp_hash == 8
#define TPP_PRIVATE_HASHOF_1(result,str) TPP_PRIVATE_HASHOF2(__TPP_EVAL((result*263+str[0])&TPP_HASH_C(0xffffffffffffffff)),__TPP_EVAL(str[1:]))
#endif
#define TPP_PRIVATE_HASHOF2(result,str) TPP_PRIVATE_PP_CAT(TPP_PRIVATE_HASHOF_,__TPP_EVAL(!!str))(result,str)
#pragma extension(pop)
#if TPP_SIZEOF_tpp_hash == 4
#define TPP_HASHOF(str) TPP_HASH_C(TPP_PRIVATE_HASHOF2(1,str))
#elif TPP_SIZEOF_tpp_hash == 8
#define TPP_HASHOF(str) TPP_HASH_C(TPP_PRIVATE_HASHOF2(1,str))
#endif
#endif /* __TPP_VERSION__ && (TPP_SIZEOF_tpp_hash == 4 || TPP_SIZEOF_tpp_hash == 8) */

#ifdef TPP_HASHOF
#define TPP_MAYBE_HASHOF(s) TPP_HASHOF(s)
#else /* TPP_HASHOF */
#define TPP_MAYBE_HASHOF(s) TPP_HASH_C(0)
#endif /* !TPP_HASHOF */


/* Create definitions for builtin keywords */
#define TPP_DEFINE_BUILTIN_KEYWORD(id, str)                      \
	static struct tpp_builtin_keyword_struct_##id {              \
		tpp_token_id         tk_id;                              \
		tpp_keyword  *tk_next;                                   \
		_TPP_BUILTIN_KEYWORD_tk_macro_DEF                        \
		_TPP_BUILTIN_KEYWORD_tk_misc_DEF                         \
		tpp_hash             tk_hash;                            \
		_TPP_BUILTIN_KEYWORD_tk_refcnt_DEF                       \
		tpp_size             tk_len;                             \
		char                 tk_kwd[sizeof(str) / sizeof(char)]; \
	} tpp_builtin_keyword_##id = {                               \
		/* .tk_id        = */ id,                                \
		/* .tk_next      = */ NULL,                              \
		_TPP_BUILTIN_KEYWORD_tk_macro_INIT                       \
		_TPP_BUILTIN_KEYWORD_tk_misc_INIT                        \
		/* .tk_hash      = */ TPP_MAYBE_HASHOF(str),             \
		_TPP_BUILTIN_KEYWORD_tk_refcnt_INIT                      \
		/* .tk_len       = */ (sizeof(str) / sizeof(char)) - 1,  \
		/* .tk_kwd       = */ str                                \
	};

#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_KWD(id, string) \
	TPP_DEFINE_BUILTIN_KEYWORD(id, string)
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS

#define tpp_builtin_keywords_list tpp_builtin_keywords_list
static tpp_keyword *tpp_builtin_keywords_list[] = {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_KWD(id, string) (tpp_keyword *)&tpp_builtin_keyword_##id,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	NULL
};

#if TPP_HAVE_USER_KEYWORDS
#define TPP_BUILTIN_KEYWORD_COUNT  (TPP_TOK_USERKEYWORD_BEGIN - TPP_TOK_KEYWORD_BEGIN)
#else /* TPP_HAVE_USER_KEYWORDS */
#define TPP_BUILTIN_KEYWORD_COUNT  (TPP_TOK_USERKEYWORD - TPP_TOK_KEYWORD_BEGIN)
#endif /* !TPP_HAVE_USER_KEYWORDS */
#define TPP_BUILTIN_KEYWORD_MASK_1 (TPP_BUILTIN_KEYWORD_COUNT | (TPP_BUILTIN_KEYWORD_COUNT >> 16))
#define TPP_BUILTIN_KEYWORD_MASK_2 (TPP_BUILTIN_KEYWORD_MASK_1 | (TPP_BUILTIN_KEYWORD_MASK_1 >> 8))
#define TPP_BUILTIN_KEYWORD_MASK_3 (TPP_BUILTIN_KEYWORD_MASK_2 | (TPP_BUILTIN_KEYWORD_MASK_2 >> 4))
#define TPP_BUILTIN_KEYWORD_MASK_4 (TPP_BUILTIN_KEYWORD_MASK_3 | (TPP_BUILTIN_KEYWORD_MASK_3 >> 2))
#define TPP_BUILTIN_KEYWORD_MASK_5 (TPP_BUILTIN_KEYWORD_MASK_4 | (TPP_BUILTIN_KEYWORD_MASK_4 >> 1))
enum { TPP_BUILTIN_KEYWORD_MASK = TPP_BUILTIN_KEYWORD_MASK_5 };
#define TPP_BUILTIN_KEYWORD_MASK TPP_BUILTIN_KEYWORD_MASK
TPP_STATIC_ASSERT(tpp_lengthof(tpp_builtin_keywords_list) == (TPP_BUILTIN_KEYWORD_COUNT + 1));

static tpp_keyword *tpp_builtin_keyword_table[TPP_BUILTIN_KEYWORD_MASK + 1];

static void tpp_init_builtin_keywords_impl(void) {
	tpp_size i;
	for (i = 0; i < TPP_BUILTIN_KEYWORD_COUNT; ++i) {
		tpp_keyword *kwd = tpp_builtin_keywords_list[i];
		tpp_keyword **bucket;
#ifndef TPP_HASHOF
		kwd->tk_hash = tpp_hashof(kwd->tk_kwd, kwd->tk_len);
#endif /* !TPP_HASHOF */
		bucket = &tpp_builtin_keyword_table[kwd->tk_hash & TPP_BUILTIN_KEYWORD_MASK];
		kwd->tk_next = *bucket;
		*bucket = kwd;
	}
}

static void tpp_init_builtin_keywords(void) {
	tpp_once({
		tpp_init_builtin_keywords_impl();
	});
}
#else /* !TPP_CONFIG_BUILTINS_FILENAME */

#if TPP_SIZEOF_tpp_hash == 4
#define TPP_BUILTIN_MAKEHASH(hash_hi, hash_lo) UINT32_C(0x##hash_lo)
#elif TPP_SIZEOF_tpp_hash == 8
#define TPP_BUILTIN_MAKEHASH(hash_hi, hash_lo) UINT64_C(0x##hash_hi##hash_lo)
#else /* TPP_SIZEOF_tpp_hash == ... */
#endif /* TPP_SIZEOF_tpp_hash != ... */
#define TPP_BUILTIN_KEYWORD(id, kwd_len, kwd, next, hash_hi, hash_lo) \
	static struct tpp_builtin_keyword_struct_##id {                   \
		tpp_token_id         tk_id;                                   \
		tpp_keyword         *tk_next;                                 \
		_TPP_BUILTIN_KEYWORD_tk_macro_DEF                             \
		_TPP_BUILTIN_KEYWORD_tk_misc_DEF                              \
		tpp_hash             tk_hash;                                 \
		_TPP_BUILTIN_KEYWORD_tk_refcnt_DEF                            \
		tpp_size             tk_len;                                  \
		char                 tk_kwd[kwd_len + 1];                     \
	} tpp_builtin_keyword_##id = {                                    \
		/* .tk_id        = */ id,                                     \
		/* .tk_next      = */ (tpp_keyword *)next,                    \
		_TPP_BUILTIN_KEYWORD_tk_macro_INIT                            \
		_TPP_BUILTIN_KEYWORD_tk_misc_INIT                             \
		/* .tk_hash      = */ TPP_BUILTIN_MAKEHASH(hash_hi, hash_lo), \
		_TPP_BUILTIN_KEYWORD_tk_refcnt_INIT                           \
		/* .tk_len       = */ kwd_len,                                \
		/* .tk_kwd       = */ kwd                                     \
	};
#define TPP_BUILTIN_KEYWORD_TABLE_ITEM(k)    (tpp_keyword const *)k,
#define TPP_BUILTIN_KEYWORD_TABLE_ITEM_BLANK NULL,
#define TPP_BUILTIN_KEYWORD_TABLE(items) \
	static tpp_keyword const *tpp_builtin_keyword_table[TPP_BUILTIN_KEYWORD_MASK + 1] = { items };

#if TPP_HAVE_EXTENSIONS
#define TPP_BUILTIN_EXTENSION_NAMES(names) \
	static tpp_size const tpp_extension_name_offsets_byname[TPP_EXT_COUNT] = { names };
#define TPP_BUILTIN_EXTENSION_NAME(id, name) tpp_offsetof(struct tpp_extension_names_struct, ten_##id),
#else /* TPP_HAVE_EXTENSIONS */
#define TPP_BUILTIN_EXTENSION_NAMES(names)   /* nothing */
#define TPP_BUILTIN_EXTENSION_NAME(id, name) /* nothing */
#endif /* !TPP_HAVE_EXTENSIONS */

#if TPP_HAVE_WARNINGS
#define TPP_BUILTIN_WARNING_GROUP_NAMES(names) \
	static tpp_size const tpp_warning_group_name_offsets_byname[/*TPP_WGN_COUNT*/] = { names };
#define TPP_BUILTIN_WARNING_GROUP_NAME(id, index, name) tpp_offsetof(struct tpp_warning_group_names_struct, twgn_##id##_##index),
#else /* TPP_HAVE_WARNING_GROUPS */
#define TPP_BUILTIN_WARNING_GROUP_NAMES(names)          /* nothing */
#define TPP_BUILTIN_WARNING_GROUP_NAME(id, index, name) /* nothing */
#endif /* !TPP_HAVE_EXTENSIONS */

/* HINT: This file gets generated by "make-amalgamation-builtins.dee" */
#include TPP_CONFIG_BUILTINS_FILENAME

#undef TPP_BUILTIN_MAKEHASH
#undef TPP_BUILTIN_KEYWORD
#undef TPP_BUILTIN_KEYWORD_TABLE_ITEM
#undef TPP_BUILTIN_KEYWORD_TABLE_ITEM_BLANK
#undef TPP_BUILTIN_KEYWORD_TABLE
#undef TPP_BUILTIN_EXTENSION_NAMES
#undef TPP_BUILTIN_EXTENSION_NAME
#undef TPP_BUILTIN_WARNING_GROUP_NAMES
#undef TPP_BUILTIN_WARNING_GROUP_NAME

#define tpp_init_builtin_keywords()                  (void)0
#define tpp_init_extension_name_offsets_byname()     (void)0
#define tpp_init_warning_group_name_offsets_byname() (void)0
#endif /* TPP_CONFIG_BUILTINS_FILENAME */


/* Assert that generated stuff is valid */
#if TPP_HAVE_USER_KEYWORDS
TPP_STATIC_ASSERT(TPP_BUILTIN_KEYWORD_COUNT == (TPP_TOK_USERKEYWORD_BEGIN - TPP_TOK_KEYWORD_BEGIN));
#else /* TPP_HAVE_USER_KEYWORDS */
TPP_STATIC_ASSERT(TPP_BUILTIN_KEYWORD_COUNT == (TPP_TOK_USERKEYWORD - TPP_TOK_KEYWORD_BEGIN));
#endif /* !TPP_HAVE_USER_KEYWORDS */
TPP_STATIC_ASSERT((TPP_BUILTIN_KEYWORD_COUNT == 0) || TPP_BUILTIN_KEYWORD_MASK >= (TPP_BUILTIN_KEYWORD_COUNT + 1));
TPP_STATIC_ASSERT(tpp_lengthof(tpp_builtin_keyword_table) == (TPP_BUILTIN_KEYWORD_MASK + 1));


/* Lookup one of the built-in, pre-defined keywords */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword(tpp_char const *tpp_restrict kwd,
                       tpp_size len, tpp_hash hash) {
	tpp_keyword const *result;
	tpp_init_builtin_keywords();
	result = tpp_builtin_keyword_table[hash & TPP_BUILTIN_KEYWORD_MASK];
	for (; result; result = result->tk_next) {
		if (result->tk_hash != hash)
			continue;
		if (result->tk_len != len)
			continue;
		if (tpp_memcmp(result->tk_kwd, kwd, len * sizeof(tpp_char)) == 0)
			break;
	}
	return result;
}

TPP_IMPL TPP_WUNUSED tpp_keyword const *TPPCALL
tpp_builtin_getkeyword_byid(enum tpp_token_id id) {
	tpp_init_builtin_keywords();
	if (TPP_TOK_ISBUILTINKEYWORD(id)) {
#ifdef tpp_builtin_keywords_list
		return tpp_builtin_keywords_list[id - TPP_TOK_KEYWORD_BEGIN];
#else  /* tpp_builtin_keywords_list */
		tpp_hash i;
		for (i = 0; i <= TPP_BUILTIN_KEYWORD_MASK; ++i) {
			tpp_keyword const *chain = tpp_builtin_keyword_table[i];
			for (; chain; chain = chain->tk_next) {
				if (chain->tk_id == id)
					return chain;
			}
		}
#endif /* !tpp_builtin_keywords_list */
	}
	return NULL;
}

#if TPP_HAVE_ESCAPED_KEYWORDS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword_esc_(tpp_char const *tpp_restrict kwd,
                            tpp_size len, tpp_hash hash
                            tpp_bse_file__PARAM) {
	tpp_keyword const *result;
	tpp_init_builtin_keywords();
	result = tpp_builtin_keyword_table[hash & TPP_BUILTIN_KEYWORD_MASK];
	for (; result; result = result->tk_next) {
		if (result->tk_hash != hash)
			continue;
		if (tpp_memcmp_esc(result->tk_kwd,
		                   result->tk_len,
		                   kwd, len, file) == 0)
			break;
	}
	return result;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */



#if TPP_HAVE_KEYWORD_FLAGS
/* Return the effective set of flags for a given "kwd"
 * Since the effective flags for (certain) builtin keywords
 * can depend on active extensions/features, this can only
 * be done in the context of a specific lexer (rather than
 * stand-alone using only the "kwd")
 *
 * @return: * : Set of `TPP_KEYWORD_FLAG_*' */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword_flags TPPCALL
tpp_lexer_getkeywordflags(tpp_lexer *tpp_restrict self,
                          tpp_keyword const *tpp_restrict kwd) {
	/* Special handling for built-in keywords */
	if (TPP_TOK_ISBUILTINKEYWORD(kwd->tk_id)) {
		(void)self;
		switch (kwd->tk_id) {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_KWD_FLAGS(id, flags_expr) \
		case id: return flags_expr;
#define tpp_current_lexer()      self
#define tpp_current_keyword()    kwd
#define tpp_current_keyword_id() kwd->tk_id
#include TPP_CONFIG_DEFS_FILENAME
#undef tpp_current_lexer
#undef tpp_current_keyword
#undef tpp_current_keyword_id
#undef TPP_DEFS
		default: break;
		}
	} else if (kwd->tk_misc != NULL) {
		return kwd->tk_misc->tkm_flags;
	}
	return TPP_KEYWORD_FLAG_NORMAL;
}
#endif /* TPP_HAVE_KEYWORD_FLAGS */

#if TPP_HAVE_LEXER_GETKEYWORDDEFINED
/* Returns true if "kwd" should be considered to be "#if defined()"
 * Since "builtin" keywords can be considered to be "defined", even
 * when `kwd->tk_macro == NULL', this function is needed to handle
 * such macros. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_getkeyworddefined(tpp_lexer *tpp_restrict self,
                            tpp_keyword const *tpp_restrict kwd) {
#if TPP_HAVE_CPP_MACROS
	if (kwd->tk_macro != NULL)
		return true;
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_CPP_BUILTIN_MACROS
#if TPP_CONF_IS_RT(TPP_HAVE_CPP_BUILTIN_MACROS)
	if (!tpp_lexer_has(self, CPP_BUILTIN_MACROS))
		return false;
#endif /* TPP_CONF_IS_RT(TPP_HAVE_CPP_BUILTIN_MACROS) */
	(void)self;
	switch (kwd->tk_id) {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_MACRO(id, if_expr) \
	case id: return if_expr;
#define tpp_current_lexer()      self
#define tpp_current_keyword()    kwd
#define tpp_current_keyword_id() kwd->tk_id
#include TPP_CONFIG_DEFS_FILENAME
#undef tpp_current_lexer
#undef tpp_current_keyword
#undef tpp_current_keyword_id
#undef TPP_DEFS
	default: break;
	}
#endif /* TPP_HAVE_CPP_BUILTIN_MACROS */
	return false;
}
#endif /* TPP_HAVE_LEXER_GETKEYWORDDEFINED */


#if TPP_HAVE_EXTENSIONS
/* Convert between extension IDs and their human-readable names. */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_extension_getname(tpp_extension_id id) {
	if ((unsigned int)id >= (unsigned int)TPP_EXT_COUNT)
		return NULL;
	return tpp_extension_getname_fast((unsigned int)id);
}

static TPP_WUNUSED tpp_extension_id TPPCALL
tpp_extension_byname_offset(tpp_size name_offset) {
	unsigned int lo, hi;
	lo = 0;
	hi = tpp_lengthof(tpp_extension_name_offsets_byid);
	for (;;) {
		unsigned int mid = (lo + hi) / 2;
		tpp_size mid_offset = tpp_extension_name_offsets_byid[mid];
		tpp_assert(lo < hi);
		if (name_offset < mid_offset) {
			hi = mid;
		} else if (name_offset > mid_offset) {
			lo = mid + 1;
		} else {
			return (tpp_extension_id)mid;
		}
	}
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_extension_id TPPCALL
tpp_extension_byname_ex(char const *tpp_restrict name, tpp_size name_maxlen) {
	unsigned int lo, hi;
	tpp_init_extension_name_offsets_byname();
	name_maxlen = tpp_strnlen(name, name_maxlen);
	lo = 0;
	hi = tpp_lengthof(tpp_extension_name_offsets_byname);
	while (lo < hi) {
		unsigned int mid = (lo + hi) / 2;
		tpp_size mid_offset = tpp_extension_name_offsets_byname[mid];
		char const *mid_name = (char const *)&tpp_extension_names + mid_offset;
		int cmp = tpp_memcmp(name, mid_name, name_maxlen * sizeof(char));
		if (cmp == 0 && mid_name[name_maxlen])
			cmp = -1;
		if (cmp < 0) {
			hi = mid;
		} else if (cmp > 0) {
			lo = mid + 1;
		} else {
			/* Found offset into name-table. Now find the matching offset in
			 * "tpp_extension_name_offsets_byid" (which is sorted by said offset),
			 * at which point the index into "tpp_extension_name_offsets_byid"
			 * will be the extension id. */
			return tpp_extension_byname_offset(mid_offset);
		}
	}
	return TPP_EXT_COUNT;
}

#if TPP_HAVE_TPP_EXTENSION_NEAREST
/* Returns the ID of the extension with the name that is closest to "name"
 * When no extensions are defined (at all), this will return "TPP_EXT_COUNT" */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_extension_id TPPCALL
tpp_extension_nearest_ex(char const *tpp_restrict name, tpp_size name_maxlen) {
	tpp_extension_id xid;
	tpp_extension_id result = TPP_EXT_COUNT;
	tpp_size result_fuzzy = TPP_SIZE_MAX;
	tpp_size name_len = tpp_strnlen(name, name_maxlen);
	for (xid = (tpp_extension_id)0; (unsigned int)xid < (unsigned int)TPP_EXT_COUNT;
	     xid = (tpp_extension_id)((unsigned int)xid + 1)) {
		char const *xname = tpp_extension_getname_fast(xid);
		tpp_size xlen = tpp_strlen(xname);
		tpp_size fuzzy = tpp_fuzzy_memcmp((tpp_char const *)name, name_len,
		                                  (tpp_char const *)xname, xlen);
		if (result_fuzzy > fuzzy) {
			result_fuzzy = fuzzy;
			result       = xid;
		}
	}
	return result;
}
#endif /* TPP_HAVE_TPP_EXTENSION_NEAREST */
#endif /* TPP_HAVE_EXTENSIONS */



#if TPP_HAVE_WARNINGS

static struct tpp_warning_format_strings_struct {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	char twf_##warning_id[sizeof(format) / sizeof(char)];
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr)
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
} const tpp_warning_format_strings = {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	/* [twf_##warning_id] = */ format,
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr)
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
};

static tpp_size const tpp_warning_format_string_offsets[TPP_W_COUNT] = {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	/* [warning_id] = */ tpp_offsetof(struct tpp_warning_format_strings_struct, twf_##warning_id),
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr) \
	/* [warning_id] = */ sizeof(struct tpp_warning_format_strings_struct) - 1,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
};

/* Returns the "tpp_lexer_printf_warning"-style format string assigned with "id".
 * When "id" is TPP_W_COUNT, invalid, or declared as "TPP_WARNING_EX", return "NULL" instead. */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_warning_getformat(tpp_warning_id id) {
	tpp_size offset;
	char const *result;
	if ((unsigned int)id >= (unsigned int)TPP_W_COUNT)
		return NULL;
	offset = tpp_warning_format_string_offsets[(unsigned int)id];
	result = (char const *)&tpp_warning_format_strings + offset;
	if (*result == '\0')
		result = NULL;
	return result;
}


#if TPP_HAVE_WARNING_NUMBERS
/* Returns the warning ID linked to a given "number", or "TPP_W_COUNT" if "number" is unknown */
TPP_IMPL TPP_WUNUSED tpp_warning_id TPPCALL
tpp_warning_ofnumber(unsigned int number) {
	switch (number) {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_WARNING_NUMBER_CASE(_, index, value) \
	case value: return _;
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_FOREACH(numbers, TPP_TUPLE_FOREACH_DUMMY_SEP, TPP_WARNING_NUMBER_CASE, warning_id)
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_WARNING_NUMBER_CASE
#undef TPP_DEFS
	default: break;
	}
	return TPP_W_COUNT;
}

/* Returns the TPP_WARNING_NUMBER_INVALID-terminated list of warning numbers
 * associated with "warning_id". If "warning_id" doesn't have any warning
 * numbers, return a pointer to "TPP_WARNING_NUMBER_INVALID". */
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED unsigned int const *TPPCALL
tpp_warning_getnumbers(tpp_warning_id warning_id) {
	static unsigned int const dummy_warning_numbers[1] = { TPP_WARNING_NUMBER_INVALID };
	switch (warning_id) {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_WARNING_NUMBERS_CASE_ITEM(_, index, value) value,
#define TPP_WARNING_NUMBERS_CASE_(warning_id, numbers)                                                \
	case warning_id: {                                                                                \
		static unsigned int const this_warning_numbers[] = {                                          \
			TPP_TUPLE_FOREACH(numbers, TPP_TUPLE_FOREACH_DUMMY_SEP, TPP_WARNING_NUMBERS_CASE_ITEM, ~) \
			TPP_WARNING_NUMBER_INVALID                                                                \
		};                                                                                            \
		return this_warning_numbers;                                                                  \
	}
#define TPP_WARNING_NUMBERS_CASE(args) TPP_WARNING_NUMBERS_CASE_ args
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_IF_NONEMPTY(numbers, TPP_WARNING_NUMBERS_CASE, (warning_id, numbers))
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_WARNING_NUMBERS_CASE
#undef TPP_DEFS
	default: break;
	}
	return dummy_warning_numbers;
}


/* Returns the context-id of a given (should-be) "numbered" warning.
 * When "warning_id" is invalid, "TPP_W_COUNT", or not numbered, this
 * function will return "TPP_WC_COUNT" instead. */
TPP_IMPL TPP_WUNUSED tpp_warning_context_id TPPCALL
tpp_warning_context_id_ofwarning(tpp_warning_id warning_id) {
	switch (warning_id) {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define _TPP_CASE_NUMBERED_WARNING(warning_id) \
	case warning_id: return TPP_WC_##warning_id;
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_IF_NONEMPTY(numbers, _TPP_CASE_NUMBERED_WARNING, warning_id)
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_CASE_NUMBERED_WARNING
#undef TPP_DEFS
	default: break;
	}
	return TPP_WC_COUNT;
}

/* Returns the warning-id linked to "ctx_id", when `tpp_warning_context_id_isnumber(ctx_id)'.
 * When no warning is linked to "ctx_id" ("ctx_id" is either linked to a warning group, or
 * is "TPP_WC_COUNT" or some other invalid ID), return "TPP_W_COUNT" instead. */
TPP_IMPL TPP_WUNUSED tpp_warning_id TPPCALL
tpp_warning_context_id_aswarning(tpp_warning_context_id ctx_id) {
	switch (ctx_id) {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define _TPP_CASE_NUMBERED_WARNING(warning_id) \
	case TPP_WC_##warning_id: return warning_id;
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_IF_NONEMPTY(numbers, _TPP_CASE_NUMBERED_WARNING, warning_id)
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_CASE_NUMBERED_WARNING
#undef TPP_DEFS
	default: break;
	}
	return TPP_W_COUNT;
}
#endif /* TPP_HAVE_WARNING_NUMBERS */


static TPP_WUNUSED tpp_warning_group_id TPPCALL
tpp_warning_group_byname_offset(tpp_size name_offset) {
	unsigned int lo, hi;
	lo = 0;
	hi = tpp_lengthof(tpp_warning_group_name_offsets_byid);
	for (;;) {
		unsigned int mid = (lo + hi) / 2;
		tpp_size mid_offset = tpp_warning_group_name_offsets_byid[mid];
		tpp_assert(lo < hi);
		if (name_offset < mid_offset) {
			hi = mid;
		} else if (name_offset > mid_offset) {
			lo = mid + 1;
		} else {
			return (tpp_warning_group_id)mid;
		}
	}
}

/* @return: TPP_WG_COUNT: No such warning_group */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_group_id TPPCALL
tpp_warning_group_byname_ex(char const *tpp_restrict name, tpp_size name_maxlen) {
	unsigned int lo, hi;
	tpp_init_warning_group_name_offsets_byname();
	name_maxlen = tpp_strnlen(name, name_maxlen);
	lo = 0;
	hi = tpp_lengthof(tpp_warning_group_name_offsets_byname);
	while (lo < hi) {
		unsigned int mid = (lo + hi) / 2;
		tpp_size mid_offset = tpp_warning_group_name_offsets_byname[mid];
		char const *mid_name = (char const *)&tpp_warning_group_names + mid_offset;
		int cmp = tpp_memcmp(name, mid_name, name_maxlen * sizeof(char));
		if (cmp == 0 && mid_name[name_maxlen])
			cmp = -1;
		if (cmp < 0) {
			hi = mid;
		} else if (cmp > 0) {
			lo = mid + 1;
		} else {
			/* Found offset into name-table. Now find the matching offset in
			 * "tpp_warning_group_name_offsets_byid" (which is sorted by said offset),
			 * at which point the index into "tpp_warning_group_name_offsets_byid"
			 * will be the warning_group id. */
			return tpp_warning_group_byname_offset(mid_offset);
		}
	}
	return TPP_WG_COUNT;
}


#if TPP_HAVE_TPP_WARNING_GROUP_NEAREST
/* Returns the ID of the warning group with the name that is closest to "name"
 * When no warning groups are defined (at all), this will return "TPP_WG_COUNT" */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_group_id TPPCALL
tpp_warning_group_nearest_ex(char const *tpp_restrict name, tpp_size name_maxlen) {
	tpp_warning_group_id gid;
	tpp_warning_group_id result = TPP_WG_COUNT;
	tpp_size result_fuzzy = TPP_SIZE_MAX;
	tpp_size name_len = tpp_strnlen(name, name_maxlen);
	for (gid = (tpp_warning_group_id)0; (unsigned int)gid < (unsigned int)TPP_WG_COUNT;
	     gid = (tpp_warning_group_id)((unsigned int)gid + 1)) {
		char const *names = tpp_warning_group_getname_fast(gid);
		do {
			tpp_size len = tpp_strlen(names);
			tpp_size fuzzy = tpp_fuzzy_memcmp((tpp_char const *)name, name_len,
			                                  (tpp_char const *)names, len);
			if (result_fuzzy > fuzzy) {
				result_fuzzy = fuzzy;
				result       = gid;
			}
			names += len + 1;
		} while (*names);
	}
	return result;
}
#endif /* TPP_HAVE_TPP_WARNING_GROUP_NEAREST */
#endif /* TPP_HAVE_WARNINGS */


#if TPP_HAVE_CPP_MACROS
/* Return the hard-coded expansion of the builtin macro linked to "id".
 * If "id" isn't a builtin keyword, or that keyword doesn't specify a
 * value for "TPP_BUILTIN_MACRO()", return "NULL" instead. */
TPP_IMPL TPP_CONSTCALL TPP_WUNUSED tpp_builtin_macro const *TPPCALL
tpp_macro_getbuiltin(tpp_token_id id) {
	switch (id) {
#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_BUILTIN_MACRO(keyword_id, value)                          \
	case keyword_id: {                                                \
		static TPP_BUILTIN_MACRO_DEFINE(builtin_##keyword_id, value); \
		return (tpp_builtin_macro const *)&builtin_##keyword_id;      \
	}	break;
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	default: break;
	}
	return NULL;
}
#endif /* TPP_HAVE_CPP_MACROS */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_BUILTINS_C */
