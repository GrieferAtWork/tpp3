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

/* Define a function `tpp_cli_loader_add_include()` */
#undef TPP_HAVE_CLI_ADD_INCLUDE
#define TPP_HAVE_CLI_ADD_INCLUDE       \
	(TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY || \
	 TPP_HAVE_CLI_DASH_IQUOTE ||            \
	 TPP_HAVE_CLI_DASH_ISYSTEM ||           \
	 TPP_HAVE_CLI_DASH_IDIRAFTER)

/* Define a function `tpp_lexer_cli_warnf()` */
#undef TPP_HAVE_LEXER_CLI_WARN
#define TPP_HAVE_LEXER_CLI_WARN                                       \
	((TPP_HAVE_CLI_OPEN_OFR && TPP_HAVE_TPP_W_NO_SUCH_FILE) || \
	 (TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT))

/* Define a function `tpp_simple_atoz()` */
#undef TPP_HAVE_SIMPLE_ATOZ
#define TPP_HAVE_SIMPLE_ATOZ \
	(TPP_HAVE_CLI_DASH_FMAX_INCLUDE_DEPTH)

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
	{"TRADITIONAL_MACROS", "enable"},
	{"TOK_CXX_COMMENT", "disable"},
	{"INCLUDE_RELATIVE_TO_CURRENT_FILE", "disable"},
	{"INCLUDE_SYSTEM_INCLUDE_PATH", "disable"},
	{"WERROR", "set"},
}) {
	local extraArgs = what == "set" ? ", v" : "";
	print("#if TPP_CONF_IS_EXT(TPP_HAVE_", option, ")");
	print("#define tpp_lexer_", what, "_", option, "(self", extraArgs, ") tpp_lexer_", what, "extension(self, TPP_EXT_", option, extraArgs, ")");
	print("#elif TPP_CONF_IS_FEAT(TPP_HAVE_", option, ")");
	print("#define tpp_lexer_", what, "_", option, "(self", extraArgs, ") (tpp_lexer_", what, "feature(self, TPP_FEAT_", option, extraArgs, "), TPP_EOK)");
	print("#else /" "* ... *" "/");
	print("#define tpp_lexer_", what, "_", option, "(self", extraArgs, ") TPP_EOK");
	print("#endif /" "* !... *" "/");
}
]]]*/
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_PREDEFINED_MACROS)
#define tpp_lexer_disable_CPP_PREDEFINED_MACROS(self) tpp_lexer_disableextension(self, TPP_EXT_CPP_PREDEFINED_MACROS)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_CPP_PREDEFINED_MACROS)
#define tpp_lexer_disable_CPP_PREDEFINED_MACROS(self) (tpp_lexer_disablefeature(self, TPP_FEAT_CPP_PREDEFINED_MACROS), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_CPP_PREDEFINED_MACROS(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_MACROS)
#define tpp_lexer_set_CPP_MACROS(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_MACROS, v)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_CPP_MACROS)
#define tpp_lexer_set_CPP_MACROS(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_MACROS, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_MACROS(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_TRIGRAPHS)
#define tpp_lexer_set_TRIGRAPHS(self, v) tpp_lexer_setextension(self, TPP_EXT_TRIGRAPHS, v)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_TRIGRAPHS)
#define tpp_lexer_set_TRIGRAPHS(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_TRIGRAPHS, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_TRIGRAPHS(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_TRIGRAPHS)
#define tpp_lexer_disable_TRIGRAPHS(self) tpp_lexer_disableextension(self, TPP_EXT_TRIGRAPHS)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_TRIGRAPHS)
#define tpp_lexer_disable_TRIGRAPHS(self) (tpp_lexer_disablefeature(self, TPP_FEAT_TRIGRAPHS), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_TRIGRAPHS(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_TRIGRAPHS)
#define tpp_lexer_enable_TRIGRAPHS(self) tpp_lexer_enableextension(self, TPP_EXT_TRIGRAPHS)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_TRIGRAPHS)
#define tpp_lexer_enable_TRIGRAPHS(self) (tpp_lexer_enablefeature(self, TPP_FEAT_TRIGRAPHS), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_TRIGRAPHS(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_BSE)
#define tpp_lexer_set_BSE(self, v) tpp_lexer_setextension(self, TPP_EXT_BSE, v)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_BSE)
#define tpp_lexer_set_BSE(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_BSE, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_BSE(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_INCLUDE)
#define tpp_lexer_set_CPP_INCLUDE(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_INCLUDE, v)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE)
#define tpp_lexer_set_CPP_INCLUDE(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_INCLUDE, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_INCLUDE(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_INCLUDE_NEXT)
#define tpp_lexer_set_CPP_INCLUDE_NEXT(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_INCLUDE_NEXT, v)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE_NEXT)
#define tpp_lexer_set_CPP_INCLUDE_NEXT(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_INCLUDE_NEXT, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_INCLUDE_NEXT(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_IMPORT)
#define tpp_lexer_set_CPP_IMPORT(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_IMPORT, v)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IMPORT)
#define tpp_lexer_set_CPP_IMPORT(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_IMPORT, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_IMPORT(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_IF_ELSE_ENDIF)
#define tpp_lexer_set_CPP_IF_ELSE_ENDIF(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_IF_ELSE_ENDIF, v)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IF_ELSE_ENDIF)
#define tpp_lexer_set_CPP_IF_ELSE_ENDIF(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_IF_ELSE_ENDIF, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_IF_ELSE_ENDIF(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_DEFINE)
#define tpp_lexer_set_CPP_DEFINE(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_DEFINE, v)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DEFINE)
#define tpp_lexer_set_CPP_DEFINE(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_DEFINE, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_DEFINE(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_ASSERT)
#define tpp_lexer_set_CPP_ASSERT(self, v) tpp_lexer_setextension(self, TPP_EXT_CPP_ASSERT, v)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ASSERT)
#define tpp_lexer_set_CPP_ASSERT(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_CPP_ASSERT, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_CPP_ASSERT(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_TOK_DOLLAR)
#define tpp_lexer_set_TOK_DOLLAR(self, v) tpp_lexer_setextension(self, TPP_EXT_TOK_DOLLAR, v)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOLLAR)
#define tpp_lexer_set_TOK_DOLLAR(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_TOK_DOLLAR, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_TOK_DOLLAR(self, v) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_TOK_COMMENT)
#define tpp_lexer_enable_TOK_COMMENT(self) tpp_lexer_enableextension(self, TPP_EXT_TOK_COMMENT)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COMMENT)
#define tpp_lexer_enable_TOK_COMMENT(self) (tpp_lexer_enablefeature(self, TPP_FEAT_TOK_COMMENT), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_TOK_COMMENT(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_TRADITIONAL_MACROS)
#define tpp_lexer_enable_TRADITIONAL_MACROS(self) tpp_lexer_enableextension(self, TPP_EXT_TRADITIONAL_MACROS)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_TRADITIONAL_MACROS)
#define tpp_lexer_enable_TRADITIONAL_MACROS(self) (tpp_lexer_enablefeature(self, TPP_FEAT_TRADITIONAL_MACROS), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_TRADITIONAL_MACROS(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_TOK_CXX_COMMENT)
#define tpp_lexer_disable_TOK_CXX_COMMENT(self) tpp_lexer_disableextension(self, TPP_EXT_TOK_CXX_COMMENT)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_COMMENT)
#define tpp_lexer_disable_TOK_CXX_COMMENT(self) (tpp_lexer_disablefeature(self, TPP_FEAT_TOK_CXX_COMMENT), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_TOK_CXX_COMMENT(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_INCLUDE_RELATIVE_TO_CURRENT_FILE)
#define tpp_lexer_disable_INCLUDE_RELATIVE_TO_CURRENT_FILE(self) tpp_lexer_disableextension(self, TPP_EXT_INCLUDE_RELATIVE_TO_CURRENT_FILE)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_INCLUDE_RELATIVE_TO_CURRENT_FILE)
#define tpp_lexer_disable_INCLUDE_RELATIVE_TO_CURRENT_FILE(self) (tpp_lexer_disablefeature(self, TPP_FEAT_INCLUDE_RELATIVE_TO_CURRENT_FILE), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_INCLUDE_RELATIVE_TO_CURRENT_FILE(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_INCLUDE_SYSTEM_INCLUDE_PATH)
#define tpp_lexer_disable_INCLUDE_SYSTEM_INCLUDE_PATH(self) tpp_lexer_disableextension(self, TPP_EXT_INCLUDE_SYSTEM_INCLUDE_PATH)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_INCLUDE_SYSTEM_INCLUDE_PATH)
#define tpp_lexer_disable_INCLUDE_SYSTEM_INCLUDE_PATH(self) (tpp_lexer_disablefeature(self, TPP_FEAT_INCLUDE_SYSTEM_INCLUDE_PATH), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_INCLUDE_SYSTEM_INCLUDE_PATH(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_IS_EXT(TPP_HAVE_WERROR)
#define tpp_lexer_set_WERROR(self, v) tpp_lexer_setextension(self, TPP_EXT_WERROR, v)
#elif TPP_CONF_IS_FEAT(TPP_HAVE_WERROR)
#define tpp_lexer_set_WERROR(self, v) (tpp_lexer_setfeature(self, TPP_FEAT_WERROR, v), TPP_EOK)
#else /* ... */
#define tpp_lexer_set_WERROR(self, v) TPP_EOK
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
#if 0
	if (*path == '=' || tpp_memcmp(path, "$SYSROOT", 8 * sizeof(char)) == 0) {
		/* XXX: Replace prefix with "system root" */
	}
#endif
	return tpp_lexer_includes_addbykind(self->tcl_lexer, kind, path, TPP_SIZE_MAX);
}
#endif /* TPP_HAVE_CLI_ADD_INCLUDE */



/* Feed an argument to the loader how exactly the argument is parsed
 * depends on the loader's current state, but sufficed to say: in its
 * default/initial state, `arg` should be something like `-Dfoo=bar`
 * or `-I/usr/include`, etc.
 *
 * @return: TPP_EOK:        Success (argument was parsed + consumed)
 * @return: TPP_ENOENT:     SOFT_ERROR: Argument could not be understood (but no
 *                          warning was emitted). You must either handle it yourself
 *                          by treating it as an argument for *your* compiler's
 *                          CLI, or as an input file for it, or emit a warning
 *                          informing the user that their CLI argument was not
 *                          understood.
 * @return: TPP_ENOMEM:     HARD_ERROR: Out of memory
 * @return: TPP_EIO:        HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR:  HARD_ERROR: A lexer error was thrown
 * @return: TPP_EWARNPRINT: HARD_ERROR: An error happened within a warning printer */
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
#if TPP_HAVE_CLI_DASH_INCLUDE || TPP_HAVE_CLI_DASH_INCLUDE_BARRIER || TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY || TPP_HAVE_CLI_DASH_IDIRAFTER
				if (tpp_streq(arg, "nclude")) { /* --include */
					arg += (sizeof("nclude") - sizeof(char));
#if TPP_HAVE_CLI_DASH_INCLUDE
					if (*arg == '=')
						return tpp_cli_loader_parse_include(self, arg + 1);
					if (*arg == '\0') {
						self->tcl_state = TPP_CLI_LOADER_STATE_INCLUDE;
						return TPP_EOK;
					}
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
						if (*arg == '=')
							return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_SYSTEM, arg + 1);
						if (*arg == '\0') {
							self->tcl_state = TPP_CLI_LOADER_STATE_INCLUDE_DIRECTORY;
							return TPP_EOK;
						}
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
						}
#endif /* TPP_HAVE_CLI_DASH_IDIRAFTER */
					} else
#endif /* TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY || TPP_HAVE_CLI_DASH_IDIRAFTER */
					{
					}
				} else
#endif /* TPP_HAVE_CLI_DASH_INCLUDE || TPP_HAVE_CLI_DASH_INCLUDE_BARRIER || TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY || TPP_HAVE_CLI_DASH_IDIRAFTER */
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
					return tpp_lexer_enable_TOK_COMMENT(self->tcl_lexer);
				} else
#endif /* TPP_HAVE_CLI_DASH_COMMENTS */
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
		case 'f': {
#if (TPP_HAVE_CLI_DASH_FPREPROCESSED ||           \
     TPP_HAVE_CLI_DASH_FDIRECTIVES_ONLY ||        \
     TPP_HAVE_CLI_DASH_FDOLLARS_IN_IDENTIFIERS || \
     TPP_HAVE_CLI_DASH_FMAX_INCLUDE_DEPTH ||      \
     TPP_HAVE_CLI_DASH_FTABSTOP ||                \
     TPP_HAVE_CLI_DASH_FEXTENSION)
			bool no = false;
			if (tpp_streq(arg, "no-")) {
				arg += 3;
				no = true;
			}
#endif /* ... */

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
			if (tpp_streq(arg, "max-include-depth=") && !no) { /* -fmax-include-depth=... */
				tpp_size new_limit = tpp_simple_atoz(arg + 19);
				tpp_lexer_setinclusionlimit(self->tcl_lexer, new_limit);
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_FMAX_INCLUDE_DEPTH */
#if TPP_HAVE_CLI_DASH_FTABSTOP
			if (tpp_streq(arg, "tabstop=") && !no) { /* -ftabstop=... */
				tpp_size new_stop = tpp_simple_atoz(arg + 8);
				tpp_settabsize(new_stop);
				return TPP_EOK;
			} else
#endif /* TPP_HAVE_CLI_DASH_FTABSTOP */
			{
#if TPP_HAVE_CLI_DASH_FEXTENSION
				/* Fallback: configure an extension */
				tpp_extension_id extension = tpp_extension_byname(arg);
				if (extension != TPP_EXT_COUNT)
					return tpp_lexer_setextension(self->tcl_lexer, extension, !no);
#endif /* TPP_HAVE_CLI_DASH_FEXTENSION */
			}
		}	break;
/************************************************************************/



/************************************************************************/
		case 'W': {
#if TPP_HAVE_CLI_DASH_WERROR || TPP_HAVE_CLI_DASH_WWARNING
			bool no = false;
			if (tpp_streq(arg, "no-")) {
				arg += 3;
				no = true;
			}
#endif /* ... */

#if TPP_HAVE_CLI_DASH_WERROR
			if (tpp_streq(arg, "error\0")) {
				return tpp_lexer_set_WERROR(self->tcl_lexer, !no);
			} else
#endif /* TPP_HAVE_CLI_DASH_WERROR */
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
		}	break;
/************************************************************************/



/************************************************************************/
		case 'C':
#if TPP_HAVE_CLI_DASH_COMMENTS
			if (*arg == '\0' || tpp_streq(arg, "C\0")) { /* -C -CC */
				return tpp_lexer_enable_TOK_COMMENT(self->tcl_lexer);
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
		return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_SYSTEM, arg);
#endif /* TPP_HAVE_CLI_DASH_ISYSTEM */

#if TPP_HAVE_CLI_DASH_IDIRAFTER
	case TPP_CLI_LOADER_STATE_IDIRAFTER:
		self->tcl_state = TPP_CLI_LOADER_STATE_NORMAL;
		return tpp_cli_loader_parse_addinclude(self, TPP_INCLUDE_PATH_KIND_AFTER, arg);
#endif /* TPP_HAVE_CLI_DASH_IDIRAFTER */

	default: tpp_unreachable();
	}
	return TPP_ENOENT;
#undef tpp_streq
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
 * @return: TPP_EOK:        Success (`*p_argc` and `*p_argv` were updated such that
 *                          they contain all unrecognized arguments, as well as all
 *                          input files for your compiler).
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O Error
 * @return: TPP_ELEXERROR:  A lexer error was thrown
 * @return: TPP_EWARNPRINT: An error happened within a warning printer */
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
			tpp_memmovedown(&argv[0], &argv[1],
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
					tpp_memmovedown(&argv[0], &argv[1], total_count_minus_1 * sizeof(char *));
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

/* Ensure that `self` is in a "normal" state (meaning that there aren't any remaining,
 * unterminated multi-argument parameters). If that is not the case, then a warning
 * `TPP_W_MISSING_CLI_ARGUMENT` is emitted on `tpp_cli_loader_getlexer(self)`
 *
 * Unlike the other CLI loader functions above, this one *MUST* be called
 * *AFTER* the lexer's initial input file has been initialized, as it may
 * need to push additional files onto the #include-stack.
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O Error
 * @return: TPP_ELEXERROR:  A lexer error was thrown
 * @return: TPP_EWARNPRINT: An error happened within a warning printer */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_cli_loader_flush(tpp_cli_loader *tpp_restrict self) {
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

	(void)self;
#if TPP_HAVE_CLI_DASH_INCLUDE
	/* Push -include files onto the lexer's #include-stack */
	{
		tpp_size i;
		for (i = 0; i < self->tcl_includec; ++i) {
			tpp_lexer_openfile_result *ofr = &self->tcl_includev[i];
			tpp_errno error = tpp_lexer_pushfile_ofr(self->tcl_lexer, ofr);
			if (TPP_ISERR(error)) {
				tpp_size remaming = (self->tcl_includec - 1) - i;
				tpp_memmovedown(&self->tcl_includev[0],
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
#endif /* TPP_HAVE_CLI */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CLI_C */
