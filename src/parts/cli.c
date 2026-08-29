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
#ifndef GUARD_TPP_CLI_C
#define GUARD_TPP_CLI_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "cli.h"
#include "config.h"
#include "error.h"
#include "lexer.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_CLI

/* Define a function `tpp_lexer_openfile_for_cli()` */
#undef TPP_HAVE_CLI_OPEN_OFR
#define TPP_HAVE_CLI_OPEN_OFR \
	(TPP_HAVE_CLI_DASH_INCLUDE || TPP_HAVE_CLI_DASH_IMACROS)

/* Define a function `tpp_cli_loader_add_include_with_prefix()` */
#undef TPP_HAVE_CLI_ADD_INCLUDE_WITH_PREFIX
#define TPP_HAVE_CLI_ADD_INCLUDE_WITH_PREFIX \
	(TPP_HAVE_CLI_DASH_IWITHPREFIX ||        \
	 TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE)

/* Define a function `tpp_cli_loader_add_include()` */
#undef TPP_HAVE_CLI_ADD_INCLUDE
#define TPP_HAVE_CLI_ADD_INCLUDE            \
	(TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY || \
	 TPP_HAVE_CLI_DASH_IQUOTE ||            \
	 TPP_HAVE_CLI_DASH_ISYSTEM ||           \
	 TPP_HAVE_CLI_DASH_IDIRAFTER ||         \
	 TPP_HAVE_CLI_DASH_EMBED_DIR ||         \
	 TPP_HAVE_CLI_ADD_INCLUDE_WITH_PREFIX)

/* Define a function `tpp_lexer_cli_warnf()` */
#undef TPP_HAVE_LEXER_CLI_WARN
#define TPP_HAVE_LEXER_CLI_WARN                                \
	((TPP_HAVE_CLI_OPEN_OFR && TPP_HAVE_TPP_W_NO_SUCH_FILE) || \
	 (TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT))

/* Define a function `tpp_simple_atoz()` */
#undef TPP_HAVE_SIMPLE_ATOZ
#define TPP_HAVE_SIMPLE_ATOZ                 \
	(TPP_HAVE_CLI_DASH_FMAX_INCLUDE_DEPTH || \
	 TPP_HAVE_CLI_DASH_FTABSTOP ||           \
	 TPP_HAVE_CLI_DASH_FMAX_ERRORS)

/*[[[deemon
for (local option, what: {
	{"CPP_PREDEFINED_MACROS", "disable"},
	{"CPP_MACROS", "set"},
	{"TRIGRAPHS", "set"},
	{"TRIGRAPHS", "disable"},
	{"TRIGRAPHS", "enable"},
	{"BSE", "set"},
	{"CPP_INCLUDE", "set"},
	{"CPP_INCLUDE_NEXT", "set"},
	{"CPP_IMPORT", "set"},
	{"CPP_IF_ELSE_ENDIF", "set"},
	{"CPP_DEFINE", "set"},
	{"CPP_ASSERT", "set"},
	{"TOK_DOLLAR", "set"},
	{"TOK_COMMENT", "enable"},
	{"TOK_SPACE", "enable"},
	{"TOK_LF", "enable"},
	{"TRADITIONAL_MACROS", "enable"},
	{"TOK_CXX_COMMENT", "disable"},
	{"INCLUDE_RELATIVE_TO_CURRENT_FILE", "disable"},
	{"INCLUDE_SYSTEM_INCLUDE_PATH", "disable"},
	{"WERROR", "set"},
	{"INCLUDE_REMAP", "enable"},
}) {
	local extraArgs = what == "set" ? ", v" : "";
	print("#if TPP_CONF_ISEXT(TPP_HAVE_", option, ")");
	print("#define tpp_lexer_", what, "_", option, "(self", extraArgs, ") tpp_lexer_", what, "extension(self, TPP_EXT_", option, extraArgs, ")");
	print("#elif TPP_CONF_ISFEAT(TPP_HAVE_", option, ")");
	print("#define tpp_lexer_", what, "_", option, "(self", extraArgs, ") (tpp_lexer_", what, "feature(self, TPP_FEAT_", option, extraArgs, "), TPP_EOK)");
	print("#else /" "* ... *" "/");
	print("#define tpp_lexer_", what, "_", option, "(self", extraArgs, ") TPP_EOK");
	print("#endif /" "* !... *" "/");
}
]]]*/
#if TPP_CONF_ISEXT(TPP_HAVE_CPP_PREDEFINED_MACROS)
#define tpp_lexer_disable_CPP_PREDEFINED_MACROS(self) tpp_lexer_disableextension(self, TPP_EXT_CPP_PREDEFINED_MACROS)
#elif TPP_CONF_ISFEAT(TPP_HAVE_CPP_PREDEFINED_MACROS)
#define tpp_lexer_disable_CPP_PREDEFINED_MACROS(self) (tpp_lexer_disablefeature(self, TPP_FEAT_CPP_PREDEFINED_MACROS), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_CPP_PREDEFINED_MACROS(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_CPP_MACROS)
#define tpp_lexer_set_CPP_MACROS(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_MACROS, v)
#elif TPP_CONF_ISFEAT(TPP_HAVE_CPP_MACROS)
#define tpp_lexer_set_CPP_MACROS(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_MACROS, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_MACROS(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TRIGRAPHS)
#define tpp_lexer_set_TRIGRAPHS(self, v) tpp_lexer_setextension(self, TPP_EXT_TRIGRAPHS, v)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TRIGRAPHS)
#define tpp_lexer_set_TRIGRAPHS(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_TRIGRAPHS, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_TRIGRAPHS(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TRIGRAPHS)
#define tpp_lexer_disable_TRIGRAPHS(self) tpp_lexer_disableextension(self, TPP_EXT_TRIGRAPHS)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TRIGRAPHS)
#define tpp_lexer_disable_TRIGRAPHS(self) (tpp_lexer_disablefeature(self, TPP_FEAT_TRIGRAPHS), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_TRIGRAPHS(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TRIGRAPHS)
#define tpp_lexer_enable_TRIGRAPHS(self) tpp_lexer_enableextension(self, TPP_EXT_TRIGRAPHS)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TRIGRAPHS)
#define tpp_lexer_enable_TRIGRAPHS(self) (tpp_lexer_enablefeature(self, TPP_FEAT_TRIGRAPHS), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_TRIGRAPHS(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_BSE)
#define tpp_lexer_set_BSE(self, v) tpp_lexer_setextension(self, TPP_EXT_BSE, v)
#elif TPP_CONF_ISFEAT(TPP_HAVE_BSE)
#define tpp_lexer_set_BSE(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_BSE, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_BSE(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_CPP_INCLUDE)
#define tpp_lexer_set_CPP_INCLUDE(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_INCLUDE, v)
#elif TPP_CONF_ISFEAT(TPP_HAVE_CPP_INCLUDE)
#define tpp_lexer_set_CPP_INCLUDE(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_INCLUDE, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_INCLUDE(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_CPP_INCLUDE_NEXT)
#define tpp_lexer_set_CPP_INCLUDE_NEXT(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_INCLUDE_NEXT, v)
#elif TPP_CONF_ISFEAT(TPP_HAVE_CPP_INCLUDE_NEXT)
#define tpp_lexer_set_CPP_INCLUDE_NEXT(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_INCLUDE_NEXT, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_INCLUDE_NEXT(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_CPP_IMPORT)
#define tpp_lexer_set_CPP_IMPORT(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_IMPORT, v)
#elif TPP_CONF_ISFEAT(TPP_HAVE_CPP_IMPORT)
#define tpp_lexer_set_CPP_IMPORT(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_IMPORT, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_IMPORT(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_CPP_IF_ELSE_ENDIF)
#define tpp_lexer_set_CPP_IF_ELSE_ENDIF(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_IF_ELSE_ENDIF, v)
#elif TPP_CONF_ISFEAT(TPP_HAVE_CPP_IF_ELSE_ENDIF)
#define tpp_lexer_set_CPP_IF_ELSE_ENDIF(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_IF_ELSE_ENDIF, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_IF_ELSE_ENDIF(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_CPP_DEFINE)
#define tpp_lexer_set_CPP_DEFINE(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_DEFINE, v)
#elif TPP_CONF_ISFEAT(TPP_HAVE_CPP_DEFINE)
#define tpp_lexer_set_CPP_DEFINE(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_DEFINE, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_DEFINE(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_CPP_ASSERT)
#define tpp_lexer_set_CPP_ASSERT(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_ASSERT, v)
#elif TPP_CONF_ISFEAT(TPP_HAVE_CPP_ASSERT)
#define tpp_lexer_set_CPP_ASSERT(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_ASSERT, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_ASSERT(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TOK_DOLLAR)
#define tpp_lexer_set_TOK_DOLLAR(self, v) tpp_lexer_setextension(self, TPP_EXT_TOK_DOLLAR, v)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_DOLLAR)
#define tpp_lexer_set_TOK_DOLLAR(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_TOK_DOLLAR, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_TOK_DOLLAR(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TOK_COMMENT)
#define tpp_lexer_enable_TOK_COMMENT(self) tpp_lexer_enableextension(self, TPP_EXT_TOK_COMMENT)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_COMMENT)
#define tpp_lexer_enable_TOK_COMMENT(self) (tpp_lexer_enablefeature(self, TPP_FEAT_TOK_COMMENT), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_TOK_COMMENT(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TOK_SPACE)
#define tpp_lexer_enable_TOK_SPACE(self) tpp_lexer_enableextension(self, TPP_EXT_TOK_SPACE)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_SPACE)
#define tpp_lexer_enable_TOK_SPACE(self) (tpp_lexer_enablefeature(self, TPP_FEAT_TOK_SPACE), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_TOK_SPACE(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TOK_LF)
#define tpp_lexer_enable_TOK_LF(self) tpp_lexer_enableextension(self, TPP_EXT_TOK_LF)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_LF)
#define tpp_lexer_enable_TOK_LF(self) (tpp_lexer_enablefeature(self, TPP_FEAT_TOK_LF), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_TOK_LF(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TRADITIONAL_MACROS)
#define tpp_lexer_enable_TRADITIONAL_MACROS(self) tpp_lexer_enableextension(self, TPP_EXT_TRADITIONAL_MACROS)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TRADITIONAL_MACROS)
#define tpp_lexer_enable_TRADITIONAL_MACROS(self) (tpp_lexer_enablefeature(self, TPP_FEAT_TRADITIONAL_MACROS), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_TRADITIONAL_MACROS(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TOK_CXX_COMMENT)
#define tpp_lexer_disable_TOK_CXX_COMMENT(self) tpp_lexer_disableextension(self, TPP_EXT_TOK_CXX_COMMENT)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_CXX_COMMENT)
#define tpp_lexer_disable_TOK_CXX_COMMENT(self) (tpp_lexer_disablefeature(self, TPP_FEAT_TOK_CXX_COMMENT), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_TOK_CXX_COMMENT(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_INCLUDE_RELATIVE_TO_CURRENT_FILE)
#define tpp_lexer_disable_INCLUDE_RELATIVE_TO_CURRENT_FILE(self) tpp_lexer_disableextension(self, TPP_EXT_INCLUDE_RELATIVE_TO_CURRENT_FILE)
#elif TPP_CONF_ISFEAT(TPP_HAVE_INCLUDE_RELATIVE_TO_CURRENT_FILE)
#define tpp_lexer_disable_INCLUDE_RELATIVE_TO_CURRENT_FILE(self) (tpp_lexer_disablefeature(self, TPP_FEAT_INCLUDE_RELATIVE_TO_CURRENT_FILE), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_INCLUDE_RELATIVE_TO_CURRENT_FILE(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_INCLUDE_SYSTEM_INCLUDE_PATH)
#define tpp_lexer_disable_INCLUDE_SYSTEM_INCLUDE_PATH(self) tpp_lexer_disableextension(self, TPP_EXT_INCLUDE_SYSTEM_INCLUDE_PATH)
#elif TPP_CONF_ISFEAT(TPP_HAVE_INCLUDE_SYSTEM_INCLUDE_PATH)
#define tpp_lexer_disable_INCLUDE_SYSTEM_INCLUDE_PATH(self) (tpp_lexer_disablefeature(self, TPP_FEAT_INCLUDE_SYSTEM_INCLUDE_PATH), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_INCLUDE_SYSTEM_INCLUDE_PATH(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_WERROR)
#define tpp_lexer_set_WERROR(self, v) tpp_lexer_setextension(self, TPP_EXT_WERROR, v)
#elif TPP_CONF_ISFEAT(TPP_HAVE_WERROR)
#define tpp_lexer_set_WERROR(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_WERROR, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_WERROR(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_INCLUDE_REMAP)
#define tpp_lexer_enable_INCLUDE_REMAP(self) tpp_lexer_enableextension(self, TPP_EXT_INCLUDE_REMAP)
#elif TPP_CONF_ISFEAT(TPP_HAVE_INCLUDE_REMAP)
#define tpp_lexer_enable_INCLUDE_REMAP(self) (tpp_lexer_enablefeature(self, TPP_FEAT_INCLUDE_REMAP), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_INCLUDE_REMAP(self) TPP_EOK
#endif /* !... */
/*[[[end]]]*/


enum {
	_TPP_CLI_LOADER_STATE_FIRST_INTERNAL = TPP_CLI_LOADER_STATE_DDASH,
	/* Extra states go here... */
#if TPP_HAVE_CLI_DASH_DEFINE_MACRO
	TPP_CLI_LOADER_STATE_DEFINE_MACRO,   /* "--define-macro name[=definition]" */
#endif /* TPP_HAVE_CLI_DASH_DEFINE_MACRO */
#if TPP_HAVE_CLI_DASH_UNDEFINE_MACRO
	TPP_CLI_LOADER_STATE_UNDEFINE_MACRO, /* "--undefine-macro name" */
#endif /* TPP_HAVE_CLI_DASH_UNDEFINE_MACRO */
#if TPP_HAVE_CLI_DASH_ASSERT
	TPP_CLI_LOADER_STATE_ASSERT,         /* "--assert predicate=answer" */
#endif /* TPP_HAVE_CLI_DASH_ASSERT */
#if TPP_HAVE_CLI_DASH_INCLUDE
	TPP_CLI_LOADER_STATE_INCLUDE,        /* "-include file" */
#endif /* TPP_HAVE_CLI_DASH_INCLUDE */
#if TPP_HAVE_CLI_DASH_IMACROS
	TPP_CLI_LOADER_STATE_IMACROS,        /* "-imacros file" */
#endif /* TPP_HAVE_CLI_DASH_IMACROS */
#if TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY
	TPP_CLI_LOADER_STATE_INCLUDE_DIRECTORY, /* "-I dir" */
#endif /* TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY */
#if TPP_HAVE_CLI_DASH_IQUOTE
	TPP_CLI_LOADER_STATE_IQUOTE, /* "-iquote dir" */
#endif /* TPP_HAVE_CLI_DASH_IQUOTE */
#if TPP_HAVE_CLI_DASH_ISYSTEM
	TPP_CLI_LOADER_STATE_ISYSTEM, /* "-isystem dir" */
#endif /* TPP_HAVE_CLI_DASH_ISYSTEM */
#if TPP_HAVE_CLI_DASH_IDIRAFTER
	TPP_CLI_LOADER_STATE_IDIRAFTER, /* "-idirafter dir" */
#endif /* TPP_HAVE_CLI_DASH_IDIRAFTER */
#if TPP_HAVE_CLI_DASH_EMBED_DIR
	TPP_CLI_LOADER_STATE_EMBED_DIRECTORY, /* "--embed-directory dir" */
#endif /* TPP_HAVE_CLI_DASH_EMBED_DIR */
#if TPP_HAVE_CLI_DASH_IPREFIX
	TPP_CLI_LOADER_STATE_IPREFIX, /* "-iprefix dir" */
#endif /* TPP_HAVE_CLI_DASH_IPREFIX */
#if TPP_HAVE_CLI_DASH_IWITHPREFIX
	TPP_CLI_LOADER_STATE_IWITHPREFIX, /* "-iwithprefix dir" */
#endif /* TPP_HAVE_CLI_DASH_IWITHPREFIX */
#if TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE
	TPP_CLI_LOADER_STATE_IWITHPREFIXBEFORE, /* "-iwithprefixbefore dir" */
#endif /* TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE */
#if TPP_HAVE_CLI_DASH_ISYSROOT
	TPP_CLI_LOADER_STATE_ISYSROOT, /* "-isysroot dir" */
#endif /* TPP_HAVE_CLI_DASH_ISYSROOT */
};


#if TPP_HAVE_SIMPLE_ATOZ
static TPP_PURECALL tpp_size TPPCALL
tpp_simple_atoz(char const *value) {
	tpp_size result = 0;
	for (;;) {
		tpp_char ch = (tpp_char)(*value++);
		if (!tpp_ascii_isdigit(ch))
			break;
		tpp_assert(ch);
		result *= 10;
		result += tpp_ascii_asdigit(ch);
	}
	return result;
}
#endif /* TPP_HAVE_SIMPLE_ATOZ */

#if TPP_HAVE_CLI_NEEDS_FINI
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_cli_loader_fini(tpp_cli_loader *tpp_restrict self) {
#if TPP_HAVE_CLI_DASH_INCLUDE
	{
		tpp_size i;
		for (i = 0; i < self->tcl_includec; ++i) {
			tpp_lexer_openfile_result *ofr = &self->tcl_includev[i];
			tpp_lexer_openfile_result_fini(ofr);
		}
		tpp_free(self->tcl_includev);
	}
#endif /* TPP_HAVE_CLI_DASH_INCLUDE */
	tpp_dbg_memset(self, sizeof(tpp_cli_loader));
}
#endif /* TPP_HAVE_CLI_NEEDS_FINI */



#if TPP_HAVE_LEXER_CLI_WARN
static TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_cli_warnf(tpp_lexer *tpp_restrict self, tpp_char const *token_start,
                    tpp_size token_size, tpp_warning_id id, ...) {
	tpp_errno result;
	va_list args;
	union tpp_lexer_core saved_core = self->tl_core;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file_init_text_utf8(file, TPP_CONFIG_CLI_FILENAME,
	                        NULL, token_start, token_size,
	                        TPP_LCINFO_INVALID, TPP_FILE_FLAGS_NORMAL);
	file->tf_tpos = token_start;
	file->tf_pos  = token_start + token_size;
	va_start(args, id);
	result = tpp_lexer_vwarnf(self, id, args);
	va_end(args);
	self->tl_core = saved_core;
	return result;
}
#endif /* TPP_HAVE_LEXER_CLI_WARN */

#if TPP_HAVE_CLI_OPEN_OFR
#if TPP_HAVE_LEXER_OPEN_INCLUDE_STRING
struct tpp_lexer_openfile_for_cli_data {
	tpp_lexer                 *tloffcd_lexer;    /* [1..1] Lexer */
	char const                *tloffcd_filename; /* [1..1] Filename to open */
	tpp_lexer_openfile_result *tloffcd_result;   /* [1..1] Resulting OFR */
};
static tpp_errno TPPCALL
tpp_lexer_openfile_for_cli_cb(void *arg, char const *relative_to
                              tpp_lexer_foreach_include_path_flags__PARAM) {
	tpp_errno result;
	struct tpp_lexer_openfile_for_cli_data *data;
	data = (struct tpp_lexer_openfile_for_cli_data *)arg;
	result = tpp_lexer_openfile(data->tloffcd_lexer, relative_to,
	                            data->tloffcd_filename, TPP_SIZE_MAX,
	                            data->tloffcd_result);
#if TPP_HAVE_FILE_SYSHDR
	if (!TPP_ISERR(result))
		data->tloffcd_result->tlofr_fileflags |= flags;
#endif /* TPP_HAVE_FILE_SYSHDR */
	return result;
}
#endif /* TPP_HAVE_LEXER_OPEN_INCLUDE_STRING */


/* Open a file for use on CLI.
 * @return: TPP_EOK:    Success (`*result` was initialized)
 * @return: TPP_ENOENT: SOFT_ERROR: File not found (a warning was already emitted)
 * @return: * :         HARD_ERROR: Some other error (propagate it) */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_openfile_for_cli(tpp_lexer *tpp_restrict self, char const *filename,
                           tpp_lexer_openfile_result *tpp_restrict result) {
	tpp_errno status;
	tpp_size filename_len = tpp_strlen(filename);

	/* First off: try to open directly (i.e.: relative to PWD) */
	status = tpp_lexer_openfile(self, NULL, filename, filename_len, result);
	if (status != TPP_ENOENT)
		return status;

	/* Now do what GCC does, and walk the #include-stack, trying to find the matching
	 * file as though the given `filename` appeared in `#include "filename"`. */
#if TPP_HAVE_LEXER_OPEN_INCLUDE_STRING
	if (!TPP_FS_ISABS(filename, filename_len)) {
		struct tpp_lexer_openfile_for_cli_data data;
		data.tloffcd_lexer    = self;
		data.tloffcd_filename = filename;
		data.tloffcd_result   = result;
		status = tpp_lexer_foreach_include_path(self, TPP_TOK_OFCHAR('"'),
		                                        &tpp_lexer_openfile_for_cli_cb,
		                                        &data);
		if (status != TPP_ENOENT)
			return status;
	}
#endif /* TPP_HAVE_LEXER_OPEN_INCLUDE_STRING */

	/* Emit a file-not-found warning */
#if TPP_HAVE_TPP_W_NO_SUCH_FILE
	status = tpp_lexer_cli_warnf(self, (tpp_char const *)filename,
	                             filename_len, TPP_W_NO_SUCH_FILE);
	if (!TPP_ISERR(status))
		status = TPP_ENOENT;
	return status;
#else /* TPP_HAVE_TPP_W_NO_SUCH_FILE */
	return TPP_ENOENT;
#endif /* !TPP_HAVE_TPP_W_NO_SUCH_FILE */
}
#endif /* TPP_HAVE_CLI_OPEN_OFR */


#if TPP_HAVE_CLI_DASH_DEFINE_MACRO
/* Parse the macro definition that comes after "-D<arg>" */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_cli_loader_parse_define_macro(tpp_cli_loader *tpp_restrict self, char const *arg) {
	tpp_errno result;
	char const *eq = tpp_strchr(arg, '=');
	char const *body = "1";
	if (eq) {
		body = eq + 1;
	} else {
		eq = arg + tpp_strlen(arg);
	}
	result = tpp_lexer_define(self->tcl_lexer,
	                          arg, (tpp_size)(eq - arg),
	                          body, TPP_SIZE_MAX);
	return result;
}
#endif /* TPP_HAVE_CLI_DASH_DEFINE_MACRO */

#if TPP_HAVE_CLI_DASH_UNDEFINE_MACRO
/* Parse the macro un-definition that comes after "-U<arg>" */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_cli_loader_parse_undefine_macro(tpp_cli_loader *tpp_restrict self, char const *arg) {
	tpp_lexer_undef(self->tcl_lexer, arg, TPP_SIZE_MAX);
	return TPP_EOK;
}
#endif /* TPP_HAVE_CLI_DASH_UNDEFINE_MACRO */

#if TPP_HAVE_CLI_DASH_ASSERT
/* Parse the predicate definition that comes after "-A<arg>" */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_cli_loader_parse_assert(tpp_cli_loader *tpp_restrict self, char const *arg) {
	char const *eq = tpp_strchr(arg, '=');
	if (*arg == '-') {
		++arg;
		if (eq) {
			tpp_lexer_unassert(self->tcl_lexer, arg, (tpp_size)(eq - arg),
			                   eq + 1, TPP_SIZE_MAX);
		} else {
			tpp_lexer_unassertall(self->tcl_lexer, arg, TPP_SIZE_MAX);
		}
	} else if (eq) {
		return tpp_lexer_assert(self->tcl_lexer, arg, (tpp_size)(eq - arg), eq + 1, TPP_SIZE_MAX);
	} else {
		return tpp_lexer_assert(self->tcl_lexer, arg, TPP_SIZE_MAX, arg, TPP_SIZE_MAX); /* ??? */
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_CLI_DASH_ASSERT */



#if TPP_HAVE_CLI_DASH_INCLUDE
/* Parse the include that comes after "-include <arg>" */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_cli_loader_parse_include(tpp_cli_loader *tpp_restrict self, char const *arg) {
	tpp_lexer_openfile_result ofr;
	tpp_lexer_openfile_result *new_includev;
	tpp_errno result = tpp_lexer_openfile_for_cli(self->tcl_lexer, arg, &ofr);
	if (TPP_ISERR(result)) {
		if (result == TPP_ENOENT)
			result = TPP_EOK;
		return result;
	}

	/* Append "ofr" to "self->tcl_includev" */
	new_includev = (tpp_lexer_openfile_result *)tpp_realloc(self->tcl_includev,
	                                                        (self->tcl_includec + 1) *
	                                                        sizeof(tpp_lexer_openfile_result));
	if tpp_unlikely(!new_includev) {
		tpp_lexer_openfile_result_fini(&ofr);
		return TPP_ENOMEM;
	}
	new_includev[self->tcl_includec] = ofr;
	self->tcl_includev = new_includev;
	++self->tcl_includec;
	return TPP_EOK;
}
#endif /* TPP_HAVE_CLI_DASH_INCLUDE */

#if TPP_HAVE_CLI_DASH_IMACROS
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_include_imacros(tpp_lexer *tpp_restrict self,
                          /*inherit(always)*/ tpp_lexer_openfile_result *tpp_restrict ofr) {
	tpp_errno result = TPP_EOK;
	union tpp_lexer_core saved_core = self->tl_core;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file_init_io_from_ofr(file, ofr); /* Inherited */

	/* Yield tokens whilst discarding input (thereby evaluating directives
	 * and macros which may do other interesting stuff during expansion). */
	for (;;) {
		tpp_token_id tok = tpp_lexer_yield_blocking(self);
		if (tok == TPP_TOK_EOF)
			break;
		if (TPP_TOK_ISERR(tok)) {
			result = TPP_TOK_ASERR(tok);
			break;
		}
	}
	tpp_lexer_finifile(self);
	self->tl_core = saved_core;
	return result;
}

/* Parse the include that comes after "-imacros <arg>" */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_cli_loader_parse_imacros(tpp_cli_loader *tpp_restrict self, char const *arg) {
	tpp_lexer_openfile_result ofr;
	tpp_errno result = tpp_lexer_openfile_for_cli(self->tcl_lexer, arg, &ofr);
	if (TPP_ISERR(result)) {
		if (result == TPP_ENOENT)
			result = TPP_EOK;
		return result;
	}
	return tpp_lexer_include_imacros(self->tcl_lexer, &ofr);
}
#endif /* TPP_HAVE_CLI_DASH_IMACROS */


#if TPP_HAVE_CLI_DASH_FPREPROCESSED
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_set_fpreprocessed(tpp_lexer *tpp_restrict self, bool no) {
	tpp_errno result = tpp_lexer_set_CPP_MACROS(self, no);
	if (!TPP_ISERR(result))
		result = tpp_lexer_set_TRIGRAPHS(self, no);
	if (!TPP_ISERR(result))
		result = tpp_lexer_set_BSE(self, no);
	if (!TPP_ISERR(result))
		result = tpp_lexer_set_CPP_INCLUDE(self, no);
	if (!TPP_ISERR(result))
		result = tpp_lexer_set_CPP_INCLUDE_NEXT(self, no);
	if (!TPP_ISERR(result))
		result = tpp_lexer_set_CPP_IMPORT(self, no);
	if (!TPP_ISERR(result))
		result = tpp_lexer_set_CPP_IF_ELSE_ENDIF(self, no);
	if (!TPP_ISERR(result))
		result = tpp_lexer_set_CPP_DEFINE(self, no);
	if (!TPP_ISERR(result))
		result = tpp_lexer_set_CPP_ASSERT(self, no);
	return result;
}
#endif /* TPP_HAVE_CLI_DASH_FPREPROCESSED */


#if TPP_HAVE_CLI_DASH_COMMENTS
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_enable_comments(tpp_lexer *tpp_restrict self) {
	/* Enable COMMENT, SPACE and LF tokens */
	tpp_errno result = tpp_lexer_enable_TOK_COMMENT(self);
	if (!TPP_ISERR(result))
		result = tpp_lexer_enable_TOK_SPACE(self);
	if (!TPP_ISERR(result))
		result = tpp_lexer_enable_TOK_LF(self);
	return result;
}
#endif /* TPP_HAVE_CLI_DASH_COMMENTS */


#if TPP_HAVE_CLI_DASH_TRADITIONAL
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_enable_traditional(tpp_lexer *tpp_restrict self) {
	/* Enable traditional macro processing rules */
	tpp_errno result = tpp_lexer_enable_TRADITIONAL_MACROS(self);

	/* Traditional preprocessors didn't have trigraphs */
	if (!TPP_ISERR(result))
		result = tpp_lexer_disable_TRIGRAPHS(self);

	/* Traditional preprocessors didn't know //-comments */
	if (!TPP_ISERR(result))
		result = tpp_lexer_disable_TOK_CXX_COMMENT(self);
	return result;
}
#endif /* TPP_HAVE_CLI_DASH_TRADITIONAL */


#if TPP_HAVE_CLI_DASH_INCLUDE_BARRIER
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_enable_include_barrier(tpp_lexer *tpp_restrict self) {
#if TPP_HAVE_INCLUDE_PATH_QUOTE
	/* Move all include-paths of `tip_system_list` to `tip_quote_list` */
	tpp_include_path_list_fini(&self->tl_include_paths.tip_quote_list);
	self->tl_include_paths.tip_quote_list = self->tl_include_paths.tip_system_list;
	tpp_include_path_list_init(&self->tl_include_paths.tip_system_list);
#endif /* TPP_HAVE_INCLUDE_PATH_QUOTE */

	return tpp_lexer_disable_INCLUDE_RELATIVE_TO_CURRENT_FILE(self);
}
#endif /* TPP_HAVE_CLI_DASH_INCLUDE_BARRIER */


#if TPP_HAVE_CLI_ADD_INCLUDE
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_cli_loader_parse_addinclude(tpp_cli_loader *tpp_restrict self,
                                tpp_include_path_kind kind,
                                char const *path) {
#if TPP_HAVE_CLI_DASH_ISYSROOT
	if (*path == '=' || tpp_memcmp(path, "$SYSROOT", 8 * sizeof(char)) == 0) {
		/* XXX: Replace prefix with "system root" */
		++path;
		if (path[-1] == '$')
			path += (sizeof("SYSROOT") - sizeof(char));
		if (self->tcl_sysroot /*&& *self->tcl_sysroot*/) {
			tpp_errno result;
			tpp_size sysroot_len = tpp_strlen(self->tcl_sysroot);
			tpp_size path_len    = tpp_strlen(path);
			tpp_size whole_len   = sysroot_len + path_len;
			char *dst, *whole = (char *)tpp_malloc((whole_len + 1) * sizeof(char));
			if tpp_unlikely(!whole)
				return TPP_ENOMEM;
			dst = (char *)tpp_mempcpy(whole, self->tcl_sysroot, sysroot_len);
			dst = (char *)tpp_mempcpy(dst, path, path_len);
			*dst = '\0';
			result = tpp_lexer_includes_addbykind(self->tcl_lexer, kind, whole, whole_len);
			tpp_free(whole);
			return result;
		}
	}
#endif /* TPP_HAVE_CLI_DASH_ISYSROOT */
	return tpp_lexer_includes_addbykind(self->tcl_lexer, kind, path, TPP_SIZE_MAX);
}
#endif /* TPP_HAVE_CLI_ADD_INCLUDE */


#if TPP_HAVE_CLI_ADD_INCLUDE_WITH_PREFIX
#if TPP_HAVE_CLI_DASH_IPREFIX
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_cli_loader_parse_addinclude_with_prefix(tpp_cli_loader *tpp_restrict self,
                                            tpp_include_path_kind kind,
                                            char const *path) {
	tpp_errno result;
	char *whole_path;
	if (self->tcl_prefix == NULL)
		return tpp_cli_loader_parse_addinclude(self, kind, path);
	whole_path = tpp_joinpath(self->tcl_prefix, path, TPP_SIZE_MAX);
	if tpp_unlikely(!whole_path)
		return TPP_ENOMEM;
	result = tpp_cli_loader_parse_addinclude(self, kind, whole_path);
	tpp_free(whole_path);
	return result;
}
#else /* TPP_HAVE_CLI_DASH_IPREFIX */
#define tpp_cli_loader_parse_addinclude_with_prefix(self, kind, path) \
	tpp_cli_loader_parse_addinclude(self, kind, path)
#endif /* !TPP_HAVE_CLI_DASH_IPREFIX */
#endif /* TPP_HAVE_CLI_ADD_INCLUDE_WITH_PREFIX */


/* Parse the path that comes after "-iprefix <arg>" */
#if TPP_HAVE_CLI_DASH_IPREFIX
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_cli_loader_parse_iprefix(tpp_cli_loader *tpp_restrict self, char const *arg) {
	/* NOTE: GCC already documents that:
	 * """
	 *    If the prefix represents a directory, you should include the final '/'.
	 * """
	 *
	 * iow: What the `tpp_joinpath()` in `tpp_cli_loader_parse_addinclude_with_prefix()`
	 *      does with the prefix set here is already compatible with what GCC does, in
	 *      that `-iprefix` actually specifies a *file* that include paths should be
	 *      added relative to! */
	self->tcl_prefix = arg;
	return TPP_EOK;
}
#endif /* TPP_HAVE_CLI_DASH_IPREFIX */


/* Parse the path that comes after "-isysroot <arg>" */
#if TPP_HAVE_CLI_DASH_ISYSROOT
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_cli_loader_parse_isysroot(tpp_cli_loader *tpp_restrict self, char const *arg) {
	self->tcl_sysroot = arg;
	return TPP_EOK;
}
#endif /* TPP_HAVE_CLI_DASH_ISYSROOT */



/* Feed an argument to the loader. How exactly the argument is parsed
 * depends on the loader's current state, but sufficed to say: in its
 * default/initial state, `arg` should be something like `-Dfoo=bar`
 * or `-I/usr/include`, etc.
 *
 * WARNING: When you call this function, you must guaranty that `arg` remains
 *          valid, allocated, and unaltered until `tpp_cli_loader_fini()` is
 *          called.
 *
 * @return: TPP_EOK:       Success (argument was parsed + consumed)
 * @return: TPP_ENOENT:    SOFT_ERROR: Argument could not be understood (but no
 *                         warning was emitted). You must either handle it yourself
 *                         by treating it as an argument for *your* compiler's
 *                         CLI, or as an input file for it, or emit a warning
 *                         informing the user that their CLI argument was not
 *                         understood.
 * @return: TPP_ENOMEM:    HARD_ERROR: Out of memory
 * @return: TPP_EIO:       HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR: HARD_ERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  HARD_ERROR: User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_cli_loader_parsearg(tpp_cli_loader *tpp_restrict self, char const *arg) {
#define tpp_streq(at, CONSTstr) \
	(tpp_memcmp(at, CONSTstr, sizeof(CONSTstr) - sizeof(char)) == 0)
	switch (self->tcl_state) {

	case TPP_CLI_LOADER_STATE_NORMAL: {
		if (*arg++ != '-')
			break; /* All arguments must start with at least 1 "-" */
		switch (*arg++) {

/************************************************************************/
#if TPP_HAVE_CLI_DASH_DEFINE_MACRO
		case 'D':
			if (*arg) /* -D */
				return tpp_cli_loader_parse_define_macro(self, arg);
			self->tcl_state = TPP_CLI_LOADER_STATE_DEFINE_MACRO;
			return TPP_EOK;
#endif /* TPP_HAVE_CLI_DASH_DEFINE_MACRO */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CLI_DASH_UNDEFINE_MACRO
		case 'U':
			if (*arg) /* -U */
				return tpp_cli_loader_parse_undefine_macro(self, arg);
			self->tcl_state = TPP_CLI_LOADER_STATE_UNDEFINE_MACRO;
			return TPP_EOK;
#endif /* TPP_HAVE_CLI_DASH_UNDEFINE_MACRO */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CLI_DASH_ASSERT
		case 'A':
			if (*arg) /* -A */
				return tpp_cli_loader_parse_assert(self, arg);
			self->tcl_state = TPP_CLI_LOADER_STATE_ASSERT;
			return TPP_EOK;
#endif /* TPP_HAVE_CLI_DASH_ASSERT */
/************************************************************************/



/************************************************************************/
		case 'I':
#if TPP_HAVE_CLI_DASH_INCLUDE_BARRIER
			if (tpp_streq(arg, "-\0")) { /* -I- */
				return tpp_lexer_enable_include_barrier(self->tcl_lexer);
			} else
#endif /* TPP_HAVE_CLI_DASH_INCLUDE_BARRIER */
			{
#if TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY
				if (*arg)
					return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_SYSTEM, arg);
				self->tcl_state = TPP_CLI_LOADER_STATE_INCLUDE_DIRECTORY;
				return TPP_EOK;
#endif /* TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY */
			}
			break;
/************************************************************************/



/************************************************************************/
		case '-':
			/* --foo argument */
			switch (*arg++) {

			case '\0':
				self->tcl_state = TPP_CLI_LOADER_STATE_DDASH; /* -- */
				return TPP_EOK;

			case 'd':
#if TPP_HAVE_CLI_DASH_DEFINE_MACRO
				if (tpp_streq(arg, "efine-macro")) { /* --define-macro */
					arg += (sizeof("efine-macro") - sizeof(char));
					if (*arg == '=')
						return tpp_cli_loader_parse_define_macro(self, arg + 1);
					if (*arg == '\0') {
						self->tcl_state = TPP_CLI_LOADER_STATE_DEFINE_MACRO;
						return TPP_EOK;
					}
				} else
#endif /* TPP_HAVE_CLI_DASH_DEFINE_MACRO */
				{
				}
				break;

			case 'u':
#if TPP_HAVE_CLI_DASH_UNDEFINE_MACRO
				if (tpp_streq(arg, "ndefine-macro")) { /* --undefine-macro */
					arg += (sizeof("ndefine-macro") - sizeof(char));
					if (*arg == '=')
						return tpp_cli_loader_parse_undefine_macro(self, arg + 1);
					if (*arg == '\0') {
						self->tcl_state = TPP_CLI_LOADER_STATE_UNDEFINE_MACRO;
						return TPP_EOK;
					}
				} else
#endif /* TPP_HAVE_CLI_DASH_UNDEFINE_MACRO */
				{
				}
				break;

			case 'a':
#if TPP_HAVE_CLI_DASH_ASSERT
				if (tpp_streq(arg, "ssert")) { /* --assert */
					arg += (sizeof("ssert") - sizeof(char));
					if (*arg == '=')
						return tpp_cli_loader_parse_assert(self, arg + 1);
					if (*arg == '\0') {
						self->tcl_state = TPP_CLI_LOADER_STATE_ASSERT;
						return TPP_EOK;
					}
				} else
#endif /* TPP_HAVE_CLI_DASH_ASSERT */
				{
				}
				break;

			case 'i':
#if (TPP_HAVE_CLI_DASH_INCLUDE ||           \
     TPP_HAVE_CLI_DASH_INCLUDE_BARRIER ||   \
     TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY || \
     TPP_HAVE_CLI_DASH_IDIRAFTER ||         \
     TPP_HAVE_CLI_DASH_IPREFIX)
				if (tpp_streq(arg, "nclude")) { /* --include */
					arg += (sizeof("nclude") - sizeof(char));
#if TPP_HAVE_CLI_DASH_INCLUDE
					if (*arg == '=') {
						return tpp_cli_loader_parse_include(self, arg + 1);
					} else if (*arg == '\0') {
						self->tcl_state = TPP_CLI_LOADER_STATE_INCLUDE;
						return TPP_EOK;
					} else
#endif /* TPP_HAVE_CLI_DASH_INCLUDE */
#if TPP_HAVE_CLI_DASH_INCLUDE_BARRIER
					if (tpp_streq(arg, "-barrier\0")) { /* --include-barrier */
						return tpp_lexer_enable_include_barrier(self->tcl_lexer);
					} else
#endif /* TPP_HAVE_CLI_DASH_INCLUDE_BARRIER */
#if TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY || TPP_HAVE_CLI_DASH_IDIRAFTER
					if (tpp_streq(arg, "-directory")) { /* --include-directory= */
						arg += (sizeof("-directory") - sizeof(char));
#if TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY
						if (*arg == '=') {
							return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_SYSTEM, arg + 1);
						} else if (*arg == '\0') {
							self->tcl_state = TPP_CLI_LOADER_STATE_INCLUDE_DIRECTORY;
							return TPP_EOK;
						} else
#endif /* TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY */
#if TPP_HAVE_CLI_DASH_IDIRAFTER
						if (tpp_streq(arg, "-after")) { /* --include-directory-after= */
							arg += (sizeof("-after") - sizeof(char));
							if (*arg == '=')
								return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_AFTER, arg + 1);
							if (*arg == '\0') {
								self->tcl_state = TPP_CLI_LOADER_STATE_IDIRAFTER;
								return TPP_EOK;
							}
						} else
#endif /* TPP_HAVE_CLI_DASH_IDIRAFTER */
						{
						}
					} else
#endif /* TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY || TPP_HAVE_CLI_DASH_IDIRAFTER */
#if TPP_HAVE_CLI_DASH_IPREFIX
					if (tpp_streq(arg, "-prefix")) { /* --include-prefix= */
						arg += (sizeof("-prefix") - sizeof(char));
						if (*arg == '=') {
							return tpp_cli_loader_parse_iprefix(self, arg + 1);
						} else if (*arg == '\0') {
							self->tcl_state = TPP_CLI_LOADER_STATE_IPREFIX;
							return TPP_EOK;
						}
					} else
#endif /* TPP_HAVE_CLI_DASH_IPREFIX */
#if TPP_HAVE_CLI_DASH_IWITHPREFIX || TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE
					if (tpp_streq(arg, "-with-prefix")) { /* --include-with-prefix[...] */
						arg += (sizeof("-with-prefix") - sizeof(char));
#if TPP_HAVE_CLI_DASH_IWITHPREFIX
						if (*arg == '=') {
							return tpp_cli_loader_parse_addinclude_with_prefix(self, TPP_INCLUDE_PATH_KIND_AFTER, arg + 1);
						} else if (*arg == '\0') {
							self->tcl_state = TPP_CLI_LOADER_STATE_IWITHPREFIX;
							return TPP_EOK;
						} else if (tpp_streq(arg, "-after")) { /* --include-with-prefix-after= */
							arg += (sizeof("-after") - sizeof(char));
							if (*arg == '=') {
								return tpp_cli_loader_parse_addinclude_with_prefix(self, TPP_INCLUDE_PATH_KIND_AFTER, arg + 1);
							} else if (*arg == '\0') {
								self->tcl_state = TPP_CLI_LOADER_STATE_IWITHPREFIX;
								return TPP_EOK;
							}
						} else
#endif /* TPP_HAVE_CLI_DASH_IWITHPREFIX */
#if TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE
						if (tpp_streq(arg, "-before")) { /* --include-with-prefix-before= */
							arg += (sizeof("-before") - sizeof(char));
							if (*arg == '=') {
								return tpp_cli_loader_parse_addinclude_with_prefix(self, TPP_INCLUDE_PATH_KIND_SYSTEM, arg + 1);
							} else if (*arg == '\0') {
								self->tcl_state = TPP_CLI_LOADER_STATE_IWITHPREFIXBEFORE;
								return TPP_EOK;
							}
						} else
#endif /* TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE */
						{
						}
					} else
#endif /* TPP_HAVE_CLI_DASH_IWITHPREFIX || TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE */
					{
					}
				} else
#endif /* ... */
#if TPP_HAVE_CLI_DASH_IMACROS
				if (tpp_streq(arg, "macros")) { /* --imacros */
					arg += (sizeof("macros") - sizeof(char));
					if (*arg == '=')
						return tpp_cli_loader_parse_imacros(self, arg + 1);
					if (*arg == '\0') {
						self->tcl_state = TPP_CLI_LOADER_STATE_IMACROS;
						return TPP_EOK;
					}
				} else
#endif /* TPP_HAVE_CLI_DASH_IMACROS */
				{
				}
				break;

			case 'c':
#if TPP_HAVE_CLI_DASH_COMMENTS
				if (tpp_streq(arg, "omments\0") ||           /* --comments */
				    tpp_streq(arg, "omments-in-macros\0")) { /* --comments-in-macros */
					return tpp_lexer_enable_comments(self->tcl_lexer);
				} else
#endif /* TPP_HAVE_CLI_DASH_COMMENTS */
				{
				}
				break;

			case 'e':
#if TPP_HAVE_CLI_DASH_EMBED_DIR
				if (tpp_streq(arg, "embed-dir=")) { /* --embed-dir=... */
					arg += (sizeof("embed-dir=") - sizeof(char));
					return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_EMBED, arg);
				} else if (tpp_streq(arg, "embed-directory")) { /* --embed-directory */
					arg += (sizeof("embed-directory") - sizeof(char));
					if (*arg == '=') {
						return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_EMBED, arg + 1);
					} else if (*arg == '\0') {
						self->tcl_state = TPP_CLI_LOADER_STATE_EMBED_DIRECTORY;
						return TPP_EOK;
					}
				} else
#endif /* TPP_HAVE_CLI_DASH_EMBED_DIR */
				{
				}
				break;

			case 't':
#if TPP_HAVE_CLI_DASH_TRADITIONAL
				if (tpp_streq(arg, "raditional\0") ||     /* --traditional */
				    tpp_streq(arg, "raditional-cpp\0")) { /* --traditional-cpp */
					return tpp_lexer_enable_traditional(self->tcl_lexer);
				} else
#endif /* TPP_HAVE_CLI_DASH_TRADITIONAL */
#if TPP_HAVE_CLI_DASH_TRIGRAPHS
				if (tpp_streq(arg, "rigraphs\0")) { /* --trigraphs */
					return tpp_lexer_enable_TRIGRAPHS(self->tcl_lexer);
				} else
#endif /* TPP_HAVE_CLI_DASH_TRIGRAPHS */
				{
				}
				break;

			case 'n':
#if TPP_HAVE_CLI_DASH_NOSTDINC
				if (tpp_streq(arg, "o-standard-includes\0")) { /* --no-standard-includes */
					return tpp_lexer_disable_INCLUDE_SYSTEM_INCLUDE_PATH(self->tcl_lexer);
				} else
#endif /* TPP_HAVE_CLI_DASH_NOSTDINC */
				{
				}
				break;

			case 's':
#if TPP_HAVE_CLI_DASH_ISYSROOT
				if (tpp_streq(arg, "ysroot\0")) { /* --sysroot ... */
					self->tcl_state = TPP_CLI_LOADER_STATE_ISYSROOT;
					return TPP_EOK;
				} else
#endif /* TPP_HAVE_CLI_DASH_ISYSROOT */
				{
				}
				break;

			default: break;
			}
			break;
/************************************************************************/



/************************************************************************/
		case 'i':
#if TPP_HAVE_CLI_DASH_INCLUDE
			if (tpp_streq(arg, "nclude\0")) { /* -include ... */
				self->tcl_state = TPP_CLI_LOADER_STATE_INCLUDE;
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_INCLUDE */
#if TPP_HAVE_CLI_DASH_IMACROS
			if (tpp_streq(arg, "macros\0")) { /* -imacros ... */
				self->tcl_state = TPP_CLI_LOADER_STATE_IMACROS;
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_IMACROS */
#if TPP_HAVE_CLI_DASH_IQUOTE
			if (tpp_streq(arg, "quote\0")) { /* -iquote ... */
				self->tcl_state = TPP_CLI_LOADER_STATE_IQUOTE;
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_IQUOTE */
#if TPP_HAVE_CLI_DASH_ISYSTEM
			if (tpp_streq(arg, "system\0")) { /* -isystem ... */
				self->tcl_state = TPP_CLI_LOADER_STATE_ISYSTEM;
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_ISYSTEM */
#if TPP_HAVE_CLI_DASH_IDIRAFTER
			if (tpp_streq(arg, "dirafter\0")) { /* -idirafter ... */
				self->tcl_state = TPP_CLI_LOADER_STATE_IDIRAFTER;
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_IDIRAFTER */
#if TPP_HAVE_CLI_DASH_IPREFIX
			if (tpp_streq(arg, "prefix\0")) { /* -iprefix ... */
				self->tcl_state = TPP_CLI_LOADER_STATE_IPREFIX;
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_IPREFIX */
#if TPP_HAVE_CLI_DASH_IWITHPREFIX
			if (tpp_streq(arg, "withprefix\0")) { /* -iwithprefix ... */
				self->tcl_state = TPP_CLI_LOADER_STATE_IWITHPREFIX;
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_IWITHPREFIX */
#if TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE
			if (tpp_streq(arg, "withprefixbefore\0")) { /* -iwithprefixbefore ... */
				self->tcl_state = TPP_CLI_LOADER_STATE_IWITHPREFIXBEFORE;
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE */
#if TPP_HAVE_CLI_DASH_ISYSROOT
			if (tpp_streq(arg, "sysroot\0")) { /* -isysroot ... */
				self->tcl_state = TPP_CLI_LOADER_STATE_ISYSROOT;
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_ISYSROOT */
			{
			}
			break;
/************************************************************************/



/************************************************************************/
		case 'u':
#if TPP_HAVE_CLI_DASH_UNDEF
			if (tpp_streq(arg, "ndef\0")) { /* -undef */
				return tpp_lexer_disable_CPP_PREDEFINED_MACROS(self->tcl_lexer);
			} else
#endif /* TPP_HAVE_CLI_DASH_UNDEF */
			{
			}
			break;
/************************************************************************/



/************************************************************************/
		case 'n':
#if TPP_HAVE_CLI_DASH_NOSTDINC
			if (tpp_streq(arg, "ostdinc\0")) { /* -nostdinc */
				return tpp_lexer_disable_INCLUDE_SYSTEM_INCLUDE_PATH(self->tcl_lexer);
			} else
#endif /* TPP_HAVE_CLI_DASH_NOSTDINC */
			{
			}
			break;
/************************************************************************/



/************************************************************************/
		case 'r':
#if TPP_HAVE_CLI_DASH_REMAP
			if (tpp_streq(arg, "emap\0")) { /* -remap */
				return tpp_lexer_enable_INCLUDE_REMAP(self->tcl_lexer);
			} else
#endif /* TPP_HAVE_CLI_DASH_REMAP */
			{
			}
			break;
/************************************************************************/



/************************************************************************/
		case 'f': {
#undef tpp_cli__and_not_no
#if (TPP_HAVE_CLI_DASH_FPREPROCESSED ||           \
     TPP_HAVE_CLI_DASH_FDIRECTIVES_ONLY ||        \
     TPP_HAVE_CLI_DASH_FDOLLARS_IN_IDENTIFIERS || \
     TPP_HAVE_CLI_DASH_FEXTENSION)
			bool no = false;
			if (tpp_streq(arg, "no-")) {
				arg += 3;
				no = true;
			}
#define tpp_cli__and_not_no && !no
#else /* ... */
#define tpp_cli__and_not_no /* nothing */
#endif /* !... */

#if TPP_HAVE_CLI_DASH_FPREPROCESSED
			if (tpp_streq(arg, "preprocessed\0")) { /* -fpreprocessed */
				return tpp_lexer_set_fpreprocessed(self->tcl_lexer, no);
			} else
#endif /* TPP_HAVE_CLI_DASH_FPREPROCESSED */
#if TPP_HAVE_CLI_DASH_FDIRECTIVES_ONLY
			if (tpp_streq(arg, "directives-only\0")) { /* -fdirectives-only */
				return tpp_lexer_set_CPP_MACROS(self->tcl_lexer, no);
			} else
#endif /* TPP_HAVE_CLI_DASH_FDIRECTIVES_ONLY */
#if TPP_HAVE_CLI_DASH_FDOLLARS_IN_IDENTIFIERS
			if (tpp_streq(arg, "dollars-in-identifiers\0")) { /* -fdollars-in-identifiers */
				return tpp_lexer_set_TOK_DOLLAR(self->tcl_lexer, no);
			} else
#endif /* TPP_HAVE_CLI_DASH_FDOLLARS_IN_IDENTIFIERS */
#if TPP_HAVE_CLI_DASH_FMAX_INCLUDE_DEPTH
			if (tpp_streq(arg, "max-include-depth=") tpp_cli__and_not_no) { /* -fmax-include-depth=... */
				tpp_size new_limit;
				arg += (sizeof("max-include-depth=") - sizeof(char));
				new_limit = tpp_simple_atoz(arg);
				tpp_lexer_setinclusionlimit(self->tcl_lexer, new_limit);
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_FMAX_INCLUDE_DEPTH */
#if TPP_HAVE_CLI_DASH_FTABSTOP
			if (tpp_streq(arg, "tabstop=") tpp_cli__and_not_no) { /* -ftabstop=... */
				tpp_column new_stop;
				arg += (sizeof("tabstop=") - sizeof(char));
				new_stop = (tpp_column)tpp_simple_atoz(arg);
				tpp_settabsize(new_stop);
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_FTABSTOP */
#if TPP_HAVE_CLI_DASH_FMAX_ERRORS
			if (tpp_streq(arg, "max-errors=") tpp_cli__and_not_no) { /* -fmax-errors=... */
				tpp_size new_limit;
				arg += (sizeof("max-errors=") - sizeof(char));
				new_limit = tpp_simple_atoz(arg);
				tpp_lexer_seterrorlimit(self->tcl_lexer, new_limit);
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_FMAX_ERRORS */
#if TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH
			if (tpp_streq(arg, "search-include-path") tpp_cli__and_not_no) { /* -fsearch-include-path... */
				tpp_token_id new_mode = TPP_TOK_EOF;
				arg += (sizeof("search-include-path") - sizeof(char));
				if (*arg == '\0') {
					new_mode = TPP_TOK_INCPATH_DQUOTE;
				} else if (tpp_streq(arg, "=user\0")) {
					new_mode = TPP_TOK_INCPATH_DQUOTE;
				} else if (tpp_streq(arg, "=system\0")) {
					new_mode = TPP_TOK_INCPATH_LANGLE;
				}
				if (new_mode != TPP_TOK_EOF) {
					tpp_cli_loader_set_search_include_path_mode(self, new_mode);
					return TPP_EOK;
				}
			} else
#endif /* TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH */
			{
#if TPP_HAVE_CLI_DASH_FEXTENSION
				/* Fallback: configure an extension */
				tpp_extension_id extension = tpp_extension_byname(arg);
				if (extension != TPP_EXT_COUNT)
					return tpp_lexer_setextension(self->tcl_lexer, extension, !no);
#endif /* TPP_HAVE_CLI_DASH_FEXTENSION */
			}
#undef tpp_cli__and_not_no
		}	break;
/************************************************************************/



/************************************************************************/
		case 'W': {
#undef tpp_cli__and_not_no
#if (TPP_HAVE_CLI_DASH_WERROR ||       \
     TPP_HAVE_CLI_DASH_WFATAL_ERROR || \
     TPP_HAVE_CLI_DASH_WWARNING)
			bool no = false;
			if (tpp_streq(arg, "no-")) {
				arg += 3;
				no = true;
			}
#define tpp_cli__and_not_no && !no
#else /* ... */
#define tpp_cli__and_not_no /* nothing */
#endif /* !... */

#if TPP_HAVE_CLI_DASH_WERROR
			if (tpp_streq(arg, "error\0")) {
				return tpp_lexer_set_WERROR(self->tcl_lexer, !no);
			} else
#endif /* TPP_HAVE_CLI_DASH_WERROR */
#if TPP_HAVE_CLI_DASH_WFATAL_ERROR
			if (tpp_streq(arg, "fatal-errors\0")) {
				tpp_lexer_seterrorlimit(self->tcl_lexer, no ? (-TPP_ERROR_LIMIT) : 1);
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_WFATAL_ERROR */
#if TPP_HAVE_CLI_DASH_WERROR_WARNING
			if (tpp_streq(arg, "error=") tpp_cli__and_not_no) {
				tpp_warning_group_id wgid;
				arg += (sizeof("error=") - sizeof(char));
				wgid = tpp_warning_group_byname(arg);
				if (wgid != TPP_WG_COUNT)
					return tpp_lexer_setwarninggrp(self->tcl_lexer, wgid, TPP_WSTATE_ERROR_OR_FATAL);
			} else
#endif /* TPP_HAVE_CLI_DASH_WERROR_WARNING */
			{
#if TPP_HAVE_CLI_DASH_WWARNING
				/* Fallback: configure a warning */
				tpp_warning_group_id wgid = tpp_warning_group_byname(arg);
				if (wgid != TPP_WG_COUNT) {
					tpp_warning_state state;
					if (no) {
						state = TPP_WSTATE_DISABLED;
					} else {
						state = tpp_lexer_getwarninggrp(self->tcl_lexer, wgid);
						if (state == TPP_WSTATE_DISABLED) {
							state = TPP_WSTATE_WARN;
						} else if (state == TPP_WSTATE_WARN) {
							state = TPP_WSTATE_ERROR_OR_FATAL;
						} else
#if TPP_HAVE_WARNING_ERROR
						if (state == TPP_WSTATE_ERROR) {
							state = TPP_WSTATE_FATAL;
						} else
#endif /* TPP_HAVE_WARNING_ERROR */
						{
						}
					}
					return tpp_lexer_setwarninggrp(self->tcl_lexer, wgid, state);
				}
#endif /* TPP_HAVE_CLI_DASH_WWARNING */
			}
#undef tpp_cli__and_not_no
		}	break;
/************************************************************************/



/************************************************************************/
		case 'C':
#if TPP_HAVE_CLI_DASH_COMMENTS
			if (*arg == '\0' || tpp_streq(arg, "C\0")) { /* -C -CC */
				return tpp_lexer_enable_comments(self->tcl_lexer);
			} else
#endif /* TPP_HAVE_CLI_DASH_COMMENTS */
			{
			}
			break;
/************************************************************************/



/************************************************************************/
		case 't':
#if TPP_HAVE_CLI_DASH_TRADITIONAL
			if (tpp_streq(arg, "raditional\0") ||     /* -traditional */
			    tpp_streq(arg, "raditional-cpp\0")) { /* -traditional-cpp */
				return tpp_lexer_enable_traditional(self->tcl_lexer);
			} else
#endif /* TPP_HAVE_CLI_DASH_TRADITIONAL */
#if TPP_HAVE_CLI_DASH_TRIGRAPHS
			if (tpp_streq(arg, "rigraphs\0")) { /* -trigraphs */
				return tpp_lexer_enable_TRIGRAPHS(self->tcl_lexer);
			} else
#endif /* TPP_HAVE_CLI_DASH_TRIGRAPHS */
			{
			}
			break;
/************************************************************************/

		default: break;
		}
	}	break;

	case TPP_CLI_LOADER_STATE_DDASH:
		break; /* Don't accept any more arguments after having encountered a "--" arguments */

#if TPP_HAVE_CLI_DASH_DEFINE_MACRO
	case TPP_CLI_LOADER_STATE_DEFINE_MACRO:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_define_macro(self, arg);
#endif /* TPP_HAVE_CLI_DASH_DEFINE_MACRO */

#if TPP_HAVE_CLI_DASH_UNDEFINE_MACRO
	case TPP_CLI_LOADER_STATE_UNDEFINE_MACRO:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_undefine_macro(self, arg);
#endif /* TPP_HAVE_CLI_DASH_UNDEFINE_MACRO */

#if TPP_HAVE_CLI_DASH_ASSERT
	case TPP_CLI_LOADER_STATE_ASSERT:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_assert(self, arg);
#endif /* TPP_HAVE_CLI_DASH_ASSERT */

#if TPP_HAVE_CLI_DASH_INCLUDE
	case TPP_CLI_LOADER_STATE_INCLUDE:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_include(self, arg);
#endif /* TPP_HAVE_CLI_DASH_INCLUDE */

#if TPP_HAVE_CLI_DASH_IMACROS
	case TPP_CLI_LOADER_STATE_IMACROS:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_imacros(self, arg);
#endif /* TPP_HAVE_CLI_DASH_IMACROS */

#if TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY
	case TPP_CLI_LOADER_STATE_INCLUDE_DIRECTORY:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_SYSTEM, arg);
#endif /* TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY */

#if TPP_HAVE_CLI_DASH_IQUOTE
	case TPP_CLI_LOADER_STATE_IQUOTE:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_QUOTE, arg);
#endif /* TPP_HAVE_CLI_DASH_IQUOTE */

#if TPP_HAVE_CLI_DASH_ISYSTEM
	case TPP_CLI_LOADER_STATE_ISYSTEM:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_SYSHDR, arg);
#endif /* TPP_HAVE_CLI_DASH_ISYSTEM */

#if TPP_HAVE_CLI_DASH_IDIRAFTER
	case TPP_CLI_LOADER_STATE_IDIRAFTER:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_AFTER, arg);
#endif /* TPP_HAVE_CLI_DASH_IDIRAFTER */

#if TPP_HAVE_CLI_DASH_EMBED_DIR
	case TPP_CLI_LOADER_STATE_EMBED_DIRECTORY:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_EMBED, arg);
#endif /* TPP_HAVE_CLI_DASH_EMBED_DIR */

#if TPP_HAVE_CLI_DASH_IPREFIX
	case TPP_CLI_LOADER_STATE_IPREFIX:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_iprefix(self, arg);
#endif /* TPP_HAVE_CLI_DASH_IPREFIX */

#if TPP_HAVE_CLI_DASH_IWITHPREFIX
	case TPP_CLI_LOADER_STATE_IWITHPREFIX:
		self->tcl_state  = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_addinclude_with_prefix(self, TPP_INCLUDE_PATH_KIND_AFTER, arg);
#endif /* TPP_HAVE_CLI_DASH_IWITHPREFIX */

#if TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE
	case TPP_CLI_LOADER_STATE_IWITHPREFIXBEFORE:
		self->tcl_state  = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_addinclude_with_prefix(self, TPP_INCLUDE_PATH_KIND_SYSTEM, arg);
#endif /* TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE */

#if TPP_HAVE_CLI_DASH_ISYSROOT
	case TPP_CLI_LOADER_STATE_ISYSROOT:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_isysroot(self, arg);
#endif /* TPP_HAVE_CLI_DASH_ISYSROOT */

	default: tpp_unreachable();
	}
	return TPP_ENOENT;
#undef tpp_streq
}

/* Try to parse a *flag*-style parameter, that is: an argument that actually consists
 * of multiple, tightly packed parameters, whilst having a singular, leading `-` (that
 * was already skipped by the caller).
 *
 * Example: `-PH` or `-HP`
 * - This argument consists of 2 flags `-H` and `-P`, which are simply concatenated
 *   into a single argument here. This function will then parse one of those flags
 *   from `**p_arg` (iow: `**p_arg` must be one of `H` or `P`), and advance `*p_arg`
 *   to either the end of the argument, or the next *flag*-style parameter.
 *
 * @return: TPP_EOK:    Success (`*p_arg` was updated to point to the next *flag*-style
 *                      parameter, or the argument string's end)
 * @return: TPP_ENOENT: Did not recognize the flag in `**p_arg` (caller should try to
 *                      handle the flag in a different context).
 * @return: TPP_ENOMEM:    HARD_ERROR: Out of memory
 * @return: TPP_EIO:       HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR: HARD_ERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  HARD_ERROR: User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_cli_loader_parseflag(tpp_cli_loader *tpp_restrict self, char const **p_arg) {
	char const *arg = *p_arg;
	char flag = *arg++;
	(void)self;
	(void)flag;
	(void)arg;
	switch (flag) {

		/* ... */

	default: break;
	}
	return TPP_ENOENT;
}


/* Convenience wrapper around `tpp_cli_loader_parsearg()`:
 * - This function passes every argument given to `tpp_cli_loader_parsearg()`
 *   in the order they appear within the specified argument vector (as it was
 *   originally passed to your `main()`)
 * - Anything for which `tpp_cli_loader_parsearg()` returns `TPP_ENOENT` is
 *   pushed to the back of the argument vector such that it will still appear
 *   within `(*p_argv)` and be accounted for in `(*p_argc)` on return, as
 *   well as still appear in the same order.
 * - If an "--" argument is encountered, one of 2 things happen:
 *   - If there were already arguments that were pushed to the back, then
 *     all remaning un-parsed arguments (*INCLUDING*) the "--" argument are
 *     also pushed to the back and this function returns with `TPP_EOK` while
 *     the remainder of the argument list looks like:
 *     >> `{ "-unknown-arg", "--", "file1.c", "file2.c" }`
 *   - If no arguments were pushed to the back yet, then the given `p_argc`
 *     and `p_argv` are updated to point to everything that comes after the
 *     "--" argument (*INCLUDING* the "--" argument itself), and the function
 *     returns `TPP_EOK` while the remainder of the argument list looks like:
 *     >> `{ "--", "file1.c", "file2.c" }`
 * - Before calling this method, be sure to strip the leading program-name
 *   argument from the argc/argv your `main()` was given, before calling
 *   this function!
 *
 * @return: TPP_EOK:       Success (`*p_argc` and `*p_argv` were updated such that
 *                         they contain all unrecognized arguments, as well as all
 *                         input files for your compiler).
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_cli_loader_parseargv(tpp_cli_loader *tpp_restrict self,
                         int *p_argc, char ***p_argv) {
	tpp_errno result = TPP_EOK;
	int argc    = *p_argc;
	char **argv = *p_argv;
	unsigned int unknown_count = 0;
	while (argc > 0) {
		char *arg = argv[0];
		result = tpp_cli_loader_parsearg(self, arg);
		if (TPP_ISERR(result)) {
			if (result != TPP_ENOENT)
				break;
			/* Add "arg" to trailing list of unknown arguments */
			--argc;
			(void)tpp_memmovedown(&argv[0], &argv[1],
			                      (argc + unknown_count) *
			                      sizeof(char *));
			argv[argc + unknown_count] = arg;
			++unknown_count;
			result = TPP_EOK;
			continue;
		}
		if (tpp_cli_loader_hasddash(self)) {
			if (unknown_count) {
				/* Right now, "argv" looks like this:
				 * >> argv = { "--", "file1.c", "file2.c", "-unknown-arg", "file0.c" }
				 * >> argc = 3
				 * >> unknown_count = 2
				 *
				 * Our job now is to make `argv` look like this:
				 * >> argv = { "-unknown-arg", "file0.c", "--", "file1.c", "file2.c" } */
				unsigned int shift_count = unknown_count;
				unsigned int total_count_minus_1 = argc + unknown_count - 1;
				while (shift_count--) {
					arg = argv[0];
					(void)tpp_memmovedown(&argv[0], &argv[1],
					                      total_count_minus_1 *
					                      sizeof(char *));
					argv[total_count_minus_1] = arg;
				}
			}
			break;
		}
		++argv;
		--argc;
	}
	*p_argc = argc + unknown_count;
	*p_argv = argv;
	return result;
}

#if TPP_HAVE_CLI_SETINPUTS

typedef struct tpp_cli_loader_open_input_data {
	tpp_lexer_openfile_result tcloid_ofr;   /* Open-file-result */
#if TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH
	tpp_lexer                *tcloid_lexer; /* [1..1] Lexer */
	char const               *tcloid_file;  /* [1..1] Filename */
#endif /* TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH */
} tpp_cli_loader_open_input_data;

#if TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH
static tpp_errno TPPCALL
tpp_cli_loader_open_input_cb(void *arg, char const *relative_to
                             tpp_lexer_foreach_include_path_flags__PARAM) {
	tpp_cli_loader_open_input_data *data;
#if TPP_HAVE_FILE_SYSHDR
	(void)flags;
#endif /* TPP_HAVE_FILE_SYSHDR */
	data = (tpp_cli_loader_open_input_data *)arg;
	return tpp_lexer_openfile(data->tcloid_lexer, relative_to,
	                          data->tcloid_file, TPP_SIZE_MAX,
	                          &data->tcloid_ofr);
}
#endif /* TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH */

/* Open the specified `input_filename`
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: No such file
 * @return: * : Some other **HARD_ERROR** */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_cli_loader_open_input(tpp_cli_loader *tpp_restrict self,
                          char const *tpp_restrict input_filename,
                          tpp_file *tpp_restrict file) {
	tpp_errno result;
	tpp_cli_loader_open_input_data data;
#if TPP_HAVE_CLI_SETINPUTS_DASH
	if (tpp_strcmp(input_filename, "-") == 0) {
#ifdef tpp_io_getstdin
		tpp_io_handle std_input = tpp_io_getstdin();
#if TPP_HAVE_FILE_NOKWD
		tpp_file_init_io_ex(file, TPP_HAVE_CLI_SETINPUTS_STDIN_FILENAME,
		                    std_input, TPP_FILE_FLAGS_NOCLOSE | TPP_FILE_FLAGS_NOKWD);
#else /* TPP_HAVE_FILE_NOKWD */
		tpp_file_init_io_ex(file, NULL, std_input, TPP_FILE_FLAGS_NOCLOSE);
#endif /* !TPP_HAVE_FILE_NOKWD */
		return TPP_EOK;
#else /* tpp_io_getstdin */
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: `TPP_HAVE_CLI_SETINPUTS_DASH` is enabled, but `tpp_io_getstdin` isn't -> no way to retrieve hosting process's STDIN handle"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
		return TPP_ENOENT;
#endif /* !tpp_io_getstdin */
	}
#endif /* TPP_HAVE_CLI_SETINPUTS_DASH */

	/* Try to open the file directly */
	result = tpp_lexer_openfile(self->tcl_lexer, NULL, input_filename, TPP_SIZE_MAX, &data.tcloid_ofr);

	/* Try to open the file via extra #include-paths, as specified by `-fsearch-include-path=...` */
#if TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH
	if (result == TPP_ENOENT &&
	    self->tcl_search_include_path_mode != TPP_TOK_EOF &&
	    !TPP_FS_ISABS(input_filename, tpp_strlen(input_filename))) {
		tpp_lexer *const lexer = self->tcl_lexer;
		tpp_assert(self->tcl_search_include_path_mode == TPP_TOK_INCPATH_LANGLE ||
		           self->tcl_search_include_path_mode == TPP_TOK_INCPATH_DQUOTE);
		data.tcloid_lexer = self->tcl_lexer;
		data.tcloid_file  = input_filename;
#if TPP_HAVE_INCLUDE_RELATIVE_TO_CURRENT_FILE
		/* Calls to `tpp_lexer_foreach_include_path` would normally also try to include
		 * data relative to the lexer's current file (or possibly even additional files
		 * further up the #include-stack).
		 * 1. We don't want that to happen -- `-fsearch-include-path` should only cause
		 *    the additional #include-paths to be searched
		 * 2. The lexer's current file may not actually be initialized, yet!
		 *
		 * To prevent any issues here, override the lexer's file (and thereby also its
		 * #include-stack) to consist entirely of an unnamed dummy/empty text-file,
		 * thereby preventing any issues with uninitialized/unintended search-paths. */
		{
			tpp_file saved_file = *tpp_lexer_getfile(lexer);
			tpp_lexer_initfile_text_utf8(lexer, NULL, NULL, NULL, 0,
			                             TPP_LCINFO_INVALID,
			                             TPP_FILE_FLAGS_NORMAL);
			result = tpp_lexer_foreach_include_path(lexer, self->tcl_search_include_path_mode,
			                                        &tpp_cli_loader_open_input_cb,
			                                        &data);
			*tpp_lexer_getfile(lexer) = saved_file;
		}
#else /* TPP_HAVE_INCLUDE_RELATIVE_TO_CURRENT_FILE */
		result = tpp_lexer_foreach_include_path(lexer, self->tcl_search_include_path_mode,
		                                        &tpp_cli_loader_open_input_cb,
		                                        &data);
#endif /* !TPP_HAVE_INCLUDE_RELATIVE_TO_CURRENT_FILE */
	}
#endif /* TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH */

	/* If OFR could be loaded, initialize the file from it. */
	if (result != TPP_ENOENT) {
		if (!TPP_ISERR(result))
			tpp_file_init_io_from_ofr(file, &data.tcloid_ofr);
		return result;
	}

	/* Emit a warning about the file not being found */
#if TPP_HAVE_TPP_W_NO_SUCH_FILE
	result = tpp_lexer_cli_warnf(self->tcl_lexer, (tpp_char const *)input_filename,
	                             tpp_strlen(input_filename), TPP_W_NO_SUCH_FILE);
	if (TPP_ISERR(result))
		return result;
#endif /* TPP_HAVE_TPP_W_NO_SUCH_FILE */
	return TPP_ENOENT;
}

/* Use the given `argc` and `argv` as inputs for the lexer.
 *
 * This function should be used to pass everything on your `argv` following
 * a potential `--` argument, as well as all arguments that don't start
 * with a leading `-` (you should permutate your `argv` similar to how
 * that is also done by `tpp_cli_loader_parseargv()` such that all unknown
 * arguments, as well as a potential `--` argument appear last).
 *
 * This function will *always* initialize the lexer's *file-stack*, such
 * that upon successful return from this function, the caller is responsible
 * to finalize that part of the lexer using `tpp_lexer_finifile()`.
 *
 * - If there are inputs, a warning `TPP_W_NO_INPUT_FILES` is emitted.
 * - If one of the inputs cannot be opened, a warning `TPP_W_NO_SUCH_FILE` is emitted.
 *
 * @return: TPP_EOK:       Success
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_cli_loader_setinputs(tpp_cli_loader *tpp_restrict self,
                         int argc, char **argv) {
	tpp_lexer *const lexer = self->tcl_lexer;
	tpp_errno error;
	char const *filename;
again:
	if (!argc) {
#if !TPP_HAVE_TPP_W_NO_INPUT_FILES
		error = TPP_EOK;
#else /* !TPP_HAVE_TPP_W_NO_INPUT_FILES */
		error = tpp_lexer_cli_warnf(lexer, NULL, 0, TPP_W_NO_INPUT_FILES);
		if (!TPP_ISERR(error))
#endif /* TPP_HAVE_TPP_W_NO_INPUT_FILES */
		{
			/* Not treated as an error -> must still initialize the lexer's file */
			tpp_lexer_initfile_text_utf8(lexer, NULL, NULL, NULL, 0,
			                             TPP_LCINFO_INVALID,
			                             TPP_FILE_FLAGS_NORMAL);
		}
		return error;
	}

	/* Initialize the *first* file */
	filename = argv[--argc];
	error = tpp_cli_loader_open_input(self, filename, tpp_lexer_getfile(lexer));
	if (TPP_ISERR(error)) {
		if (error == TPP_ENOENT)
			goto again; /* Try our luck with the next file */
		return error;
	}

#if !TPP_HAVE_INCLUDE_STACK
#if TPP_HAVE_TPP_W_TOO_MANY_INPUT_FILES
	if (argc != 0) {
		error = tpp_lexer_cli_warnf(lexer, NULL, 0, TPP_W_TOO_MANY_INPUT_FILES);
		if (TPP_ISERR(error)) {
			tpp_lexer_finifile(lexer);
			return error;
		}
	}
#endif /* TPP_HAVE_TPP_W_TOO_MANY_INPUT_FILES */
#else /* !TPP_HAVE_INCLUDE_STACK */
	/* Enumerate remaining input files (which all need to be pushed). */
	while (argc) {
		tpp_file *const file = tpp_lexer_getfile(lexer);
		tpp_file *const prev_file = tpp_file_alloc();
		if tpp_unlikely(!prev_file) {
			error = TPP_ENOMEM;
			goto return_error;
		}
		tpp_file_move(prev_file, file);
		filename = argv[--argc];
		error = tpp_cli_loader_open_input(self, filename, file);
		if (TPP_ISERR(error)) {
			tpp_file_move(file, prev_file);
			tpp_file_free(prev_file);
			if (error != TPP_ENOENT) {
return_error:
				tpp_lexer_finifile(lexer);
				return error;
			}
			/* Try our luck with the next file */
		} else {
			file->tf_prev  = prev_file;
			file->tf_tprev = prev_file;
			error = tpp_lexer_callhook_file_pushed(lexer);
			if (TPP_ISERR(error))
				goto return_error;
		}
	}
#endif /* TPP_HAVE_INCLUDE_STACK */
	return TPP_EOK;
}
#endif /* TPP_HAVE_CLI_SETINPUTS */


/* Ensure that `self` is in a *normal* state (meaning that there aren't any remaining,
 * unterminated multi-argument parameters). If that is not the case, then a warning
 * `TPP_W_MISSING_CLI_ARGUMENT` is emitted on `tpp_cli_loader_getlexer(self)`
 *
 * Unlike the other CLI loader functions above, this one *MUST* be called
 * *AFTER* the lexer's initial input file has been initialized, as it may
 * need to push additional files onto the `#include`-stack.
 *
 * @return: TPP_EOK:       Success
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_cli_loader_flush(tpp_cli_loader *tpp_restrict self) {
	(void)self;

	/* Emit a warning if the CLI loader isn't in a neutral state */
#if TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT
	if (self->tcl_state != TPP_CLI_LOADER_STATE_NORMAL &&
	    self->tcl_state != TPP_CLI_LOADER_STATE_DDASH) {
		tpp_errno error = tpp_lexer_cli_warnf(self->tcl_lexer, NULL, 0,
		                                      TPP_W_MISSING_CLI_ARGUMENT);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT */

#if TPP_HAVE_CLI_DASH_INCLUDE
	/* Push -include files onto the lexer's #include-stack */
	{
		tpp_size i;
		for (i = 0; i < self->tcl_includec; ++i) {
			tpp_lexer_openfile_result *ofr = &self->tcl_includev[i];
			tpp_errno error = tpp_lexer_pushfile_ofr(self->tcl_lexer, ofr);
			if (TPP_ISERR(error)) {
				tpp_size remaming = (self->tcl_includec - 1) - i;
				(void)tpp_memmovedown(&self->tcl_includev[0],
				                      &self->tcl_includev[i],
				                      remaming * sizeof(tpp_lexer_openfile_result));
				self->tcl_includec = remaming;
				return error;
			}
		}
		self->tcl_includec = 0;
	}
#endif /* TPP_HAVE_CLI_DASH_INCLUDE */
	return TPP_EOK;
}

#if TPP_HAVE_CLI_HELP
#undef TPP_CLI_HELP1
#undef TPP_CLI_HELP2
#define TPP_CLI_HELP1(spelling, description) \
	spelling "\0\0" description "\0"
#if TPP_HAVE_CLI_HELP_ALL_SPELLINGS
#define TPP_CLI_HELP2(spelling1, spelling2, description) \
	spelling1 "\0" spelling2 "\0\0" description "\0"
#else /* TPP_HAVE_CLI_HELP_ALL_SPELLINGS */
#define TPP_CLI_HELP2(spelling1, spelling2, description) \
	TPP_CLI_HELP1(spelling1, description)
#endif /* !TPP_HAVE_CLI_HELP_ALL_SPELLINGS */

/* Returns supported CLI parameters, and human-readable information
 * for them in the form of:
 * >> "-Dmacro[=def]\0--define-macro macro[=def]\0\0"
 * >> "Define a macro with def, or 1 as its value.\0"
 * >> "-Umacro\0--undefine-macro macro\0\0"
 * >> "Undefined macro\0"
 * >> ...
 * >> "\0"
 *
 * Format (repeated):
 * >> <SPELLING1>[\0<SPELLING2>][\0<SPELLING3>][...]\0\0<DESCRIPTION>\0
 * >> \0
 *
 * The end is reached when "DESCRIPTION" is immediately followed
 * by another NUL-character, where there would otherwise be the
 * first character of the first spelling of the next CLI parameter.
 *
 * CLI parameter spellings are sorted such that the most *prominent*
 * spelling comes first. For the sake of keeping your `--help` readable,
 * I suggest you only print `SPELLING1` and have some kind of `--help all`
 * option (or similar) that will print *all* spellings.
 *
 * To enumerate available options, you can use code like this:
 * ```c
 * char const *iter = tpp_cli_loader_help;
 * while (*iter) {
 *     bool first = true;
 *     // Print spellings
 *     do {
 *         printf("%s%s", first ? "" : " ", iter);
 *         iter += tpp_strlen(iter) + 1;
 *         first = false;
 *     } while (*iter);
 *     ++iter;
 *     // Print description
 *     printf("\n\t\t\t%s\n", iter);
 *     iter += tpp_strlen(iter) + 1;
 * }
 * ``` */
TPP_CONST_IMPL char const tpp_cli_loader_help[] =
#if TPP_HAVE_CLI_DASH_DEFINE_MACRO
TPP_CLI_HELP2("-DMACRO[=DEF]", "--define-macro MACRO[=DEF]",
              "Define MACRO as DEF or 1")
#endif /* TPP_HAVE_CLI_DASH_DEFINE_MACRO */
#if TPP_HAVE_CLI_DASH_UNDEFINE_MACRO
TPP_CLI_HELP2("-UMACRO", "--undefine-macro MACRO",
              "Undefined MACRO")
#endif /* TPP_HAVE_CLI_DASH_UNDEFINE_MACRO */
#if TPP_HAVE_CLI_DASH_ASSERT
TPP_CLI_HELP2("-APREDICATE=ANSWER", "--assert=PREDICATE=ANSWER",
              "Define preprocessor assertion")
TPP_CLI_HELP2("-A-PREDICATE[=ANSWER]", "--assert=-PREDICATE[=ANSWER]",
              "Delete preprocessor assertion")
#endif /* TPP_HAVE_CLI_DASH_ASSERT */
#if TPP_HAVE_CLI_DASH_INCLUDE
TPP_CLI_HELP1("-include FILE",
              "Inject FILE at the start of main input")
#endif /* TPP_HAVE_CLI_DASH_INCLUDE */
#if TPP_HAVE_CLI_DASH_IMACROS
TPP_CLI_HELP2("-imacros FILE", "--imacros=FILE",
              "Include macro definitions from FILE")
#endif /* TPP_HAVE_CLI_DASH_IMACROS */
#if TPP_HAVE_CLI_DASH_UNDEF
TPP_CLI_HELP1("-undef",
              "Delete all predefined macro definitions")
#endif /* TPP_HAVE_CLI_DASH_UNDEF */
#if TPP_HAVE_CLI_DASH_FEXTENSION
TPP_CLI_HELP1("-f[no-]EXTENSION",
              "Enable or disable EXTENSION")
#endif /* TPP_HAVE_CLI_DASH_FEXTENSION */
#if TPP_HAVE_CLI_DASH_FPREPROCESSED
TPP_CLI_HELP1("-f[no-]preprocessed",
              "Turn off unnecessary features for already-preprocessed input")
#endif /* TPP_HAVE_CLI_DASH_FPREPROCESSED */
#if TPP_HAVE_CLI_DASH_FDIRECTIVES_ONLY
TPP_CLI_HELP1("-f[no-]directives-only",
              "Disable macro expansion")
#endif /* TPP_HAVE_CLI_DASH_FDIRECTIVES_ONLY */
#if TPP_HAVE_CLI_DASH_FDOLLARS_IN_IDENTIFIERS
TPP_CLI_HELP1("-f[no-]dollars-in-identifiers",
              "Allow $ to appear in identifiers")
#endif /* TPP_HAVE_CLI_DASH_FDOLLARS_IN_IDENTIFIERS */
#if TPP_HAVE_CLI_DASH_FMAX_INCLUDE_DEPTH
TPP_CLI_HELP1("-fmax-include-depth=COUNT",
              "Set max # of times a file may appear on #include-stack")
#endif /* TPP_HAVE_CLI_DASH_FMAX_INCLUDE_DEPTH */
#if TPP_HAVE_CLI_DASH_FTABSTOP
TPP_CLI_HELP1("-ftabstop=WIDTH",
              "Set WIDTH of \\t in columns")
#endif /* TPP_HAVE_CLI_DASH_FTABSTOP */
#if TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH
TPP_CLI_HELP1("-fsearch-include-path[=(user|system)]",
              "Find missing inputs in #include \"file\" (user) or #include <file> (system)")
#endif /* TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH */
#if TPP_HAVE_CLI_DASH_FTABSTOP
TPP_CLI_HELP2("-C", "--comments\0-CC\0--comments-in-macros",
              "Enable emission of COMMENT/SPACE/LF tokens")
#endif /* TPP_HAVE_CLI_DASH_FTABSTOP */
#if TPP_HAVE_CLI_DASH_TRADITIONAL
TPP_CLI_HELP2("-traditional", "--traditional\0-traditional-cpp\0--traditional-cpp",
              "Enable traditional macro expansion rules\n"
              "Turn off trigraphs and //-comments")
#endif /* TPP_HAVE_CLI_DASH_TRADITIONAL */
#if TPP_HAVE_CLI_DASH_TRIGRAPHS
TPP_CLI_HELP2("-trigraphs", "--trigraphs",
              "Enable trigraph sequences")
#endif /* TPP_HAVE_CLI_DASH_TRIGRAPHS */
#if TPP_HAVE_CLI_DASH_INCLUDE_BARRIER
TPP_CLI_HELP2("-I-", "--include-barrier",
              "Turn all preceding -IPATH to -iquote PATH\n"
              "Turn off relative filenames in #include \"file\"")
#endif /* TPP_HAVE_CLI_DASH_INCLUDE_BARRIER */
#if TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY
TPP_CLI_HELP2("-IPATH", "--include-directory=PATH",
              "Add PATH to list of #include <file> search directories")
#endif /* TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY */
#if TPP_HAVE_CLI_DASH_IQUOTE
TPP_CLI_HELP1("-iquote PATH",
              "Add PATH to list of #include \"file\" search directories")
#endif /* TPP_HAVE_CLI_DASH_IQUOTE */
#if TPP_HAVE_CLI_DASH_ISYSTEM
TPP_CLI_HELP1("-isystem PATH",
              "Like -IPATH, but treat files like #pragma GCC system_header")
#endif /* TPP_HAVE_CLI_DASH_ISYSTEM */
#if TPP_HAVE_CLI_DASH_IDIRAFTER
TPP_CLI_HELP2("-idirafter PATH", "--include-directory-after=PATH",
              "Search PATH in #include after all others have been searched")
#endif /* TPP_HAVE_CLI_DASH_IDIRAFTER */
#if TPP_HAVE_CLI_DASH_EMBED_DIR
TPP_CLI_HELP2("--embed-dir=PATH", "--embed-directory PATH",
              "Search PATH in #embed <file>")
#endif /* TPP_HAVE_CLI_DASH_EMBED_DIR */
#if TPP_HAVE_CLI_DASH_IPREFIX
TPP_CLI_HELP2("-iprefix PREFIX", "--include-prefix PREFIX",
              "Set PREFIX for -iwithprefix and -iwithprefixbefore")
#endif /* TPP_HAVE_CLI_DASH_IPREFIX */
#if TPP_HAVE_CLI_DASH_IWITHPREFIX
TPP_CLI_HELP2("-iwithprefix PATH", "--include-with-prefix PATH\0--include-with-prefix-after PATH",
              "Like -idirafter but combine PATH with PREFIX from -iprefix")
#endif /* TPP_HAVE_CLI_DASH_IWITHPREFIX */
#if TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE
TPP_CLI_HELP2("-iwithprefixbefore PATH", "--include-with-prefix-before PATH",
              "Like -I but combine PATH with PREFIX from -iprefix")
#endif /* TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE */
#if TPP_HAVE_CLI_DASH_ISYSROOT
TPP_CLI_HELP2("-isysroot PATH", "--sysroot PATH",
              "Use PATH to expand leading = or $SYSROOT in -I and friends")
#endif /* TPP_HAVE_CLI_DASH_ISYSROOT */
#if TPP_HAVE_CLI_DASH_NOSTDINC
TPP_CLI_HELP2("-nostdinc", "--no-standard-includes",
              "Don't search builtin system include paths")
#endif /* TPP_HAVE_CLI_DASH_NOSTDINC */
#if TPP_HAVE_CLI_DASH_REMAP
TPP_CLI_HELP1("-remap", "Enable processing of " TPP_CONFIG_INCLUDE_REMAP_FILENAME " files")
#endif /* TPP_HAVE_CLI_DASH_REMAP */
#if TPP_HAVE_CLI_DASH_WERROR
TPP_CLI_HELP1("-W[no-]error",
              "Treat all warnings as errors")
#endif /* TPP_HAVE_CLI_DASH_WERROR */
#if TPP_HAVE_CLI_DASH_WFATAL_ERROR
TPP_CLI_HELP1("-W[no-]fatal-errors",
              "Treat all errors as fatal")
#endif /* TPP_HAVE_CLI_DASH_WFATAL_ERROR */
#if TPP_HAVE_CLI_DASH_FMAX_ERRORS
TPP_CLI_HELP1("-fmax-errors=COUNT",
              "Set number of errors before treating as fatal")
#endif /* TPP_HAVE_CLI_DASH_FMAX_ERRORS */
#if TPP_HAVE_CLI_DASH_WWARNING
TPP_CLI_HELP1("-W[no-]WARNING",
              "Turn WARNING on or off")
#endif /* TPP_HAVE_CLI_DASH_WWARNING */
#if TPP_HAVE_CLI_DASH_WERROR_WARNING
TPP_CLI_HELP1("-Werror=WARNING",
              "Treat WARNING as error")
#endif /* TPP_HAVE_CLI_DASH_WERROR_WARNING */
"";
#undef TPP_CLI_HELP1
#undef TPP_CLI_HELP2
#endif /* TPP_HAVE_CLI_HELP */

#endif /* TPP_HAVE_CLI */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CLI_C */
