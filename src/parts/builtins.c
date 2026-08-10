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
/*!depends config.h*/
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
#if !TPP_IGNORE_INVALID_CONFIGURATION
TPP_STATIC_ASSERT_MSG(TPP_EXT_COUNT != 0, "No extensions defined -- you should build with `-DTPP_HAVE_EXTENSIONS=0`");
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */

static struct tpp_extension_names_struct {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) char ten_##id[sizeof(name) / sizeof(char)];
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
} const tpp_extension_names = {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) /* .ten_##id = */ name,
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
};

static tpp_size const tpp_extension_name_offsets_byid[TPP_EXT_COUNT ? TPP_EXT_COUNT : 1] = {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) \
	/* [id] = */ tpp_offsetof(struct tpp_extension_names_struct, ten_##id),
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
};
#define tpp_extension_getname_fast(id) \
	((char const *)&tpp_extension_names + tpp_extension_name_offsets_byid[id])
#endif /* TPP_HAVE_EXTENSIONS */



#if TPP_HAVE_WARNINGS

static struct tpp_warning_group_names_struct {
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	char twgn_##wgroup_id##_##index[sizeof(value) / sizeof(char)];
#define TPP_WGROUP(wgroup_id, names, default)                                                  \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id) \
	char twgn_tail_##wgroup_id;
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
} const tpp_warning_group_names = {
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	/* .twgn_##wgroup_id##_##index = */ value,
#define TPP_WGROUP(wgroup_id, names, default)                                                  \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id) \
	/* .twgn_tail_##wgroup_id = */ 0,
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
};

enum {
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	_TPP_WGN_##wgroup_id##_##index,
#define TPP_WGROUP(wgroup_id, names, default) \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id)
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
	TPP_WGN_COUNT
};


/* Convert WARNING_GROUP_NAME_ID -> tpp_warning_group_id */
static tpp_warning_group_id const tpp_warning_group_bynameid[TPP_WGN_COUNT] = {
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	/* [_TPP_WGN_##wgroup_id##_##index] = */ wgroup_id,
#define TPP_WGROUP(wgroup_id, names, default) \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id)
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
};

/* Convert WARNING_GROUP_NAME_ID -> offset-into-tpp_warning_group_names */
static tpp_size const tpp_warning_group_name_offsets_bynameid[TPP_WGN_COUNT] = {
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	/* [_TPP_WGN_##wgroup_id##_##index] = */ tpp_offsetof(struct tpp_warning_group_names_struct, twgn_##wgroup_id##_##index),
#define TPP_WGROUP(wgroup_id, names, default) \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id)
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
};

/* Convert tpp_warning_group_id -> offset-into-tpp_warning_group_names */
static tpp_size const tpp_warning_group_name_offsets_byid[TPP_WG_COUNT] = {
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) \
	/* [wgroup_id] = */ tpp_offsetof(struct tpp_warning_group_names_struct, twgn_##wgroup_id##_0),
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
};
#define tpp_warning_group_getname_fast(id) \
	((char const *)&tpp_warning_group_names + tpp_warning_group_name_offsets_byid[id])


/* Return a pointer to a `\0\0`-terminated list of strings describing the names
 * of for the given warning group `id`. Returns `NULL` if `id` is `TPP_WG_COUNT`
 * or some other invalid warning group ID. */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_warning_group_getnames(tpp_warning_group_id id) {
	if ((unsigned int)id >= (unsigned int)TPP_WG_COUNT)
		return NULL;
	return tpp_warning_group_getname_fast((unsigned int)id);
}


static struct tpp_warning_groups_struct {
#define TPP_DEFS
#define _TPP_EXPAND_WARNING_GROUP_IDS(warning_id, index, value) \
	tpp_warning_group_id twig_##warning_id##_##index;
#define _TPP_EXPAND_WARNING_GROUP_TAIL(warning_id) \
	tpp_warning_group_id twig_tail_##warning_id;
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format)                             \
	TPP_TUPLE_FOREACH(wgroup_ids, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WARNING_GROUP_IDS, warning_id) \
	TPP_TUPLE_IF_NONEMPTY(wgroup_ids, _TPP_EXPAND_WARNING_GROUP_TAIL, warning_id)
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WARNING_GROUP_IDS
#undef _TPP_EXPAND_WARNING_GROUP_TAIL
#undef TPP_DEFS
} const tpp_warning_groups = {
#define TPP_DEFS
#define _TPP_EXPAND_WARNING_GROUP_IDS(warning_id, index, value) \
	/* .twig_##warning_id##_##index = */ value,
#define _TPP_EXPAND_WARNING_GROUP_TAIL(warning_id) \
	/* .twig_tail_##warning_id = */ TPP_WG_COUNT,
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format)                             \
	TPP_TUPLE_FOREACH(wgroup_ids, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WARNING_GROUP_IDS, warning_id) \
	TPP_TUPLE_IF_NONEMPTY(wgroup_ids, _TPP_EXPAND_WARNING_GROUP_TAIL, warning_id)
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WARNING_GROUP_IDS
#undef _TPP_EXPAND_WARNING_GROUP_TAIL
#undef TPP_DEFS
};

/* Convert tpp_warning_id -> offset-into-tpp_warning_groups */
static tpp_size const tpp_warning_group_offsets_byid[TPP_W_COUNT] = {
#define TPP_DEFS
#define _TPP_WARNING_GROUPS_NONEMPTY(warning_id) \
	/* [warning_id] = */ tpp_offsetof(struct tpp_warning_groups_struct, twig_##warning_id##_0),
#define _TPP_WARNING_GROUPS_EMPTY(warning_id) \
	/* [warning_id] = */ sizeof(struct tpp_warning_groups_struct) - sizeof(tpp_warning_group_id),
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format)   \
	TPP_TUPLE_IF_NONEMPTY(wgroup_ids, _TPP_WARNING_GROUPS_NONEMPTY, warning_id) \
	TPP_TUPLE_IF_EMPTY(wgroup_ids, _TPP_WARNING_GROUPS_EMPTY, warning_id)
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_WARNING_GROUPS_NONEMPTY
#undef _TPP_WARNING_GROUPS_EMPTY
#undef TPP_DEFS
};
#define tpp_warning_groups_fast(id) \
	((tpp_warning_group_id const *)((char const *)&tpp_warning_groups + tpp_warning_group_offsets_byid[id]))


/* Returns a `TPP_WG_COUNT`-terminated list of group IDs associated with the given warning `id`.
 * When the given `id` is `TPP_W_COUNT` or invalid, return a pointer to an empty warning-group-id-list. */
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED tpp_warning_group_id const *TPPCALL
tpp_warning_getgroups(tpp_warning_id id) {
	if ((unsigned int)id >= (unsigned int)TPP_W_COUNT) {
		return (tpp_warning_group_id const *)((char const *)&tpp_warning_groups +
		                                      sizeof(tpp_warning_groups) -
		                                      sizeof(tpp_warning_group_id));
	}
	return tpp_warning_groups_fast((unsigned int)id);
}

TPP_CONST_IMPL tpp_warnings_state const tpp_warnings_state_default = {
	/* .tws_state = */ {
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) \
		/* .twsg_##wgroup_id  = */ (unsigned int)default,
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#if TPP_HAVE_WARNING_NUMBERS
#define TPP_DECLARE_NUMBERED_WARNING(numbers_default) \
		/* .twsn_##warning_id = */ (unsigned int)numbers_default,
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
		TPP_TUPLE_IF_NONEMPTY(numbers, TPP_DECLARE_NUMBERED_WARNING, numbers_default)
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DECLARE_NUMBERED_WARNING
#endif /* TPP_HAVE_WARNING_NUMBERS */
#undef TPP_DEFS
	}
};

#endif /* TPP_HAVE_WARNINGS */


/* This macro (`TPP_CONFIG_BUILTINS_FILENAME`) should be defined as `"defs-cache.inl"`
 * for TPP to use your pre-generated cache of builtin definitions. For more info, see:
 * - `/src/makecache.py`
 * - `/samples/simple-with-cache/defs-cache.inl`
 * - `/samples/simple-with-cache/Makefile`
 */
#ifdef TPP_CONFIG_BUILTINS_FILENAME
#if TPP_SIZEOF_tpp_hash == 4
#define TPP_BUILTIN_MAKEHASH(hash_hi, hash_lo) UINT32_C(0x##hash_lo)
#elif TPP_SIZEOF_tpp_hash == 8
#define TPP_BUILTIN_MAKEHASH(hash_hi, hash_lo) UINT64_C(0x##hash_hi##hash_lo)
#else /* TPP_SIZEOF_tpp_hash == ... */
#endif /* TPP_SIZEOF_tpp_hash != ... */
#define TPP_BUILTIN_KEYWORD(id, kwd_len, kwd, next, hash_hi, hash_lo) \
	static struct tpp_builtin_keyword_struct_##id {                   \
		tpp_token_id         tk_id;                                   \
		_TPP_BUILTIN_KEYWORD_tk_macro_DEF                             \
		_TPP_BUILTIN_KEYWORD_tk_misc_DEF                              \
		tpp_hash             tk_hash;                                 \
		tpp_keyword         *tk_next;                                 \
		_TPP_BUILTIN_KEYWORD_tk_refcnt_DEF                            \
		tpp_size             tk_len;                                  \
		char                 tk_kwd[kwd_len + 1];                     \
	} tpp_builtin_keyword_##id = {                                    \
		/* .tk_id        = */ id,                                     \
		_TPP_BUILTIN_KEYWORD_tk_macro_INIT                            \
		_TPP_BUILTIN_KEYWORD_tk_misc_INIT                             \
		/* .tk_hash      = */ TPP_BUILTIN_MAKEHASH(hash_hi, hash_lo), \
		/* .tk_next      = */ (tpp_keyword *)next,                    \
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

/* HINT: This file gets generated by `makecache.py` */
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
#elif defined(__TPP_VERSION__) && __TPP_VERSION__ >= 300 && 0
/* Using a combination of __TPP_EVAL + #pragma tpp_exec() + __TPP_EXEC + __TPP_STR_DECOMPILE,
 * it should be possible to do pretty much anything within the preprocessor, including
 * generating builtin tables right. */

#define _TPP_EXEC_INCLUDE2(x) __TPP_EXEC("#include " #x)
#define _TPP_EXEC_INCLUDE(x) _TPP_EXEC_INCLUDE2(x)
#define _TPP_STR2(x) #x
#define _TPP_STR(x) _TPP_STR2(x)

/* Define a macro `TPP_BUILTIN_KEYWORD_COUNT` as a singular decimal token
 * representing the # of times the `TPP_KWD` is used in `TPP_CONFIG_DEFS_FILENAME` */
#define TPP_DEFS
#define TPP_KWD(id, string) +1
#undef GUARD_TPP_AMALGAMATION_H
__pragma(tpp_exec("#define TPP_BUILTIN_KEYWORD_COUNT " _TPP_STR(__TPP_EVAL(
	_TPP_EXEC_INCLUDE(TPP_CONFIG_DEFS_FILENAME)
))))
#undef TPP_DEFS

/* XXX: Calculate "TPP_BUILTIN_KEYWORD_MASK" */
/* XXX: For every TPP_KWD, calculate its hash masked by "TPP_BUILTIN_KEYWORD_MASK".
 *      Then, define a macro "TPP_BUILTIN_KEYWORD_H_<MASKED_HASH>" that points to
 *      the keyword that should appear in the keyword table at offset <MASKED_HASH>
 *      In those places where that macro has already been defined, override the
 *      macro, but use its own definition as the "tk_next" pointer of the keyword
 *      that is overriding it */
/* XXX: Generate keyword structures, but in those places where the keyword's "tk_next"
 *      is non-NULL, make sure that the pointed-to keyword is generated first. */
/* XXX: Emit the hash-table, using "TPP_BUILTIN_KEYWORD_H_<I>" for index "I". If
 *      no such macro is defined, that index's bucket must be initialized as "NULL"
 *      instead */

/* XXX: For extension/warning names, need some kind of mechanism by which TPP is
 *      able to sort an array of strings. I'm sure it's possible somehow, but I'm not
 *      quite certain on how this can be done most elegantly (an in O(N*log(N)) time)
 * -> I feel like it should be possible to implement MergeSort using macros:
 *    - Can split items in half using __TPP_EVAL(__VA_NARGS__ / 2)
 *    - Once there are <=2 items, can sort inplace
 *    - To re-merge sorted arrays, can use 2 sets of CURRENT_ITEM+ADVANCE_ITEM macros
 * With this, it should be possible to get a macro:
 * >> #define TPP_QSORT(items, pred_lo) ...
 * >>
 * >> #define my_prev_lo(a, b) __TPP_EVAL((a) < (b))
 * >> TPP_QSORT((20, 10, (11+2), 99, 14), my_prev_lo) // Expands to 10,(11+2),14,20,99
 */

#undef _TPP_EXEC_INCLUDE2
#undef _TPP_EXEC_INCLUDE
#undef _TPP_STR2
#undef _TPP_STR

#else /* ... */

#if TPP_HAVE_EXTENSIONS
static tpp_size tpp_extension_name_offsets_byname[TPP_EXT_COUNT ? TPP_EXT_COUNT : 1] = {};
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
	qsort(tpp_warning_group_name_offsets_byname, TPP_WGN_COUNT, sizeof(tpp_size),
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
#pragma warning(disable: 108) /* Index out-of-bounds in `__TPP_EVAL` */
#endif /* __TPP_VERSION__ < 300 */

/* Using some sick-a$$ TPP extensions, we can actually
 * calculate keyword hashes within the preprocessor! */
#pragma extension(push,"-fmacro-recursion")
#if TPP_SIZEOF_tpp_hash == 4
#define TPP_PRIVATE_HASHOF_1(result,str) TPP_PRIVATE_HASHOF2(__TPP_EVAL(tpp_hash_combine_char(result,str[0])&TPP_HASH_C(0xffffffff)),__TPP_EVAL(str[1:]))
#elif TPP_SIZEOF_tpp_hash == 8
#define TPP_PRIVATE_HASHOF_1(result,str) TPP_PRIVATE_HASHOF2(__TPP_EVAL(tpp_hash_combine_char(result,str[0])&TPP_HASH_C(0xffffffffffffffff)),__TPP_EVAL(str[1:]))
#endif /* ... */
#define TPP_PRIVATE_HASHOF2(result,str) TPP_PRIVATE_PP_CAT(TPP_PRIVATE_HASHOF_,__TPP_EVAL(!!str))(result,str)
#pragma extension(pop)
#if TPP_SIZEOF_tpp_hash == 4
#define TPP_HASHOF(str) TPP_HASH_C(TPP_PRIVATE_HASHOF2(TPP_HASH_INITIAL,str))
#elif TPP_SIZEOF_tpp_hash == 8
#define TPP_HASHOF(str) TPP_HASH_C(TPP_PRIVATE_HASHOF2(TPP_HASH_INITIAL,str))
#endif /* ... */
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
		_TPP_BUILTIN_KEYWORD_tk_macro_DEF                        \
		_TPP_BUILTIN_KEYWORD_tk_misc_DEF                         \
		tpp_hash             tk_hash;                            \
		tpp_keyword         *tk_next;                            \
		_TPP_BUILTIN_KEYWORD_tk_refcnt_DEF                       \
		tpp_size             tk_len;                             \
		char                 tk_kwd[sizeof(str) / sizeof(char)]; \
	} tpp_builtin_keyword_##id = {                               \
		/* .tk_id        = */ id,                                \
		_TPP_BUILTIN_KEYWORD_tk_macro_INIT                       \
		_TPP_BUILTIN_KEYWORD_tk_misc_INIT                        \
		/* .tk_hash      = */ TPP_MAYBE_HASHOF(str),             \
		/* .tk_next      = */ NULL,                              \
		_TPP_BUILTIN_KEYWORD_tk_refcnt_INIT                      \
		/* .tk_len       = */ (sizeof(str) / sizeof(char)) - 1,  \
		/* .tk_kwd       = */ str                                \
	};

#define TPP_DEFS
#define TPP_KWD(id, string) \
	TPP_DEFINE_BUILTIN_KEYWORD(id, string)
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS

#define tpp_builtin_keywords_list tpp_builtin_keywords_list
static tpp_keyword *tpp_builtin_keywords_list[] = {
#define TPP_DEFS
#define TPP_KWD(id, string) (tpp_keyword *)&tpp_builtin_keyword_##id,
#undef GUARD_TPP_AMALGAMATION_H
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
#endif /* !... */


/* Assert that generated stuff is valid */
#if TPP_HAVE_USER_KEYWORDS
TPP_STATIC_ASSERT(TPP_BUILTIN_KEYWORD_COUNT == (TPP_TOK_USERKEYWORD_BEGIN - TPP_TOK_KEYWORD_BEGIN));
#else /* TPP_HAVE_USER_KEYWORDS */
TPP_STATIC_ASSERT(TPP_BUILTIN_KEYWORD_COUNT == (TPP_TOK_USERKEYWORD - TPP_TOK_KEYWORD_BEGIN));
#endif /* !TPP_HAVE_USER_KEYWORDS */
TPP_STATIC_ASSERT((TPP_BUILTIN_KEYWORD_COUNT == 0) || TPP_BUILTIN_KEYWORD_MASK >= (TPP_BUILTIN_KEYWORD_COUNT + 1));
TPP_STATIC_ASSERT(tpp_lengthof(tpp_builtin_keyword_table) == (TPP_BUILTIN_KEYWORD_MASK + 1));


/* Lookup one of the built-in, predefined keywords */
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
                            _tpp_esc_lexer__PARAM) {
	tpp_keyword const *result;
	tpp_init_builtin_keywords();
	result = tpp_builtin_keyword_table[hash & TPP_BUILTIN_KEYWORD_MASK];
	for (; result; result = result->tk_next) {
		if (result->tk_hash != hash)
			continue;
		if (tpp_memcmp_esc(result->tk_kwd,
		                   result->tk_len,
		                   kwd, len, lexer) == 0)
			break;
	}
	return result;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Shared API for `tpp_lexer_getkeywordfeature()` and `tpp_lexer_getpredefinedmacro()` */
#define tpp_current_lexer()      _self
#define tpp_current_keyword()    _kwd
#define tpp_current_keyword_id() _kwd->tk_id
#define tpp_current_expansion()  _result
#define tpp_return_conststr(CONSTstr) return (tpp_macro_expansion_init_conststr(tpp_current_expansion(), CONSTstr), TPP_EOK)
#define tpp_return_bool(is_enabled)   return (tpp_macro_expansion_init_cstr(tpp_current_expansion(), &"01"[!!(is_enabled)], 1), TPP_EOK)

#if TPP_HAVE_LEXER_GETKEYWORDFEATURE
/* Return the effective expansion of a feature-keyword `kwd`.
 * @param: feature_kind: One of `TPP_KWD___has_attribute`, `TPP_KWD___has_feature`, etc.
 * @return: TPP_EOK:    Success: expansion was stored in `result`
 * @return: TPP_ENOENT: SOFT_ERROR: keyword has no expansion (caller should expand to `0` instead)
 * @return: TPP_E*:     HARD_ERROR: error returned by a user-defined feature-test callback. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_getkeywordfeature(tpp_lexer *tpp_restrict _self,
                            tpp_keyword const *tpp_restrict _kwd,
                            tpp_token_id _feature_kind,
                            tpp_macro_expansion *tpp_restrict _result) {
	(void)_self;
	(void)_kwd;
	(void)_feature_kind;
	(void)_result;

	/* Switch on different feature kinds */
	switch (_feature_kind) {
/*[[[deemon
import KEYWORD_FEATURE_KINDS from .config;
for (local kind: KEYWORD_FEATURE_KINDS) {
	local KIND = kind.upper();
	print("#if TPP_HAVE_CLANG_MACRO___", kind);
	print("	case TPP_KWD___", kind, ":");
	print("#if TPP_HAVE_KEYWORD_FEATURE_", KIND);
	print("		{");
	print("			TPP_REF tpp_string *keyword_feature;");
	print("			keyword_feature = tpp_keyword_getfeature(_kwd, TPP_KEYWORD_FEATURE_KIND_", KIND, ");");
	print("			if (keyword_feature) {");
	print("				tpp_macro_expansion_init_chunk_inherited(_result, keyword_feature);");
	print("				return TPP_EOK;");
	print("			}");
	print("		}");
	print("#endif /" "* TPP_HAVE_KEYWORD_FEATURE_", KIND, " *" "/");
	print("		switch (tpp_keyword_getid(_kwd)) {");
	print("#define TPP_DEFS");
	print("#define TPP_PREDEFINED_FEATURE_", KIND, "(keyword_id, expansion_expr) \\");
	print("		case keyword_id: {  ", " " * #KIND, "                                \\");
	print("			expansion_expr; ", " " * #KIND, "                                \\");
	print("		}	break;");
	print("#undef GUARD_TPP_AMALGAMATION_H");
	print("#include TPP_CONFIG_DEFS_FILENAME");
	print("#undef TPP_DEFS");
	print("		default: break;");
	print("		}");
	print("		break;");
	print("#endif /* TPP_HAVE_CLANG_MACRO___", kind, " *" "/");
}
]]]*/
#if TPP_HAVE_CLANG_MACRO___has_attribute
	case TPP_KWD___has_attribute:
#if TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE
		{
			TPP_REF tpp_string *keyword_feature;
			keyword_feature = tpp_keyword_getfeature(_kwd, TPP_KEYWORD_FEATURE_KIND_HAS_ATTRIBUTE);
			if (keyword_feature) {
				tpp_macro_expansion_init_chunk_inherited(_result, keyword_feature);
				return TPP_EOK;
			}
		}
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE */
		switch (tpp_keyword_getid(_kwd)) {
#define TPP_DEFS
#define TPP_PREDEFINED_FEATURE_HAS_ATTRIBUTE(keyword_id, expansion_expr) \
		case keyword_id: {                                               \
			expansion_expr;                                              \
		}	break;
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
		default: break;
		}
		break;
#endif /* TPP_HAVE_CLANG_MACRO___has_attribute */
#if TPP_HAVE_CLANG_MACRO___has_builtin
	case TPP_KWD___has_builtin:
#if TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN
		{
			TPP_REF tpp_string *keyword_feature;
			keyword_feature = tpp_keyword_getfeature(_kwd, TPP_KEYWORD_FEATURE_KIND_HAS_BUILTIN);
			if (keyword_feature) {
				tpp_macro_expansion_init_chunk_inherited(_result, keyword_feature);
				return TPP_EOK;
			}
		}
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN */
		switch (tpp_keyword_getid(_kwd)) {
#define TPP_DEFS
#define TPP_PREDEFINED_FEATURE_HAS_BUILTIN(keyword_id, expansion_expr) \
		case keyword_id: {                                             \
			expansion_expr;                                            \
		}	break;
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
		default: break;
		}
		break;
#endif /* TPP_HAVE_CLANG_MACRO___has_builtin */
#if TPP_HAVE_CLANG_MACRO___has_cpp_attribute
	case TPP_KWD___has_cpp_attribute:
#if TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE
		{
			TPP_REF tpp_string *keyword_feature;
			keyword_feature = tpp_keyword_getfeature(_kwd, TPP_KEYWORD_FEATURE_KIND_HAS_CPP_ATTRIBUTE);
			if (keyword_feature) {
				tpp_macro_expansion_init_chunk_inherited(_result, keyword_feature);
				return TPP_EOK;
			}
		}
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE */
		switch (tpp_keyword_getid(_kwd)) {
#define TPP_DEFS
#define TPP_PREDEFINED_FEATURE_HAS_CPP_ATTRIBUTE(keyword_id, expansion_expr) \
		case keyword_id: {                                                   \
			expansion_expr;                                                  \
		}	break;
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
		default: break;
		}
		break;
#endif /* TPP_HAVE_CLANG_MACRO___has_cpp_attribute */
#if TPP_HAVE_CLANG_MACRO___has_declspec_attribute
	case TPP_KWD___has_declspec_attribute:
#if TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE
		{
			TPP_REF tpp_string *keyword_feature;
			keyword_feature = tpp_keyword_getfeature(_kwd, TPP_KEYWORD_FEATURE_KIND_HAS_DECLSPEC_ATTRIBUTE);
			if (keyword_feature) {
				tpp_macro_expansion_init_chunk_inherited(_result, keyword_feature);
				return TPP_EOK;
			}
		}
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE */
		switch (tpp_keyword_getid(_kwd)) {
#define TPP_DEFS
#define TPP_PREDEFINED_FEATURE_HAS_DECLSPEC_ATTRIBUTE(keyword_id, expansion_expr) \
		case keyword_id: {                                                        \
			expansion_expr;                                                       \
		}	break;
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
		default: break;
		}
		break;
#endif /* TPP_HAVE_CLANG_MACRO___has_declspec_attribute */
#if TPP_HAVE_CLANG_MACRO___has_extension
	case TPP_KWD___has_extension:
#if TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION
		{
			TPP_REF tpp_string *keyword_feature;
			keyword_feature = tpp_keyword_getfeature(_kwd, TPP_KEYWORD_FEATURE_KIND_HAS_EXTENSION);
			if (keyword_feature) {
				tpp_macro_expansion_init_chunk_inherited(_result, keyword_feature);
				return TPP_EOK;
			}
		}
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION */
		switch (tpp_keyword_getid(_kwd)) {
#define TPP_DEFS
#define TPP_PREDEFINED_FEATURE_HAS_EXTENSION(keyword_id, expansion_expr) \
		case keyword_id: {                                               \
			expansion_expr;                                              \
		}	break;
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
		default: break;
		}
		break;
#endif /* TPP_HAVE_CLANG_MACRO___has_extension */
#if TPP_HAVE_CLANG_MACRO___has_feature
	case TPP_KWD___has_feature:
#if TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE
		{
			TPP_REF tpp_string *keyword_feature;
			keyword_feature = tpp_keyword_getfeature(_kwd, TPP_KEYWORD_FEATURE_KIND_HAS_FEATURE);
			if (keyword_feature) {
				tpp_macro_expansion_init_chunk_inherited(_result, keyword_feature);
				return TPP_EOK;
			}
		}
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE */
		switch (tpp_keyword_getid(_kwd)) {
#define TPP_DEFS
#define TPP_PREDEFINED_FEATURE_HAS_FEATURE(keyword_id, expansion_expr) \
		case keyword_id: {                                             \
			expansion_expr;                                            \
		}	break;
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
		default: break;
		}
		break;
#endif /* TPP_HAVE_CLANG_MACRO___has_feature */
#if TPP_HAVE_CLANG_MACRO___has_c_attribute
	case TPP_KWD___has_c_attribute:
#if TPP_HAVE_KEYWORD_FEATURE_HAS_C_ATTRIBUTE
		{
			TPP_REF tpp_string *keyword_feature;
			keyword_feature = tpp_keyword_getfeature(_kwd, TPP_KEYWORD_FEATURE_KIND_HAS_C_ATTRIBUTE);
			if (keyword_feature) {
				tpp_macro_expansion_init_chunk_inherited(_result, keyword_feature);
				return TPP_EOK;
			}
		}
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_C_ATTRIBUTE */
		switch (tpp_keyword_getid(_kwd)) {
#define TPP_DEFS
#define TPP_PREDEFINED_FEATURE_HAS_C_ATTRIBUTE(keyword_id, expansion_expr) \
		case keyword_id: {                                                 \
			expansion_expr;                                                \
		}	break;
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
		default: break;
		}
		break;
#endif /* TPP_HAVE_CLANG_MACRO___has_c_attribute */
/*[[[end]]]*/
	default: tpp_unreachable();
	}
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_LEXER_GETKEYWORDFEATURE */


#if TPP_HAVE_CPP_PREDEFINED_MACROS
/* Return the effective expansion of a keyword-style macro.
 * @return: TPP_EOK:    Success: expansion was stored in `result`
 * @return: TPP_ENOENT: SOFT_ERROR: keyword has no expansion
 * @return: TPP_E*:     HARD_ERROR: error returned by a user-defined feature-test callback. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_getpredefinedmacro(tpp_lexer *tpp_restrict _self,
                             tpp_keyword const *tpp_restrict _kwd,
                             tpp_macro_expansion *tpp_restrict _result) {
	(void)_self;
	switch (tpp_keyword_getid(_kwd)) {
#define TPP_DEFS
#define TPP_PREDEFINED_MACRO(keyword_id, expansion_expr) \
	case keyword_id: {                                   \
		expansion_expr;                                  \
	}	break;
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	default: break;
	}
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_CPP_PREDEFINED_MACROS */

#undef tpp_return_conststr
#undef tpp_return_bool
#undef tpp_current_lexer
#undef tpp_current_keyword
#undef tpp_current_keyword_id
#undef tpp_current_expansion


#if TPP_HAVE_LEXER_GETKEYWORDDEFINED
#if TPP_HAVE_CPP_BUILTIN_MACROS
static TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_builtin_keyword_defined(tpp_lexer *tpp_restrict self, tpp_token_id id) {
	(void)self;
	switch (id) {

#if TPP_HAVE_MACRO__Pragma /* GCC defines _Pragma as a macro... So I'll just do that as well! */
	case TPP_KWD__Pragma:
		return tpp_lexer_has(self, MACRO__Pragma);
#endif /* TPP_HAVE_MACRO__Pragma */
#if TPP_HAVE_MACRO___pragma /* I promised this would be defined as a macro... */
	case TPP_KWD___pragma:
		return tpp_lexer_has(self, MACRO___pragma);
#endif /* TPP_HAVE_MACRO___pragma */

/* Declare magic, built-in macros */
#if TPP_HAVE_CLANG_MACRO___has_attribute
	case TPP_KWD___has_attribute:
		return tpp_lexer_has(self, CLANG_MACRO___has_attribute);
#endif /* TPP_HAVE_CLANG_MACRO___has_attribute */
#if TPP_HAVE_CLANG_MACRO___has_builtin
	case TPP_KWD___has_builtin:
		return tpp_lexer_has(self, CLANG_MACRO___has_builtin);
#endif /* TPP_HAVE_CLANG_MACRO___has_builtin */
#if TPP_HAVE_CLANG_MACRO___has_cpp_attribute
	case TPP_KWD___has_cpp_attribute:
		return tpp_lexer_has(self, CLANG_MACRO___has_cpp_attribute);
#endif /* TPP_HAVE_CLANG_MACRO___has_cpp_attribute */
#if TPP_HAVE_CLANG_MACRO___has_declspec_attribute
	case TPP_KWD___has_declspec_attribute:
		return tpp_lexer_has(self, CLANG_MACRO___has_declspec_attribute);
#endif /* TPP_HAVE_CLANG_MACRO___has_declspec_attribute */
#if TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension
	case TPP_KWD___has_extension:
		return tpp_lexer_has(self, CLANG_MACRO___has_extension) ||
		       tpp_lexer_has(self, MACRO___has_extension);
#endif /* TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension */
#if TPP_HAVE_CLANG_MACRO___has_feature
	case TPP_KWD___has_feature:
		return tpp_lexer_has(self, CLANG_MACRO___has_feature);
#endif /* TPP_HAVE_CLANG_MACRO___has_feature */
#if TPP_HAVE_CLANG_MACRO___has_c_attribute
	case TPP_KWD___has_c_attribute:
		return tpp_lexer_has(self, CLANG_MACRO___has_c_attribute);
#endif /* TPP_HAVE_CLANG_MACRO___has_c_attribute */
#if TPP_HAVE_MACRO___is_identifier
	case TPP_KWD___is_identifier:
		return tpp_lexer_has(self, MACRO___is_identifier);
#endif /* TPP_HAVE_MACRO___is_identifier */
#if TPP_HAVE_MACRO___is_deprecated
	case TPP_KWD___is_deprecated:
		return tpp_lexer_has(self, MACRO___is_deprecated);
#endif /* TPP_HAVE_MACRO___is_deprecated */
#if TPP_HAVE_MACRO___is_poisoned
	case TPP_KWD___is_poisoned:
		return tpp_lexer_has(self, MACRO___is_poisoned);
#endif /* TPP_HAVE_MACRO___is_poisoned */
#if TPP_HAVE_MACRO___has_known_extension
	case TPP_KWD___has_known_extension:
		return tpp_lexer_has(self, MACRO___has_known_extension);
#endif /* TPP_HAVE_MACRO___has_known_extension */
#if TPP_HAVE_MACRO___has_warning
	case TPP_KWD___has_warning:
		return tpp_lexer_has(self, MACRO___has_warning);
#endif /* TPP_HAVE_MACRO___has_warning */
#if TPP_HAVE_MACRO___has_known_warning
	case TPP_KWD___has_known_warning:
		return tpp_lexer_has(self, MACRO___has_known_warning);
#endif /* TPP_HAVE_MACRO___has_known_warning */
#if TPP_HAVE_MACRO___has_include
	case TPP_KWD___has_include:
		return tpp_lexer_has(self, MACRO___has_include);
#endif /* TPP_HAVE_MACRO___has_include */
#if TPP_HAVE_MACRO___has_include_next
	case TPP_KWD___has_include_next:
		return tpp_lexer_has(self, MACRO___has_include_next);
#endif /* TPP_HAVE_MACRO___has_include_next */
#if TPP_HAVE_MACRO___has_embed
	case TPP_KWD___has_embed:
		return tpp_lexer_has(self, MACRO___has_embed);
#endif /* TPP_HAVE_MACRO___has_embed */
#if TPP_HAVE_MACRO___FILE__
	case TPP_KWD___FILE__:
		return tpp_lexer_has(self, MACRO___FILE__);
#endif /* TPP_HAVE_MACRO___FILE__ */
#if TPP_HAVE_MACRO___LINE__
	case TPP_KWD___LINE__:
		return tpp_lexer_has(self, MACRO___LINE__);
#endif /* TPP_HAVE_MACRO___LINE__ */
#if TPP_HAVE_MACRO___TIME__
	case TPP_KWD___TIME__:
		return tpp_lexer_has(self, MACRO___TIME__);
#endif /* TPP_HAVE_MACRO___TIME__ */
#if TPP_HAVE_MACRO___DATE__
	case TPP_KWD___DATE__:
		return tpp_lexer_has(self, MACRO___DATE__);
#endif /* TPP_HAVE_MACRO___DATE__ */
#if TPP_HAVE_MACRO___COLUMN__
	case TPP_KWD___COLUMN__:
		return tpp_lexer_has(self, MACRO___COLUMN__);
#endif /* TPP_HAVE_MACRO___COLUMN__ */
#if TPP_HAVE_MACRO___BASE_FILE__
	case TPP_KWD___BASE_FILE__:
		return tpp_lexer_has(self, MACRO___BASE_FILE__);
#endif /* TPP_HAVE_MACRO___BASE_FILE__ */
#if TPP_HAVE_MACRO___FILE_NAME__
	case TPP_KWD___FILE_NAME__:
		return tpp_lexer_has(self, MACRO___FILE_NAME__);
#endif /* TPP_HAVE_MACRO___FILE_NAME__ */
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__
	case TPP_KWD___INCLUDE_LEVEL__:
		return tpp_lexer_has(self, MACRO___INCLUDE_LEVEL__);
#endif /* TPP_HAVE_MACRO___INCLUDE_LEVEL__ */
#if TPP_HAVE_MACRO___INCLUDE_DEPTH__
	case TPP_KWD___INCLUDE_DEPTH__:
		return tpp_lexer_has(self, MACRO___INCLUDE_DEPTH__);
#endif /* TPP_HAVE_MACRO___INCLUDE_DEPTH__ */
#if TPP_HAVE_MACRO___COUNTER__
	case TPP_KWD___COUNTER__:
		return tpp_lexer_has(self, MACRO___COUNTER__);
#endif /* TPP_HAVE_MACRO___COUNTER__ */
#if TPP_HAVE_MACRO___TIMESTAMP__
	case TPP_KWD___TIMESTAMP__:
		return tpp_lexer_has(self, MACRO___TIMESTAMP__);
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */
#if TPP_HAVE_NUMERIC_DATE_MACROS
	case TPP_KWD___DATE_DAY__:
	case TPP_KWD___DATE_WDAY__:
	case TPP_KWD___DATE_YDAY__:
	case TPP_KWD___DATE_MONTH__:
	case TPP_KWD___DATE_YEAR__:
		return tpp_lexer_has(self, NUMERIC_DATE_MACROS);
#endif /* TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_TIME_MACROS
	case TPP_KWD___TIME_SEC__:
	case TPP_KWD___TIME_MIN__:
	case TPP_KWD___TIME_HOUR__:
		return tpp_lexer_has(self, NUMERIC_TIME_MACROS);
#endif /* TPP_HAVE_NUMERIC_TIME_MACROS */
#if TPP_HAVE_MACRO___TPP_EVAL
	case TPP_KWD___TPP_EVAL:
		return tpp_lexer_has(self, MACRO___TPP_EVAL);
#endif /* TPP_HAVE_MACRO___TPP_EVAL */
#if TPP_HAVE_MACRO___TPP_EXEC
	case TPP_KWD___TPP_EXEC:
		return tpp_lexer_has(self, MACRO___TPP_EXEC);
#endif /* TPP_HAVE_MACRO___TPP_EXEC */
#if TPP_HAVE_MACRO___TPP_UNIQUE
	case TPP_KWD___TPP_UNIQUE:
		return tpp_lexer_has(self, MACRO___TPP_UNIQUE);
#endif /* TPP_HAVE_MACRO___TPP_UNIQUE */
#if TPP_HAVE_MACRO___TPP_LOAD_FILE
	case TPP_KWD___TPP_LOAD_FILE:
		return tpp_lexer_has(self, MACRO___TPP_LOAD_FILE);
#endif /* TPP_HAVE_MACRO___TPP_LOAD_FILE */
#if TPP_HAVE_MACRO___TPP_COUNTER
	case TPP_KWD___TPP_COUNTER:
		return tpp_lexer_has(self, MACRO___TPP_COUNTER);
#endif /* TPP_HAVE_MACRO___TPP_COUNTER */
#if TPP_HAVE_MACRO___TPP_RANDOM
	case TPP_KWD___TPP_RANDOM:
		return tpp_lexer_has(self, MACRO___TPP_RANDOM);
#endif /* TPP_HAVE_MACRO___TPP_RANDOM */
#if TPP_HAVE_MACRO___TPP_STR_DECOMPILE
	case TPP_KWD___TPP_STR_DECOMPILE:
		return tpp_lexer_has(self, MACRO___TPP_STR_DECOMPILE);
#endif /* TPP_HAVE_MACRO___TPP_STR_DECOMPILE */
#if TPP_HAVE_MACRO___TPP_STR_SUBSTR
	case TPP_KWD___TPP_STR_SUBSTR:
		return tpp_lexer_has(self, MACRO___TPP_STR_SUBSTR);
	case TPP_KWD___TPP_STR_AT:
		return tpp_lexer_has(self, MACRO___TPP_STR_SUBSTR);
#endif /* TPP_HAVE_MACRO___TPP_STR_SUBSTR */
#if TPP_HAVE_MACRO___TPP_STR_PACK
	case TPP_KWD___TPP_STR_PACK:
		return tpp_lexer_has(self, MACRO___TPP_STR_PACK);
#endif /* TPP_HAVE_MACRO___TPP_STR_PACK */
#if TPP_HAVE_MACRO___TPP_STR_SIZE
	case TPP_KWD___TPP_STR_SIZE:
		return tpp_lexer_has(self, MACRO___TPP_STR_SIZE);
#endif /* TPP_HAVE_MACRO___TPP_STR_SIZE */
#if TPP_HAVE_MACRO___TPP_COUNT_TOKENS
	case TPP_KWD___TPP_COUNT_TOKENS:
		return tpp_lexer_has(self, MACRO___TPP_COUNT_TOKENS);
#endif /* TPP_HAVE_MACRO___TPP_COUNT_TOKENS */
#if TPP_HAVE_MACRO___TPP_IDENTIFIER
	case TPP_KWD___TPP_IDENTIFIER:
		return tpp_lexer_has(self, MACRO___TPP_IDENTIFIER);
#endif /* TPP_HAVE_MACRO___TPP_IDENTIFIER */
	default: break;
	}
	return false;
}
#endif /* TPP_HAVE_CPP_BUILTIN_MACROS */

#if TPP_HAVE_CPP_PREDEFINED_MACROS
static TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_predefined_keyword_defined(tpp_lexer *tpp_restrict _self,
                                     tpp_keyword const *tpp_restrict _kwd) {
	(void)_self;
	switch (_kwd->tk_id) {
#define TPP_DEFS
#define TPP_MACRO(id, if_expr) \
	case id: return if_expr;
#define tpp_current_lexer()      _self
#define tpp_current_keyword()    _kwd
#define tpp_current_keyword_id() _kwd->tk_id
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef tpp_current_lexer
#undef tpp_current_keyword
#undef tpp_current_keyword_id
#undef TPP_DEFS
	default: break;
	}
	return false;
}
#endif /* TPP_HAVE_CPP_PREDEFINED_MACROS */

/* Returns true if `kwd` should be considered to be `#if defined()`
 * Does all the handling necessary to determine the correct state of
 * builtin/predefined macros. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_getkeyworddefined(tpp_lexer *tpp_restrict self,
                            tpp_keyword const *tpp_restrict kwd) {
	(void)self;

	/* Check for user-defined macro definitions */
#if TPP_HAVE_CPP_MACROS
	if (_TPP_KEYWORD_MACRO_ISDEFINED(kwd->tk_macro))
		return true; /* User-defined macro definition */
	if (tpp_keyword_isuser(kwd))
		return false; /* User-keyword never have builtin/predefined expansions */
#endif /* TPP_HAVE_CPP_MACROS */

	/* Check if the user has deleted this macro's definition */
#if TPP_HAVE_CPP_BUILTIN_MACROS || TPP_HAVE_CPP_PREDEFINED_MACROS
	if (kwd->tk_macro == _TPP_KEYWORD_MACRO_UNDEFINED)
		return false; /* Manually deleted */
#endif /* TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_PREDEFINED_MACROS */

	/* Check for predefined macro definitions */
#if TPP_HAVE_CPP_PREDEFINED_MACROS
	if (tpp_lexer_predefined_keyword_defined(self, kwd))
		return tpp_lexer_has(self, CPP_PREDEFINED_MACROS);
#endif /* TPP_HAVE_CPP_PREDEFINED_MACROS */

	/* Check for builtin macro definitions */
#if TPP_HAVE_CPP_BUILTIN_MACROS
	if (tpp_lexer_builtin_keyword_defined(self, tpp_keyword_getid(kwd)))
		return tpp_lexer_has(self, CPP_BUILTIN_MACROS);
#endif /* TPP_HAVE_CPP_BUILTIN_MACROS */

	/* Else: keyword isn't considered "defined" */
	return false;
}
#endif /* TPP_HAVE_LEXER_GETKEYWORDDEFINED */


#if TPP_HAVE_LEXER_ISIDENTIFIER
/* Returns true if `kwd` should is considered a `__is_identifier()`
 *
 * When that is the case, `__is_identifier()` expands to `1` (rather
 * than `0`) for that keyword, and the user attempting to define a
 * macro of the same name triggers a `-Wkeyword-macro` warning. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_isidentifier(tpp_lexer *tpp_restrict _self,
                       tpp_keyword const *tpp_restrict _kwd) {
	/* User-defined keywords are never identifiers */
	if (tpp_keyword_isuser(_kwd))
		return false;

	/* Evaluate on a per-keyword basis. */
	(void)_self;
	switch (_kwd->tk_id) {
#define TPP_DEFS
#if TPP_HAVE_CPP_MACROS
#define TPP_PREDEFINED_MACRO(keyword_id, value) \
	case keyword_id: return false; /* Builtin macros with custom expansions are never identifiers */
#endif /* TPP_HAVE_CPP_MACROS */
#define TPP_KWD_IS_IDENTIFIER(id, is_identifier_expr) \
	case id: return is_identifier_expr;
#define tpp_current_lexer()      _self
#define tpp_current_keyword()    _kwd
#define tpp_current_keyword_id() _kwd->tk_id
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef tpp_current_lexer
#undef tpp_current_keyword
#undef tpp_current_keyword_id
#undef TPP_DEFS
	default: break;
	}
	return TPP_HAVE_LEXER_ISIDENTIFIER_DEFAULT;
}
#endif /* TPP_HAVE_LEXER_ISIDENTIFIER */


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
/* Returns the ID of the extension with the name that is closest to `name`
 * When no extensions are defined (at all), this will return `TPP_EXT_COUNT` */
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
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	char twf_##warning_id[sizeof(format) / sizeof(char)];
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr)
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
} const tpp_warning_format_strings = {
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	/* [twf_##warning_id] = */ format,
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr)
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
};

static tpp_size const tpp_warning_format_string_offsets[TPP_W_COUNT] = {
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	/* [warning_id] = */ tpp_offsetof(struct tpp_warning_format_strings_struct, twf_##warning_id),
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr) \
	/* [warning_id] = */ sizeof(struct tpp_warning_format_strings_struct) - 1,
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
};

/* Returns the `tpp_lexer_printf_warning`-style format string assigned with `id`.
 * When `id` is `TPP_W_COUNT`, invalid, or declared as `TPP_WARNING_EX`, return `NULL` instead. */
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
/* Returns the warning ID linked to a given `number`,
 * or `TPP_W_COUNT` if `number` is unknown */
TPP_IMPL TPP_WUNUSED tpp_warning_id TPPCALL
tpp_warning_ofnumber(unsigned int number) {
	switch (number) {
#define TPP_DEFS
#define TPP_WARNING_NUMBER_CASE(_, index, value) \
	case value: return _;
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_FOREACH(numbers, TPP_TUPLE_FOREACH_DUMMY_SEP, TPP_WARNING_NUMBER_CASE, warning_id)
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_WARNING_NUMBER_CASE
#undef TPP_DEFS
	default: break;
	}
	return TPP_W_COUNT;
}

/* Returns the `TPP_WARNING_NUMBER_INVALID`-terminated list of warning numbers
 * associated with `warning_id`. If `warning_id` doesn't have any warning
 * numbers, return a pointer to `TPP_WARNING_NUMBER_INVALID`. */
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED unsigned int const *TPPCALL
tpp_warning_getnumbers(tpp_warning_id warning_id) {
	static unsigned int const dummy_warning_numbers[1] = { TPP_WARNING_NUMBER_INVALID };
	switch (warning_id) {
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
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_WARNING_NUMBERS_CASE
#undef TPP_DEFS
	default: break;
	}
	return dummy_warning_numbers;
}


/* Returns the context-id of a given (should-be) *numbered* warning.
 * When `warning_id` is invalid, `TPP_W_COUNT`, or not numbered, this
 * function will return `TPP_WC_COUNT` instead. */
TPP_IMPL TPP_WUNUSED tpp_warning_context_id TPPCALL
tpp_warning_context_id_ofwarning(tpp_warning_id warning_id) {
	switch (warning_id) {
#define TPP_DEFS
#define _TPP_CASE_NUMBERED_WARNING(warning_id) \
	case warning_id: return TPP_WC_##warning_id;
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_IF_NONEMPTY(numbers, _TPP_CASE_NUMBERED_WARNING, warning_id)
#undef GUARD_TPP_AMALGAMATION_H
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_CASE_NUMBERED_WARNING
#undef TPP_DEFS
	default: break;
	}
	return TPP_WC_COUNT;
}

/* Returns the warning-id linked to `ctx_id`, when `tpp_warning_context_id_isnumber(ctx_id)`.
 * When no warning is linked to `ctx_id` (`ctx_id` is either linked to a warning group, or
 * is `TPP_WC_COUNT` or some other invalid ID), return `TPP_W_COUNT` instead. */
TPP_IMPL TPP_WUNUSED tpp_warning_id TPPCALL
tpp_warning_context_id_aswarning(tpp_warning_context_id ctx_id) {
	switch (ctx_id) {
#define TPP_DEFS
#define _TPP_CASE_NUMBERED_WARNING(warning_id) \
	case TPP_WC_##warning_id: return warning_id;
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_IF_NONEMPTY(numbers, _TPP_CASE_NUMBERED_WARNING, warning_id)
#undef GUARD_TPP_AMALGAMATION_H
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
	hi = tpp_lengthof(tpp_warning_group_name_offsets_bynameid);
	for (;;) {
		unsigned int mid = (lo + hi) / 2;
		tpp_size mid_offset = tpp_warning_group_name_offsets_bynameid[mid];
		tpp_assert(lo < hi);
		if (name_offset < mid_offset) {
			hi = mid;
		} else if (name_offset > mid_offset) {
			lo = mid + 1;
		} else {
			/* At this point, "mid" is the *name-id* of the warning group.
			 * That ID must now finally be converted */
			return tpp_warning_group_bynameid[mid];
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
/* Returns the ID of the warning group with the name that is closest to `name`
 * When no warning groups are defined (at all), this will return `TPP_WG_COUNT` */
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


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_BUILTINS_C */
