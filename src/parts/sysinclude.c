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
#ifndef GUARD_TPP_SYSINCLUDE_C
#define GUARD_TPP_SYSINCLUDE_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "file-io.h"
#include "sysinclude.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_INCLUDE_PATH

/* Initialize/finalize a given "tpp_include_path_list" */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_include_path_list_fini(tpp_include_path_list *tpp_restrict self) {
	tpp_size i;
	for (i = 0; i < self->tipl_size; ++i) {
		tpp_include_path_entry *entry = &self->tipl_list[i];
		_tpp_include_path_entry_fini(entry);
	}
	tpp_free(self->tipl_list);
	tpp_dbg_memset(self, sizeof(*self));
}

static TPP_WUNUSED tpp_size TPPCALL
tpp_include_path_strlen(char const *path, tpp_size path_maxlen) {
	tpp_size result = tpp_strnlen(path, path_maxlen);
	while (result && TPP_FS_ISSEP(path[result - 1]))
		--result;
	return result;
}

#define tpp_include_path_entry_equals(entry, path, pathlen)                    \
	tpp_include_path_entry_equals_impl(_tpp_include_path_entry_getpath(entry), \
	                                   path, pathlen)
static TPP_WUNUSED TPP_NONNULL((1, 2)) bool TPPCALL
tpp_include_path_entry_equals_impl(char const *entry_path,
                                   char const *path,
                                   tpp_size pathlen) {
	if (tpp_memcmp(entry_path, path, pathlen * sizeof(char)) != 0)
		return false;
	if (entry_path[pathlen] != TPP_FS_SEP)
		return false;
	return entry_path[pathlen + 1] == '\0';
}

#if TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING
#define tpp_include_path_string         TPP_REF tpp_string
#define tpp_include_path_string_free(x) tpp_string_decref(x)
#define tpp_include_path_entry_init_inherited(self, s) \
	(void)((self)->tipe_pathstr = (s))
#else /* TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */
#define tpp_include_path_string         char
#define tpp_include_path_string_free(x) tpp_free(x)
#define tpp_include_path_entry_init_inherited(self, s) \
	(void)((self)->tipe_path = (s))
#endif /* !TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */

static tpp_include_path_string *TPPCALL
tpp_include_path_string_new(char const *path, tpp_size pathlen) {
	tpp_include_path_string *result;
#if TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING
	result = tpp_string_malloc(pathlen + 1);
	if tpp_unlikely(!result)
		return NULL;
	tpp_memcpy(result->ts_str, path, pathlen * sizeof(char));
	result->ts_str[pathlen] = TPP_FS_SEP;
#else /* TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */
	result = (char *)tpp_malloc((pathlen + 2) * sizeof(char));
	if tpp_unlikely(!result)
		return NULL;
	tpp_memcpy(result, path, pathlen * sizeof(char));
	result[pathlen + 0] = TPP_FS_SEP;
	result[pathlen + 1] = '\0';
#endif /* !TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */
	return result;
}

/* Append the given "path" to `self`.
 * @param: path:        The path to append.
 * @param: path_maxlen: The max length of "path". The actual length of the path that will
 *                      be appended to the path list is "tpp_strnlen(path, path_maxlen)".
 *
 * @return: TPP_EOK:    Success, or path was already present and was moved to the end.
 * @return: TPP_ENOMEM: Out of memory. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_include_path_list_pushtail(tpp_include_path_list *tpp_restrict self,
                               char const *path, tpp_size path_maxlen) {
	tpp_size i;
	tpp_include_path_string *newpath;
	tpp_size pathlen = tpp_include_path_strlen(path, path_maxlen);
	tpp_include_path_entry *new_list;

	/* Check if the path already exists. */
	for (i = 0; i < self->tipl_size; ++i) {
		tpp_include_path_entry *entry = &self->tipl_list[i];
		if (tpp_include_path_entry_equals(entry, path, pathlen)) {
			/* Move entry to back */
			tpp_include_path_entry temp = *entry;
			++i;
			tpp_memmovedown(entry, entry + 1,
			                (self->tipl_size - i) *
			                sizeof(tpp_include_path_entry));
			self->tipl_list[self->tipl_size - 1] = temp;
			return TPP_EOK;
		}
	}

	/* Allocate a new entry. */
	newpath = tpp_include_path_string_new(path, pathlen);
	if tpp_unlikely(!newpath)
		return TPP_ENOMEM;

	/* Must append the new entry. */
	new_list = (tpp_include_path_entry *)tpp_realloc(self->tipl_list,
	                                                 (self->tipl_size + 1) *
	                                                 sizeof(tpp_include_path_entry));
	if tpp_unlikely(!new_list) {
		tpp_include_path_string_free(newpath);
		return TPP_ENOMEM;
	}
	tpp_include_path_entry_init_inherited(&new_list[self->tipl_size], newpath);
	self->tipl_list = new_list;
	++self->tipl_size;
	return TPP_EOK;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_include_path_list_pushhead(tpp_include_path_list *tpp_restrict self,
                               char const *path, tpp_size path_maxlen) {
	tpp_size i;
	tpp_include_path_string *newpath;
	tpp_size pathlen = tpp_include_path_strlen(path, path_maxlen);
	tpp_include_path_entry *new_list;

	/* Check if the path already exists. */
	for (i = 0; i < self->tipl_size; ++i) {
		tpp_include_path_entry const *entry = &self->tipl_list[i];
		if (tpp_include_path_entry_equals(entry, path, pathlen)) {
			/* Move entry to front */
			tpp_include_path_entry temp = *entry;
			tpp_memmoveup(&self->tipl_list[1],
			              &self->tipl_list[0],
			              i * sizeof(tpp_include_path_entry));
			self->tipl_list[0] = temp;
			return TPP_EOK;
		}
	}

	/* Allocate a new entry. */
	newpath = tpp_include_path_string_new(path, pathlen);
	if tpp_unlikely(!newpath)
		return TPP_ENOMEM;

	/* Must append the new entry. */
	new_list = (tpp_include_path_entry *)tpp_realloc(self->tipl_list,
	                                                 (self->tipl_size + 1) *
	                                                 sizeof(tpp_include_path_entry));
	if tpp_unlikely(!new_list) {
		tpp_include_path_string_free(newpath);
		return TPP_ENOMEM;
	}
	tpp_memmoveup(&new_list[1], &new_list[0],
	              self->tipl_size *
	              sizeof(tpp_include_path_entry));
	tpp_include_path_entry_init_inherited(&new_list[0], newpath);
	self->tipl_list = new_list;
	++self->tipl_size;
	return TPP_EOK;
}

/* Remove the given "path" from `self`.
 * @param: path:        The path to remove.
 * @param: path_maxlen: The max length of "path". The actual length of the path that will
 *                      be removed from the path list is "tpp_strnlen(path, path_maxlen)".
 *
 * @return: TPP_EOK:    Path was located and removed
 * @return: TPP_ENOENT: Path could not be found */
TPP_IMPL TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_include_path_list_remove(tpp_include_path_list *tpp_restrict self,
                             char const *path, tpp_size path_maxlen) {
	tpp_size i;
	tpp_size pathlen = tpp_include_path_strlen(path, path_maxlen);
	/* Check if the path already exists. */
	for (i = 0; i < self->tipl_size; ++i) {
		tpp_include_path_entry *entry = &self->tipl_list[i];
		if (tpp_include_path_entry_equals(entry, path, pathlen)) {
			/* Remove this entry */
			tpp_include_path_entry temp = *entry;
			--self->tipl_size;
			tpp_memmovedown(entry + 0, entry + 1,
			                (self->tipl_size - i) *
			                sizeof(tpp_include_path_entry));
			_tpp_include_path_entry_fini(&temp);
			return TPP_EOK;
		}
	}

	/* No such entry */
	return TPP_ENOENT;
}


#define tpp_include_paths_fini_common(self)               \
	(tpp_include_path_list_fini(&(self)->tip_system_list) \
	 _tpp_include_paths_fini_quote(self)                  \
	 _tpp_include_paths_fini_syshdr(self)                 \
	 _tpp_include_paths_fini_after(self)                  \
	 _tpp_include_paths_fini_embed(self))


/* Initialize/finalize include paths. */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_include_paths_fini(tpp_include_paths *tpp_restrict self) {
	tpp_include_paths_fini_common(self);

#if TPP_HAVE_INCLUDE_PATH_PUSH_POP
	{
		tpp_include_paths *prev = self->tip_prev;
		while (prev) {
			tpp_include_paths *pprev = prev->tip_prev;
			tpp_include_paths_fini_common(prev);
			_tpp_include_paths_free(prev);
			prev = pprev;
		}
	}
#endif /* TPP_HAVE_INCLUDE_PATH_PUSH_POP */
	tpp_dbg_memset(self, sizeof(*self));
}

/* Helper methods to add/remove paths to different include path lists */
#if TPP_HAVE_INCLUDE_PATH_PUSH_POP || TPP_HAVE_LEXER_COPY
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_include_path_list_copy(tpp_include_path_list *tpp_restrict self,
                           tpp_include_path_list const *tpp_restrict other) {
	tpp_size i;
	tpp_include_path_entry *listcopy;
	if (other->tipl_size == 0) {
		self->tipl_list = NULL;
		self->tipl_size = 0;
		return TPP_EOK;
	}
	listcopy = (tpp_include_path_entry *)tpp_malloc(other->tipl_size *
	                                                sizeof(tpp_include_path_entry));
	if tpp_unlikely(!listcopy)
		return TPP_ENOMEM;
	for (i = 0; i < other->tipl_size; ++i) {
		tpp_include_path_entry *dst = &listcopy[i];
		tpp_include_path_entry const *src = &other->tipl_list[i];
#if TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING
		dst->tipe_pathstr = src->tipe_pathstr;
		tpp_string_incref(dst->tipe_pathstr);
#else /* TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */
		tpp_size pathlen = tpp_strlen(src->tipe_path);
		char *path_copy = (char *)tpp_malloc((pathlen + 1) * sizeof(char));
		if tpp_unlikely(!path_copy) {
			while (i--)
				tpp_free(listcopy[i].tipe_path);
			tpp_free(listcopy);
			return TPP_ENOMEM;
		}
		tpp_memcpy(path_copy, src->tipe_path, (pathlen + 1) * sizeof(char));
		dst->tipe_path = path_copy;
#endif /* !TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */
	}
	self->tipl_size = other->tipl_size;
	self->tipl_list = listcopy;
	return TPP_EOK;
}

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_include_paths_copyone(tpp_include_paths *tpp_restrict self,
                          tpp_include_paths const *tpp_restrict from) {
	tpp_errno error;
#if TPP_HAVE_INCLUDE_PATH_PUSH_POP
	self->tip_pushcnt = from->tip_pushcnt;
#endif /* TPP_HAVE_INCLUDE_PATH_PUSH_POP */
#if TPP_HAVE_INCLUDE_PATH_EMBED
	error = tpp_include_path_list_copy(&self->tip_embed_list,
	                                   &from->tip_embed_list);
	if (TPP_ISERR(error))
		goto err;
#endif /* TPP_HAVE_INCLUDE_PATH_EMBED */
#if TPP_HAVE_INCLUDE_PATH_AFTER
	error = tpp_include_path_list_copy(&self->tip_after_list,
	                                   &from->tip_after_list);
	if (TPP_ISERR(error))
		goto err_embed;
#endif /* TPP_HAVE_INCLUDE_PATH_AFTER */
#if TPP_HAVE_INCLUDE_PATH_SYSHDR
	error = tpp_include_path_list_copy(&self->tip_syshdr_list,
	                                   &from->tip_syshdr_list);
	if (TPP_ISERR(error))
		goto err_embed_after;
#endif /* TPP_HAVE_INCLUDE_PATH_SYSHDR */
#if TPP_HAVE_INCLUDE_PATH_QUOTE
	error = tpp_include_path_list_copy(&self->tip_quote_list,
	                                   &from->tip_quote_list);
	if (TPP_ISERR(error))
		goto err_embed_after_syshdr;
#endif /* TPP_HAVE_INCLUDE_PATH_QUOTE */
	error = tpp_include_path_list_copy(&self->tip_system_list,
	                                   &from->tip_system_list);
	if (TPP_ISERR(error))
		goto err_embed_after_syshdr_quote;
	return error;
err_embed_after_syshdr_quote:
#if TPP_HAVE_INCLUDE_PATH_QUOTE
	tpp_include_path_list_fini(&self->tip_quote_list);
err_embed_after_syshdr:
#endif /* TPP_HAVE_INCLUDE_PATH_QUOTE */
#if TPP_HAVE_INCLUDE_PATH_SYSHDR
	tpp_include_path_list_fini(&self->tip_syshdr_list);
err_embed_after:
#endif /* TPP_HAVE_INCLUDE_PATH_SYSHDR */
#if TPP_HAVE_INCLUDE_PATH_AFTER
	tpp_include_path_list_fini(&self->tip_after_list);
err_embed:
#endif /* TPP_HAVE_INCLUDE_PATH_AFTER */
#if TPP_HAVE_INCLUDE_PATH_EMBED
	tpp_include_path_list_fini(&self->tip_embed_list);
err:
#endif /* TPP_HAVE_INCLUDE_PATH_EMBED */
	return error;
}
#endif /* TPP_HAVE_INCLUDE_PATH_PUSH_POP || TPP_HAVE_LEXER_COPY */


#if TPP_HAVE_LEXER_COPY
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_include_paths_copy(tpp_include_paths *tpp_restrict self,
                       tpp_include_paths const *tpp_restrict from) {
#if TPP_HAVE_INCLUDE_PATH_PUSH_POP
	tpp_include_paths *last = self;
	tpp_errno error = tpp_include_paths_copyone(self, from);
	if (TPP_ISERR(error))
		return error;
	while (from->tip_prev) {
		tpp_include_paths *from_prev_copy;
		from = from->tip_prev;
		from_prev_copy = _tpp_include_paths_alloc();
		if tpp_unlikely(!from_prev_copy) {
			error = TPP_ENOMEM;
			goto destroy_copied_warning_states_and_return_error;
		}
		last->tip_prev = from_prev_copy;
		error = tpp_include_paths_copyone(from_prev_copy, from);
		if (TPP_ISERR(error)) {
			_tpp_include_paths_free(from_prev_copy);
destroy_copied_warning_states_and_return_error:
			if (self != last) {
				tpp_include_paths *iter = self->tip_prev;
				for (;;) {
					tpp_include_paths *iter_prev = iter->tip_prev;
					tpp_include_paths_fini(iter);
					_tpp_include_paths_free(iter);
					if (iter == last)
						break;
					iter = iter_prev;
				}
			}
			return error;
		}
		last = from_prev_copy;
	}
	last->tip_prev = NULL;
	return TPP_EOK;
#else /* TPP_HAVE_INCLUDE_PATH_PUSH_POP */
	return tpp_include_paths_copyone(self, from);
#endif /* !TPP_HAVE_INCLUDE_PATH_PUSH_POP */
}
#endif /* TPP_HAVE_LEXER_COPY */


#if TPP_HAVE_INCLUDE_PATH_PUSH_POP
static TPP_WUNUSED TPP_NONNULL((1)) tpp_include_paths *TPPCALL
tpp_include_paths_dupone(tpp_include_paths const *tpp_restrict self) {
	tpp_include_paths *result = _tpp_include_paths_alloc();
	if tpp_unlikely(!result)
		goto err;
	if (TPP_ISERR(tpp_include_paths_copyone(result, self)))
		goto err_r;
#if TPP_HAVE_WARNINGS_PUSH_POP
	result->tip_prev = self->tip_prev; /* Hadn't been copied by "tpp_include_paths_copyone()" */
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */
	return result;
err_r:
	_tpp_include_paths_free(result);
err:
	return NULL;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_include_paths_unshare(tpp_include_paths *tpp_restrict self) {
	if (tpp_include_paths_mustcopy(self)) {
		/* Create a copy... */
		tpp_include_paths *copy = tpp_include_paths_dupone(self);
		if tpp_unlikely(!copy)
			return TPP_ENOMEM;

		/* Link the copy into the push-stack, and remember
		 * that no pushes happened since the last copy. */
		self->tip_prev = copy;
		self->tip_pushcnt = 0;
	}
	return TPP_EOK;
}


#if TPP_HAVE_INCLUDE_PATH_MULTIPLE
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_include_paths_addbykind(tpp_include_paths *tpp_restrict self,
                            tpp_include_path_kind kind,
                            char const *path, tpp_size path_maxlen)
#else /* TPP_HAVE_INCLUDE_PATH_MULTIPLE */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_include_paths_addbykind(tpp_include_paths *tpp_restrict self,
                             char const *path, tpp_size path_maxlen)
#endif /* !TPP_HAVE_INCLUDE_PATH_MULTIPLE */
{
	tpp_errno error = tpp_include_paths_unshare(self);
	if (!TPP_ISERR(error)) {
		error = tpp_include_path_list_pushtail(_tpp_include_paths_bykind(self, kind),
		                                       path, path_maxlen);
	}
	return error;
}

#if TPP_HAVE_INCLUDE_PATH_MULTIPLE
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_include_paths_addbykind_head(tpp_include_paths *tpp_restrict self,
                                 tpp_include_path_kind kind,
                                 char const *path, tpp_size path_maxlen)
#else /* TPP_HAVE_INCLUDE_PATH_MULTIPLE */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_include_paths_addbykind_head(tpp_include_paths *tpp_restrict self,
                                  char const *path, tpp_size path_maxlen)
#endif /* !TPP_HAVE_INCLUDE_PATH_MULTIPLE */
{
	tpp_errno error = tpp_include_paths_unshare(self);
	if (!TPP_ISERR(error)) {
		error = tpp_include_path_list_pushhead(_tpp_include_paths_bykind(self, kind),
		                                       path, path_maxlen);
	}
	return error;
}

/* @return: TPP_EOK:    Path was located and removed
 * @return: TPP_ENOENT: Path could not be found
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HAVE_INCLUDE_PATH_MULTIPLE
TPP_IMPL TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_include_paths_delbykind(tpp_include_paths *tpp_restrict self,
                            tpp_include_path_kind kind,
                            char const *path, tpp_size path_maxlen)
#else /* TPP_HAVE_INCLUDE_PATH_MULTIPLE */
TPP_IMPL TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_include_paths_delbykind(tpp_include_paths *tpp_restrict self,
                             char const *path, tpp_size path_maxlen)
#endif /* !TPP_HAVE_INCLUDE_PATH_MULTIPLE */
{
	tpp_errno error = tpp_include_paths_unshare(self);
	if (!TPP_ISERR(error)) {
		error = tpp_include_path_list_remove(_tpp_include_paths_bykind(self, kind),
		                                     path, path_maxlen);
	}
	return error;
}

/* @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HAVE_INCLUDE_PATH_MULTIPLE
TPP_IMPL TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_include_paths_clearbykind(tpp_include_paths *tpp_restrict self,
                              tpp_include_path_kind kind)
#else /* TPP_HAVE_INCLUDE_PATH_MULTIPLE */
TPP_IMPL TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_include_paths_clearbykind(tpp_include_paths *tpp_restrict self)
#endif /* !TPP_HAVE_INCLUDE_PATH_MULTIPLE */
{
	tpp_errno error = tpp_include_paths_unshare(self);
	if (!TPP_ISERR(error))
		tpp_include_path_list_clear(_tpp_include_paths_bykind(self, kind));
	return error;
}

/* Pop the current include paths state (may only
 * be called when `tpp_include_paths_canpop(self)`) */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_include_paths_pop(tpp_include_paths *tpp_restrict self) {
	tpp_assert(tpp_include_paths_canpop(self));
	if (self->tip_pushcnt == 0) {
		tpp_include_paths *prev = self->tip_prev;
		tpp_include_paths_fini_common(self);
		tpp_memcpy(self, prev, sizeof(tpp_include_paths));
		_tpp_include_paths_free(prev);
		tpp_assert(self->tip_pushcnt != 0);
	}
	--self->tip_pushcnt;
}

#endif /* TPP_HAVE_INCLUDE_PATH_PUSH_POP */
#endif /* TPP_HAVE_INCLUDE_PATH */


#if TPP_HAVE_INCLUDE_PATH_ENVIRON
/* Marker for `tpp_envinclude_paths` to indicate
 * loaded-but-empty. This points to a \0-character */
TPP_CONST_IMPL char const _tpp_envinclude_cpath_empty[1] = { '\0' };

#if TPP_HAVE_LEXER_COPY
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_envinclude_paths_copy(tpp_envinclude_paths *tpp_restrict self,
                          tpp_envinclude_paths const *tpp_restrict from) {
	self->teip_cpath = from->teip_cpath;
	if (self->teip_cpath != NULL &&
	    self->teip_cpath != (char *)_tpp_envinclude_cpath_empty) {
		tpp_size whole_len = 1;
		char *copy, *iter = self->teip_cpath;
		while (*iter) {
			tpp_size part = tpp_strlen(iter) + 1;
			iter += part;
			whole_len += part;
		}
		copy = (char *)tpp_malloc(whole_len * sizeof(char));
		if tpp_unlikely(!copy)
			return TPP_ENOMEM;
		copy = (char *)tpp_memcpy(copy, self->teip_cpath, whole_len * sizeof(char));
		self->teip_cpath = copy;
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_COPY */

typedef struct tpp_envinclude_paths_builder {
	char    *teipb_buf; /* [0..teipb_len][owned] Path buffer */
	tpp_size teipb_len; /* Used buffer size */
	tpp_size teipb_alc; /* Allocated buffer size (excluding 1 always-allocated trailing character) */
} tpp_envinclude_paths_builder;

#define tpp_envinclude_paths_builder_init(self) \
	(void)((self)->teipb_buf = NULL,            \
	       (self)->teipb_len = 0,               \
	       (self)->teipb_alc = 0)
#define tpp_envinclude_paths_builder_fini(self) \
	tpp_free((self)->teipb_buf)
static TPP_WUNUSED TPP_RETNONNULL TPP_NONNULL((1)) char *TPPCALL
tpp_envinclude_paths_builder_pack(tpp_envinclude_paths_builder *tpp_restrict self) {
	char *result;
	if (self->teipb_len == 0) {
		/* No paths defined... */
		tpp_free(self->teipb_buf);
		return (char *)_tpp_envinclude_cpath_empty;
	}
	/* Release unused memory... */
	result = (char *)tpp_tryrealloc(self->teipb_buf,
	                                (self->teipb_len + 1) *
	                                sizeof(char));
	if (result == NULL)
		result = self->teipb_buf;
	tpp_assert(result);
	result[self->teipb_len] = '\0'; /* Add final (secondary) trailing NUL */
	return result;
}

static TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL
tpp_envinclude_paths_alloc(tpp_envinclude_paths_builder *tpp_restrict self,
                           tpp_size num_chars) {
	char *result;
	tpp_size avail = self->teipb_alc - self->teipb_len;
	tpp_assert(self->teipb_alc >= self->teipb_len);
	if (num_chars > avail) {
		tpp_size new_alloc = self->teipb_alc * 2;
		tpp_size min_alloc = self->teipb_len + num_chars;
		if (new_alloc < min_alloc)
			new_alloc = min_alloc * 2;
		result = (char *)tpp_tryrealloc(self->teipb_buf, (new_alloc + 1) * sizeof(char));
		if tpp_unlikely(!result) {
			new_alloc = min_alloc;
			result = (char *)tpp_realloc(self->teipb_buf, (new_alloc + 1) * sizeof(char));
			if tpp_unlikely(!result)
				return NULL;
		}
		self->teipb_buf = result;
		self->teipb_alc = new_alloc;
	}
	result = self->teipb_buf;
	result += self->teipb_len;
	self->teipb_len += num_chars;
	return result;
}

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_envinclude_paths_builder_addpath(tpp_envinclude_paths_builder *tpp_restrict self,
                                     char const *tpp_restrict path, tpp_size pathlen) {
	char *buf;
	if (pathlen == 0) {
		/* Special case: an entirely empty path must be treated as `.` (i.e.: $PWD) */
		path = ".";
		pathlen = 1;
	}
	while (pathlen && TPP_FS_ISSEP(path[pathlen - 1]))
		--pathlen;
	/* +2 == +1 (trailing TPP_FS_SEP) +1 (trailing \0) */
	buf = tpp_envinclude_paths_alloc(self, pathlen + 2);
	if tpp_unlikely(!buf)
		return TPP_ENOMEM;
	buf = (char *)tpp_mempcpy(buf, path, pathlen * sizeof(char));
	*buf++ = TPP_FS_SEP;
	*buf++ = '\0';
	return TPP_EOK;
}

static tpp_errno TPPCALL
tpp_envinclude_paths_parse(void *arg, char const *envvalue) {
	tpp_errno error;
	tpp_envinclude_paths_builder *const self = (tpp_envinclude_paths_builder *)arg;
	for (;;) {
		char const *delim = tpp_strchr(envvalue, TPP_FS_DELIM);
		if (!delim)
			return tpp_envinclude_paths_builder_addpath(self, envvalue, tpp_strlen(envvalue));
		error = tpp_envinclude_paths_builder_addpath(self, envvalue, (tpp_size)(delim - envvalue));
		if (TPP_ISERR(error))
			break;
		envvalue = delim + 1;
	}
	return error;
}

/* Allocate a new set of paths */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_envinclude_paths_newpaths(char **tpp_restrict p_result) {
	tpp_errno error;
	tpp_envinclude_paths_builder builder;
	tpp_envinclude_paths_builder_init(&builder);

	/* Parse environment variables */
#define tpp_envinclude_paths_tuple_item(_, index, value)                  \
	error = tpp_io_withenv(value, &tpp_envinclude_paths_parse, &builder); \
	if (TPP_ISERR(error))                                                 \
		goto err;
	TPP_TUPLE_FOREACH(TPP_CONFIG_INCLUDE_PATH_ENVIRON,
	                  TPP_TUPLE_FOREACH_DUMMY_SEP,
	                  tpp_envinclude_paths_tuple_item,
	                  ~)
#undef tpp_envinclude_paths_tuple_sep
#undef tpp_envinclude_paths_tuple_item
	*p_result = tpp_envinclude_paths_builder_pack(&builder);
	tpp_assert(*p_result);
	return TPP_EOK;
err:
	tpp_envinclude_paths_builder_fini(&builder);
	return error;
}

/* Lazily initialize (if not already initialized) `self` and (on success) store
 * a pointer to the cached \0\0-terminated string array of paths that should be
 * searched. If no paths are defined, return `TPP_EOK` with `**p_result == '\0'`
 *
 * @return: TPP_EOK:    Success (including the case where `*p_result` directly points
 *                      at the trailing array-element, in which case no environment
 *                      variables of interest were defined)
 * @return: TPP_ENOMEM: HARD_ERROR: Out of memory
 * @return: * :         HARD_ERROR: Some other hard error */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_envinclude_paths_getpaths(tpp_envinclude_paths *tpp_restrict self,
                              char const **tpp_restrict p_result) {
	if (self->teip_cpath == NULL) {
		/* Allocate paths... */
		tpp_errno error = tpp_envinclude_paths_newpaths(&self->teip_cpath);
		if (TPP_ISERR(error))
			return error;
	}
	tpp_assert(self->teip_cpath);
	*p_result = self->teip_cpath;
	return TPP_EOK;
}
#endif /* TPP_HAVE_INCLUDE_PATH_ENVIRON */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_SYSINCLUDE_C */
