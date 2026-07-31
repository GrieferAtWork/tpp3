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
#ifndef GUARD_TPP_SYSINCLUDE_H
#define GUARD_TPP_SYSINCLUDE_H 1

#include "api.h"
#include "config.h"
#include "error.h"
#include "string.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_INCLUDE_PATH
typedef struct tpp_include_path_entry {
#if TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING
	TPP_REF tpp_string *TPP_INTERNAL(tipe_pathstr); /* [1..1] The path described by this entry (with a trailing TPP_FS_SEP_S). */
#else /* TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */
	char               *TPP_INTERNAL(tipe_path);    /* [1..1][owned] The path described by this entry (with a trailing TPP_FS_SEP_S). */
#endif /* !TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */
} tpp_include_path_entry;

#if TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING
#define _tpp_include_path_entry_fini(self)                  \
	(tpp_string_decref((self)->TPP_INTERNAL(tipe_pathstr)), \
	 tpp_dbg_memset(self, sizeof(tpp_include_path_entry)))
#define _tpp_include_path_entry_getpath(self) \
	tpp_string_cstr((self)->TPP_INTERNAL(tipe_pathstr))
#else /* TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */
#define _tpp_include_path_entry_fini(self)      \
	(tpp_free((self)->TPP_INTERNAL(tipe_path)), \
	 tpp_dbg_memset(self, sizeof(tpp_include_path_entry)))
#define _tpp_include_path_entry_getpath(self) \
	((char const *)(self)->TPP_INTERNAL(tipe_path))
#endif /* !TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */

typedef struct tpp_include_path_list {
	tpp_include_path_entry *TPP_INTERNAL(tipl_list); /* [0..tipl_size][owned] List of include paths. */
	tpp_size                TPP_INTERNAL(tipl_size); /* # of entries in `tipl_list' */
} tpp_include_path_list;

/* Initialize/finalize a given "tpp_include_path_list" */
#define tpp_include_path_list_init(self)           \
	(void)((self)->TPP_INTERNAL(tipl_list) = NULL, \
	       (self)->TPP_INTERNAL(tipl_size) = 0)
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_include_path_list_fini(tpp_include_path_list *tpp_restrict self);

/* Return the # of paths described by "self" */
#define tpp_include_path_list_getcount(self) \
	(self)->TPP_INTERNAL(tipl_size)

/* Return the i'th path as a NUL-terminated string. */
#define tpp_include_path_list_getentry(self, i) \
	_tpp_include_path_entry_getpath(&(self)->TPP_INTERNAL(tipl_list)[i])

/* Clear this include-path list. */
#define tpp_include_path_list_clear(self) \
	(tpp_include_path_list_fini(self),    \
	 tpp_include_path_list_init(self))

/* Append the given "path" to "self".
 * @param: path:        The path to append.
 * @param: path_maxlen: The max length of "path". The actual length of the path that will
 *                      be appended to the path list is "tpp_strnlen(path, path_maxlen)".
 *
 * @return: TPP_EOK:    Success, or path was already present and was moved to the end.
 * @return: TPP_ENOMEM: Out of memory. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_include_path_list_pushtail(tpp_include_path_list *tpp_restrict self,
                               char const *path, tpp_size path_maxlen);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_include_path_list_pushhead(tpp_include_path_list *tpp_restrict self,
                               char const *path, tpp_size path_maxlen);

/* Remove the given "path" from "self".
 * @param: path:        The path to remove.
 * @param: path_maxlen: The max length of "path". The actual length of the path that will
 *                      be removed from the path list is "tpp_strnlen(path, path_maxlen)".
 *
 * @return: TPP_EOK:    Path was located and removed
 * @return: TPP_ENOENT: Path could not be found */
TPP_DECL TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_include_path_list_remove(tpp_include_path_list *tpp_restrict self,
                             char const *path, tpp_size path_maxlen);



/* Collective descriptor for defined include paths.
 * The actual order of paths in TPP3 is as follows (this matches GCC):
 *
 * 1.  (#include "foo.h" only): Relative to the current I/O-file, or (when
 *                              "TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE" is
 *                              enabled): every I/O-file on the #include-stack
 * 2.  (#include "foo.h" only): Paths specified in "tip_quote_list" (if available)
 * 3.  Paths specified in "tip_system_list"
 * 4.  Paths specified in "tip_syshdr_list"
 * 5.  Paths hard-coded using "TPP_CONFIG_SYSTEM_INCLUDE_PATH"
 * 6.  Paths specified in "tip_after_list" (if available)
 */
typedef struct tpp_include_paths {
	tpp_include_path_list TPP_INTERNAL(tip_system_list); /* System #include-path list: #pragma TPP include_path("/usr/include") */
#if TPP_HAVE_INCLUDE_PATH_QUOTE
	tpp_include_path_list TPP_INTERNAL(tip_quote_list);  /* "-quote #include-path list: #pragma TPP include_path(quote: "/usr/include") */
#define _tpp_include_paths_init_quote(self) , tpp_include_path_list_init(&(self)->TPP_INTERNAL(tip_quote_list))
#define _tpp_include_paths_fini_quote(self) , tpp_include_path_list_fini(&(self)->TPP_INTERNAL(tip_quote_list))
#else /* TPP_HAVE_INCLUDE_PATH_QUOTE */
#define _tpp_include_paths_init_quote(self) /* nothing */
#define _tpp_include_paths_fini_quote(self) /* nothing */
#endif /* !TPP_HAVE_INCLUDE_PATH_QUOTE */
#if TPP_HAVE_INCLUDE_PATH_SYSHDR
	tpp_include_path_list TPP_INTERNAL(tip_syshdr_list);  /* #include-paths treated as TPP_FILE_FLAGS_SYSHDR: #pragma TPP include_path(system: "/usr/include") */
#define _tpp_include_paths_init_syshdr(self) , tpp_include_path_list_init(&(self)->TPP_INTERNAL(tip_syshdr_list))
#define _tpp_include_paths_fini_syshdr(self) , tpp_include_path_list_fini(&(self)->TPP_INTERNAL(tip_syshdr_list))
#else /* TPP_HAVE_INCLUDE_PATH_SYSHDR */
#define _tpp_include_paths_init_syshdr(self) /* nothing */
#define _tpp_include_paths_fini_syshdr(self) /* nothing */
#endif /* !TPP_HAVE_INCLUDE_PATH_SYSHDR */
#if TPP_HAVE_INCLUDE_PATH_AFTER
	tpp_include_path_list TPP_INTERNAL(tip_after_list);  /* #include-path list searched after all others: #pragma TPP include_path(dirafter: "/usr/include") */
#define _tpp_include_paths_init_after(self) , tpp_include_path_list_init(&(self)->TPP_INTERNAL(tip_after_list))
#define _tpp_include_paths_fini_after(self) , tpp_include_path_list_fini(&(self)->TPP_INTERNAL(tip_after_list))
#else /* TPP_HAVE_INCLUDE_PATH_AFTER */
#define _tpp_include_paths_init_after(self) /* nothing */
#define _tpp_include_paths_fini_after(self) /* nothing */
#endif /* !TPP_HAVE_INCLUDE_PATH_AFTER */

#if TPP_HAVE_INCLUDE_PATH_EMBED
	tpp_include_path_list TPP_INTERNAL(tip_embed_list);  /* #embed-path list searched for `#embed <file>`-like filenames */
#define _tpp_include_paths_init_embed(self) , tpp_include_path_list_init(&(self)->TPP_INTERNAL(tip_embed_list))
#define _tpp_include_paths_fini_embed(self) , tpp_include_path_list_fini(&(self)->TPP_INTERNAL(tip_embed_list))
#else /* TPP_HAVE_INCLUDE_PATH_EMBED */
#define _tpp_include_paths_init_embed(self) /* nothing */
#define _tpp_include_paths_fini_embed(self) /* nothing */
#endif /* !TPP_HAVE_INCLUDE_PATH_EMBED */

#if TPP_HAVE_INCLUDE_PATH_PUSH_POP
	tpp_size                  TPP_INTERNAL(tip_pushcnt); /* # of times paths pushed were since last modified */
	struct tpp_include_paths *TPP_INTERNAL(tip_prev);    /* [0..1][owned] Old warning state. */
#define _tpp_include_paths_init_push(self) , (self)->TPP_INTERNAL(tip_pushcnt) = 0, (self)->TPP_INTERNAL(tip_prev) = NULL
#else /* TPP_HAVE_INCLUDE_PATH_PUSH_POP */
#define _tpp_include_paths_init_push(self) /* nothing */
#endif /* !TPP_HAVE_INCLUDE_PATH_PUSH_POP */
} tpp_include_paths;

/* Initialize/finalize include paths. */
#define tpp_include_paths_init(self)                                    \
	(tpp_include_path_list_init(&(self)->TPP_INTERNAL(tip_system_list)) \
	 _tpp_include_paths_init_quote(self)                                \
	 _tpp_include_paths_init_syshdr(self)                               \
	 _tpp_include_paths_init_after(self)                                \
	 _tpp_include_paths_init_embed(self)                                \
	 _tpp_include_paths_init_push(self))
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_include_paths_fini(tpp_include_paths *tpp_restrict self);

/* Reset (re-initialize) "self" */
#define tpp_include_paths_reset(self) \
	(tpp_include_paths_fini(self), tpp_include_paths_init(self))


#if TPP_HAVE_LEXER_COPY
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_include_paths_copy(tpp_include_paths *tpp_restrict self,
                       tpp_include_paths const *tpp_restrict from);
#endif /* TPP_HAVE_LEXER_COPY */

#undef TPP_HAVE_INCLUDE_PATH_MULTIPLE
typedef enum tpp_include_path_kind {
	TPP_INCLUDE_PATH_KIND_SYSTEM = tpp_offsetof(tpp_include_paths, TPP_INTERNAL(tip_system_list)),
#if TPP_HAVE_INCLUDE_PATH_QUOTE
	TPP_INCLUDE_PATH_KIND_QUOTE = tpp_offsetof(tpp_include_paths, TPP_INTERNAL(tip_quote_list)),
#define TPP_HAVE_INCLUDE_PATH_MULTIPLE 1
#endif /* TPP_HAVE_INCLUDE_PATH_QUOTE */
#if TPP_HAVE_INCLUDE_PATH_SYSHDR
	TPP_INCLUDE_PATH_KIND_SYSHDR = tpp_offsetof(tpp_include_paths, TPP_INTERNAL(tip_syshdr_list)),
#define TPP_HAVE_INCLUDE_PATH_MULTIPLE 1
#endif /* TPP_HAVE_INCLUDE_PATH_SYSHDR */
#if TPP_HAVE_INCLUDE_PATH_AFTER
	TPP_INCLUDE_PATH_KIND_AFTER = tpp_offsetof(tpp_include_paths, TPP_INTERNAL(tip_after_list)),
#define TPP_HAVE_INCLUDE_PATH_MULTIPLE 1
#endif /* TPP_HAVE_INCLUDE_PATH_AFTER */
#if TPP_HAVE_INCLUDE_PATH_EMBED
	TPP_INCLUDE_PATH_KIND_EMBED = tpp_offsetof(tpp_include_paths, TPP_INTERNAL(tip_embed_list)),
#define TPP_HAVE_INCLUDE_PATH_MULTIPLE 1
#endif /* TPP_HAVE_INCLUDE_PATH_EMBED */
} tpp_include_path_kind;
#ifndef TPP_HAVE_INCLUDE_PATH_MULTIPLE
#define TPP_HAVE_INCLUDE_PATH_MULTIPLE 0
#endif /* !TPP_HAVE_INCLUDE_PATH_MULTIPLE */

#if TPP_HAVE_INCLUDE_PATH_MULTIPLE
#define _tpp_include_paths_bykind(self, kind) \
	((tpp_include_path_list *)((char *)(self) + (tpp_size)(unsigned int)(kind)))
#else /* TPP_HAVE_INCLUDE_PATH_MULTIPLE */
#define _tpp_include_paths_bykind(self, kind) (&(self)->TPP_INTERNAL(tip_system_list))
#endif /* !TPP_HAVE_INCLUDE_PATH_MULTIPLE */


/* Access include paths */
#define tpp_include_paths_numsystem(self)    tpp_include_path_list_getcount(&(self)->TPP_INTERNAL(tip_system_list))
#define tpp_include_paths_getsystem(self, i) tpp_include_path_list_getentry(&(self)->TPP_INTERNAL(tip_system_list), i)
#if TPP_HAVE_INCLUDE_PATH_QUOTE
#define tpp_include_paths_numquote(self)     tpp_include_path_list_getcount(&(self)->TPP_INTERNAL(tip_quote_list))
#define tpp_include_paths_getquote(self, i)  tpp_include_path_list_getentry(&(self)->TPP_INTERNAL(tip_quote_list), i)
#endif /* TPP_HAVE_INCLUDE_PATH_QUOTE */
#if TPP_HAVE_INCLUDE_PATH_SYSHDR
#define tpp_include_paths_numsyshdr(self)    tpp_include_path_list_getcount(&(self)->TPP_INTERNAL(tip_syshdr_list))
#define tpp_include_paths_getsyshdr(self, i) tpp_include_path_list_getentry(&(self)->TPP_INTERNAL(tip_syshdr_list), i)
#endif /* TPP_HAVE_INCLUDE_PATH_SYSHDR */
#if TPP_HAVE_INCLUDE_PATH_AFTER
#define tpp_include_paths_numafter(self)     tpp_include_path_list_getcount(&(self)->TPP_INTERNAL(tip_after_list))
#define tpp_include_paths_getafter(self, i)  tpp_include_path_list_getentry(&(self)->TPP_INTERNAL(tip_after_list), i)
#endif /* TPP_HAVE_INCLUDE_PATH_AFTER */
#if TPP_HAVE_INCLUDE_PATH_EMBED
#define tpp_include_paths_numembed(self)     tpp_include_path_list_getcount(&(self)->TPP_INTERNAL(tip_embed_list))
#define tpp_include_paths_getembed(self, i)  tpp_include_path_list_getentry(&(self)->TPP_INTERNAL(tip_embed_list), i)
#endif /* TPP_HAVE_INCLUDE_PATH_EMBED */
#define tpp_include_paths_numbykind(self, kind)    tpp_include_path_list_getcount(_tpp_include_paths_bykind(self, kind))
#define tpp_include_paths_getbykind(self, kind, i) tpp_include_path_list_getentry(_tpp_include_paths_bykind(self, kind), i)

/* Helper methods to add/remove paths to different include path lists */
#if TPP_HAVE_INCLUDE_PATH_PUSH_POP
#define _tpp_include_paths_alloc() ((tpp_include_paths *)tpp_malloc(sizeof(tpp_include_paths)))
#define _tpp_include_paths_free(p) tpp_free(p)

#if TPP_HAVE_INCLUDE_PATH_MULTIPLE
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_include_paths_addbykind(tpp_include_paths *tpp_restrict self,
                            tpp_include_path_kind kind,
                            char const *path, tpp_size path_maxlen);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_include_paths_addbykind_head(tpp_include_paths *tpp_restrict self,
                                 tpp_include_path_kind kind,
                                 char const *path, tpp_size path_maxlen);

/* @return: TPP_EOK:    Path was located and removed
 * @return: TPP_ENOENT: Path could not be found
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_include_paths_delbykind(tpp_include_paths *tpp_restrict self,
                            tpp_include_path_kind kind,
                            char const *path, tpp_size path_maxlen);

/* @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_include_paths_clearbykind(tpp_include_paths *tpp_restrict self,
                              tpp_include_path_kind kind);
#else /* TPP_HAVE_INCLUDE_PATH_MULTIPLE */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_include_paths_addbykind(tpp_include_paths *tpp_restrict self,
                             char const *path, tpp_size path_maxlen);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_include_paths_addbykind_head(tpp_include_paths *tpp_restrict self,
                                  char const *path, tpp_size path_maxlen);

/* @return: TPP_EOK:    Path was located and removed
 * @return: TPP_ENOENT: Path could not be found
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_include_paths_delbykind(tpp_include_paths *tpp_restrict self,
                             char const *path, tpp_size path_maxlen);

/* @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_include_paths_clearbykind(tpp_include_paths *tpp_restrict self);
#define tpp_include_paths_addbykind(self, kind, path, path_maxlen) \
	_tpp_include_paths_addbykind(self, path, path_maxlen)
#define tpp_include_paths_addbykind_head(self, kind, path, path_maxlen) \
	_tpp_include_paths_addbykind_head(self, path, path_maxlen)
#define tpp_include_paths_delbykind(self, kind, path, path_maxlen) \
	_tpp_include_paths_delbykind(self, path, path_maxlen)
#define tpp_include_paths_clearbykind(self, kind) \
	_tpp_include_paths_clearbykind(self)
#endif /* !TPP_HAVE_INCLUDE_PATH_MULTIPLE */


#define tpp_include_paths_addsystem(self, path, path_maxlen)      tpp_include_paths_addbykind(self, TPP_INCLUDE_PATH_KIND_SYSTEM, path, path_maxlen)
#define tpp_include_paths_addsystem_head(self, path, path_maxlen) tpp_include_paths_addbykind_head(self, TPP_INCLUDE_PATH_KIND_SYSTEM, path, path_maxlen)
#define tpp_include_paths_delsystem(self, path, path_maxlen)      tpp_include_paths_delbykind(self, TPP_INCLUDE_PATH_KIND_SYSTEM, path, path_maxlen)
#define tpp_include_paths_clearsystem(self)                       tpp_include_paths_clearbykind(self, TPP_INCLUDE_PATH_KIND_SYSTEM)
#if TPP_HAVE_INCLUDE_PATH_QUOTE
#define tpp_include_paths_addquote(self, path, path_maxlen)      tpp_include_paths_addbykind(self, TPP_INCLUDE_PATH_KIND_QUOTE, path, path_maxlen)
#define tpp_include_paths_addquote_head(self, path, path_maxlen) tpp_include_paths_addbykind_head(self, TPP_INCLUDE_PATH_KIND_QUOTE, path, path_maxlen)
#define tpp_include_paths_delquote(self, path, path_maxlen)      tpp_include_paths_delbykind(self, TPP_INCLUDE_PATH_KIND_QUOTE, path, path_maxlen)
#define tpp_include_paths_clearquote(self)                       tpp_include_paths_clearbykind(self, TPP_INCLUDE_PATH_KIND_QUOTE)
#endif /* TPP_HAVE_INCLUDE_PATH_QUOTE */
#if TPP_HAVE_INCLUDE_PATH_SYSHDR
#define tpp_include_paths_addsyshdr(self, path, path_maxlen)      tpp_include_paths_addbykind(self, TPP_INCLUDE_PATH_KIND_SYSHDR, path, path_maxlen)
#define tpp_include_paths_addsyshdr_head(self, path, path_maxlen) tpp_include_paths_addbykind_head(self, TPP_INCLUDE_PATH_KIND_SYSHDR, path, path_maxlen)
#define tpp_include_paths_delsyshdr(self, path, path_maxlen)      tpp_include_paths_delbykind(self, TPP_INCLUDE_PATH_KIND_SYSHDR, path, path_maxlen)
#define tpp_include_paths_clearsyshdr(self)                       tpp_include_paths_clearbykind(self, TPP_INCLUDE_PATH_KIND_SYSHDR)
#endif /* TPP_HAVE_INCLUDE_PATH_SYSHDR */
#if TPP_HAVE_INCLUDE_PATH_AFTER
#define tpp_include_paths_addafter(self, path, path_maxlen)      tpp_include_paths_addbykind(self, TPP_INCLUDE_PATH_KIND_AFTER, path, path_maxlen)
#define tpp_include_paths_addafter_head(self, path, path_maxlen) tpp_include_paths_addbykind_head(self, TPP_INCLUDE_PATH_KIND_AFTER, path, path_maxlen)
#define tpp_include_paths_delafter(self, path, path_maxlen)      tpp_include_paths_delbykind(self, TPP_INCLUDE_PATH_KIND_AFTER, path, path_maxlen)
#define tpp_include_paths_clearafter(self)                       tpp_include_paths_clearbykind(self, TPP_INCLUDE_PATH_KIND_AFTER)
#endif /* TPP_HAVE_INCLUDE_PATH_AFTER */
#if TPP_HAVE_INCLUDE_PATH_EMBED
#define tpp_include_paths_addembed(self, path, path_maxlen)      tpp_include_paths_addbykind(self, TPP_INCLUDE_PATH_KIND_EMBED, path, path_maxlen)
#define tpp_include_paths_addembed_head(self, path, path_maxlen) tpp_include_paths_addbykind_head(self, TPP_INCLUDE_PATH_KIND_EMBED, path, path_maxlen)
#define tpp_include_paths_delembed(self, path, path_maxlen)      tpp_include_paths_delbykind(self, TPP_INCLUDE_PATH_KIND_EMBED, path, path_maxlen)
#define tpp_include_paths_clearembed(self)                       tpp_include_paths_clearbykind(self, TPP_INCLUDE_PATH_KIND_EMBED)
#endif /* TPP_HAVE_INCLUDE_PATH_EMBED */

/* Push the current include paths state */
#define tpp_include_paths_push(self) (void)(++(self)->TPP_INTERNAL(tip_pushcnt))

/* Pop the current include paths state (may only be called when `tpp_include_paths_canpop(self)') */
TPP_DECL TPP_NONNULL((1)) void TPPCALL tpp_include_paths_pop(tpp_include_paths *tpp_restrict self);
#define tpp_include_paths_canpop(self)         \
	((self)->TPP_INTERNAL(tip_pushcnt) != 0 || \
	 (self)->TPP_INTERNAL(tip_prev) != NULL)

/* When true, `tpp_include_paths_setctx()' must first copy the extension
 * state (which requires heap memory, and may thus fail) */
#define tpp_include_paths_mustcopy(self) ((self)->TPP_INTERNAL(tip_pushcnt) != 0)

#else /* TPP_HAVE_INCLUDE_PATH_PUSH_POP */
#define tpp_include_paths_addsystem(self, path, path_maxlen)      tpp_include_path_list_pushtail(&(self)->TPP_INTERNAL(tip_system_list), path, path_maxlen)
#define tpp_include_paths_addsystem_head(self, path, path_maxlen) tpp_include_path_list_pushhead(&(self)->TPP_INTERNAL(tip_system_list), path, path_maxlen)
#define tpp_include_paths_delsystem(self, path, path_maxlen)      tpp_include_path_list_remove(&(self)->TPP_INTERNAL(tip_system_list), path, path_maxlen)
#define tpp_include_paths_clearsystem(self)                       tpp_include_path_list_clear(&(self)->TPP_INTERNAL(tip_system_list))
#if TPP_HAVE_INCLUDE_PATH_QUOTE
#define tpp_include_paths_addquote(self, path, path_maxlen)      tpp_include_path_list_pushtail(&(self)->TPP_INTERNAL(tip_quote_list), path, path_maxlen)
#define tpp_include_paths_addquote_head(self, path, path_maxlen) tpp_include_path_list_pushhead(&(self)->TPP_INTERNAL(tip_quote_list), path, path_maxlen)
#define tpp_include_paths_delquote(self, path, path_maxlen)      tpp_include_path_list_remove(&(self)->TPP_INTERNAL(tip_quote_list), path, path_maxlen)
#define tpp_include_paths_clearquote(self)                       tpp_include_path_list_clear(&(self)->TPP_INTERNAL(tip_quote_list))
#endif /* TPP_HAVE_INCLUDE_PATH_QUOTE */
#if TPP_HAVE_INCLUDE_PATH_SYSHDR
#define tpp_include_paths_addsyshdr(self, path, path_maxlen)      tpp_include_path_list_pushtail(&(self)->TPP_INTERNAL(tip_syshdr_list), path, path_maxlen)
#define tpp_include_paths_addsyshdr_head(self, path, path_maxlen) tpp_include_path_list_pushhead(&(self)->TPP_INTERNAL(tip_syshdr_list), path, path_maxlen)
#define tpp_include_paths_delsyshdr(self, path, path_maxlen)      tpp_include_path_list_remove(&(self)->TPP_INTERNAL(tip_syshdr_list), path, path_maxlen)
#define tpp_include_paths_clearsyshdr(self)                       tpp_include_path_list_clear(&(self)->TPP_INTERNAL(tip_syshdr_list))
#endif /* TPP_HAVE_INCLUDE_PATH_SYSHDR */
#if TPP_HAVE_INCLUDE_PATH_AFTER
#define tpp_include_paths_addafter(self, path, path_maxlen)      tpp_include_path_list_pushtail(&(self)->TPP_INTERNAL(tip_after_list), path, path_maxlen)
#define tpp_include_paths_addafter_head(self, path, path_maxlen) tpp_include_path_list_pushhead(&(self)->TPP_INTERNAL(tip_after_list), path, path_maxlen)
#define tpp_include_paths_delafter(self, path, path_maxlen)      tpp_include_path_list_remove(&(self)->TPP_INTERNAL(tip_after_list), path, path_maxlen)
#define tpp_include_paths_clearafter(self)                       tpp_include_path_list_clear(&(self)->TPP_INTERNAL(tip_after_list))
#endif /* TPP_HAVE_INCLUDE_PATH_AFTER */
#if TPP_HAVE_INCLUDE_PATH_EMBED
#define tpp_include_paths_addembed(self, path, path_maxlen)      tpp_include_path_list_pushtail(&(self)->TPP_INTERNAL(tip_embed_list), path, path_maxlen)
#define tpp_include_paths_addembed_head(self, path, path_maxlen) tpp_include_path_list_pushhead(&(self)->TPP_INTERNAL(tip_embed_list), path, path_maxlen)
#define tpp_include_paths_delembed(self, path, path_maxlen)      tpp_include_path_list_remove(&(self)->TPP_INTERNAL(tip_embed_list), path, path_maxlen)
#define tpp_include_paths_clearembed(self)                       tpp_include_path_list_clear(&(self)->TPP_INTERNAL(tip_embed_list))
#endif /* TPP_HAVE_INCLUDE_PATH_EMBED */
#endif /* !TPP_HAVE_INCLUDE_PATH_PUSH_POP */
#endif /* TPP_HAVE_INCLUDE_PATH */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_SYSINCLUDE_H */
