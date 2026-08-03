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
#ifndef GUARD_TPP_OPTIONAL_EMITTER_EMITTER_CLI_C
#define GUARD_TPP_OPTIONAL_EMITTER_EMITTER_CLI_C 1
#define TPP_BUILDING_OPTIONAL 1
#define TPP_EMITTER_BUILDING 1

#include "api.h"

#include "config.h"
#include "emitter-cli.h"
#include "emitter.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_EMITTER_HAVE_CLI

/*[[[deemon
for (local option, what: {
	{"TOK_SPACE", "enable"},
	{"TOK_LF", "enable"},
	{"TOK_SPACE", "disable"},
	{"TOK_LF", "disable"},
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
#if TPP_CONF_ISEXT(TPP_HAVE_TOK_SPACE)
#define tpp_lexer_disable_TOK_SPACE(self) tpp_lexer_disableextension(self, TPP_EXT_TOK_SPACE)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_SPACE)
#define tpp_lexer_disable_TOK_SPACE(self) (tpp_lexer_disablefeature(self, TPP_FEAT_TOK_SPACE), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_TOK_SPACE(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TOK_LF)
#define tpp_lexer_disable_TOK_LF(self) tpp_lexer_disableextension(self, TPP_EXT_TOK_LF)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_LF)
#define tpp_lexer_disable_TOK_LF(self) (tpp_lexer_disablefeature(self, TPP_FEAT_TOK_LF), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_TOK_LF(self) TPP_EOK
#endif /* !... */
/*[[[end]]]*/


/*[[[deemon
for (local option, what: {
	{"NORMALIZE_SPACE", "set"},
	{"NORMALIZE_LF", "set"},
	{"NORMALIZE_C_STRING", "set"},
	{"NORMALIZE_KEYWORDS", "set"},
	{"NORMALIZE_BSE", "set"},
	{"NORMALIZE_TRIGRAPHS", "set"},
	{"NORMALIZE_DIGRAPHS", "set"},
	{"NOLINE", "enable"},
	{"NOLINE", "disable"},
}) {
	local extraArgs = what == "set" ? ", v" : "";
	print("#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_", option, ")");
	print("#define tpp_emitter_", what, "_", option, "(self", extraArgs, ") tpp_emitter_", what, "feature(self, TPP_EMITTER_FEAT_", option, extraArgs, ")");
	print("#else /" "* ... *" "/");
	print("#define tpp_emitter_", what, "_", option, "(self", extraArgs, ") (void)0");
	print("#endif /" "* !... *" "/");
}
]]]*/
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
#define tpp_emitter_set_NORMALIZE_SPACE(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_SPACE, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_SPACE(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF)
#define tpp_emitter_set_NORMALIZE_LF(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_LF, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_LF(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
#define tpp_emitter_set_NORMALIZE_C_STRING(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_C_STRING, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_C_STRING(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
#define tpp_emitter_set_NORMALIZE_KEYWORDS(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_KEYWORDS, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_KEYWORDS(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE)
#define tpp_emitter_set_NORMALIZE_BSE(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_BSE, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_BSE(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
#define tpp_emitter_set_NORMALIZE_TRIGRAPHS(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_TRIGRAPHS, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_TRIGRAPHS(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
#define tpp_emitter_set_NORMALIZE_DIGRAPHS(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_DIGRAPHS, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_DIGRAPHS(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE)
#define tpp_emitter_enable_NOLINE(self) tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_NOLINE)
#else /* ... */
#define tpp_emitter_enable_NOLINE(self) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE)
#define tpp_emitter_disable_NOLINE(self) tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_NOLINE)
#else /* ... */
#define tpp_emitter_disable_NOLINE(self) (void)0
#endif /* !... */
/*[[[end]]]*/


/* Define a function `tpp_emitter_cli_warnf()` */
#undef TPP_HAVE_EMITTER_CLI_WARN
#define TPP_HAVE_EMITTER_CLI_WARN \
	(TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT)

#if TPP_HAVE_EMITTER_CLI_WARN
static TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_warnf(tpp_emitter *tpp_restrict self, tpp_char const *token_start,
                      tpp_size token_size, tpp_warning_id id, ...) {
	/* XXX: Use of `TPP_INTERNAL` here isn't allowed -- come up with
	 *      APIs to allow users to do this stuff without needing to
	 *      access TPP internal! */
	tpp_lexer *const lexer = tpp_emitter_getlexer(self);
	tpp_errno result;
	va_list args;
	union TPP_INTERNAL(tpp_lexer_core) saved_core = lexer->TPP_INTERNAL(tl_core);
	tpp_file *const file = tpp_lexer_getfile(lexer);
	tpp_file_init_text_utf8(file, TPP_CONFIG_CLI_FILENAME,
	                        NULL, token_start, token_size,
	                        TPP_LCINFO_INVALID, TPP_FILE_FLAGS_NORMAL);
	file->TPP_INTERNAL(tf_tpos) = token_start;
	file->TPP_INTERNAL(tf_pos)  = token_start + token_size;
	va_start(args, id);
	result = tpp_lexer_vwarnf(lexer, id, args);
	va_end(args);
	lexer->TPP_INTERNAL(tl_core) = saved_core;
	return result;
}
#endif /* TPP_HAVE_EMITTER_CLI_WARN */


#if TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_no_line_commands(tpp_emitter_cli_loader *tpp_restrict self) {
	tpp_errno result;
	(void)self;
	tpp_emitter_enable_NOLINE(self->tcl_emitter);

	/* Enable emission of SPACE/LF tokens */
	result = tpp_lexer_enable_TOK_SPACE(tpp_emitter_getlexer(self->tcl_emitter));
	if (!TPP_ISERR(result))
		result = tpp_lexer_enable_TOK_LF(tpp_emitter_getlexer(self->tcl_emitter));
	return result;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS */


#if TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_trace_includes(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
	tpp_emitter_enable_trace_includes(self->tcl_emitter);
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_NOLINE) */
	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES */


#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_M
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_M(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Set flag to dump definitions of builtin/predefined macros later. */
	self->tcl_flags |= _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M;

	/* Turn on re-emission of additional macros */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
	tpp_emitter_enable_reemit_macro_definitions(self->tcl_emitter);
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

	/* Set emitter mode to "TPP_EMITTER_MODE_DISPOSE" */
#if TPP_EMITTER_HAVE_MODE_DISPOSE
	tpp_emitter_setmode(self->tcl_emitter, TPP_EMITTER_MODE_DISPOSE);
#endif /* TPP_EMITTER_HAVE_MODE_DISPOSE */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_M */


#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_D
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_D(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Set flag to dump definitions of builtin/predefined macros later. */
	self->tcl_flags |= _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M;

	/* Turn on re-emission of additional macros */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
	tpp_emitter_enable_reemit_macro_definitions(self->tcl_emitter);
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_D */


#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_N
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_N(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Set flag to dump definitions of builtin/predefined macros later. */
	self->tcl_flags |= _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M;

	/* Turn on re-emission of additional macros */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
	tpp_emitter_enable_reemit_macro_definitions(self->tcl_emitter);
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

	/* *only* print the name of macros in `#define` directives */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
	tpp_emitter_enablefeature(self->tcl_emitter, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_NAME_ONLY);
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY) */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_N */


#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_I
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_I(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Turn on re-emission of #include-directives */
#if TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES
	tpp_emitter_enable_reemit_include_directives(self->tcl_emitter);
#endif /* TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_I */


#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_U
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_U(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Turn on re-emission of #include-directives */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
	tpp_emitter_enable_reemit_macro_definitions_lazy(self->tcl_emitter);
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_U */


#undef TPP_EMITTER_HAVE_CLI_DASH_DUMP
#define TPP_EMITTER_HAVE_CLI_DASH_DUMP   \
	(TPP_EMITTER_HAVE_CLI_DASH_DUMP_M || \
	 TPP_EMITTER_HAVE_CLI_DASH_DUMP_D || \
	 TPP_EMITTER_HAVE_CLI_DASH_DUMP_N || \
	 TPP_EMITTER_HAVE_CLI_DASH_DUMP_I || \
	 TPP_EMITTER_HAVE_CLI_DASH_DUMP_U)
#undef TPP_EMITTER_HAVE_CLI_DASH_MODE
#define TPP_EMITTER_HAVE_CLI_DASH_MODE         \
	(TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT ||    \
	 TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE || \
	 TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET || \
	 TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED)


enum {
	_TPP_EMITTER_CLI_LOADER_STATE_FIRST_INTERNAL = TPP_EMITTER_CLI_LOADER_STATE_DDASH,
#if TPP_EMITTER_HAVE_CLI_DASH_DUMP
	TPP_EMITTER_CLI_LOADER_STATE_DUMP,
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP */
};

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump(tpp_emitter_cli_loader *tpp_restrict self,
                            tpp_char what) {
	switch (what) {

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_M
	case 'M':
		return tpp_emitter_cli_enable_dump_M(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_M */

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_D
	case 'D':
		return tpp_emitter_cli_enable_dump_D(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_D */

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_N
	case 'N':
		return tpp_emitter_cli_enable_dump_N(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_N */

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_I
	case 'I':
		return tpp_emitter_cli_enable_dump_I(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_I */

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_U
	case 'U':
		return tpp_emitter_cli_enable_dump_U(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_U */

	default: break;
	}
	return TPP_ENOENT;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dumps(tpp_emitter_cli_loader *tpp_restrict self,
                             char const *whats) {
	tpp_errno result = TPP_EOK;
	for (;;) {
		char what = *whats++;
		if (what == '\0')
			break;
		result = tpp_emitter_cli_enable_dump(self, what);
		if (TPP_ISERR(result))
			break;
	}
	return result;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP */


/* Feed an argument to the loader. How exactly the argument is parsed
 * depends on the loader's current state, but sufficed to say: in its
 * default/initial state, `arg` is a CLI argument as you'd expect.
 *
 * WARNING: When you call this function, you must guaranty that `arg` remains
 *          valid, allocated, and unaltered until `tpp_emitter_cli_loader_fini()` is
 *          called.
 *
 * @return: TPP_EOK:        Success (argument was parsed + consumed)
 * @return: TPP_ENOENT:     SOFT_ERROR: Argument could not be understood (but no
 *                          warning was emitted). You must either handle it yourself
 *                          by treating it as an argument for *your* compiler's
 *                          CLI, or as an input file for the emitter, or emit a
 *                          warning informing the user that their CLI argument
 *                          was not understood. You should also probably try to
 *                          pass it to `tpp_cli_loader_parsearg()`.
 * @return: TPP_ENOMEM:     HARD_ERROR: Out of memory
 * @return: TPP_EIO:        HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR:  HARD_ERROR: A emitter error was thrown
 * @return: TPP_EWARNPRINT: HARD_ERROR: An error happened within a warning printer */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_cli_loader_parsearg(tpp_emitter_cli_loader *tpp_restrict self, char const *arg) {
#define tpp_streq(at, CONSTstr) \
	(tpp_memcmp(at, CONSTstr, sizeof(CONSTstr) - sizeof(char)) == 0)
	switch (self->tcl_state) {

	case TPP_EMITTER_CLI_LOADER_STATE_NORMAL: {
		if (*arg++ != '-')
			break; /* All arguments must start with at least 1 "-" */
		switch (*arg++) {

		case '-':
			/* --foo argument */
			switch (*arg++) {

			case '\0':
				self->tcl_state = TPP_EMITTER_CLI_LOADER_STATE_DDASH; /* -- */
				return TPP_EOK;

			case 'n':
#if TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS
				if (tpp_streq(arg, "o-line-commands\0")) {
					return tpp_emitter_cli_enable_no_line_commands(self);
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS */
				{
				}
				break;

			case 't':
#if TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES
				if (tpp_streq(arg, "race-includes\0")) {
					return tpp_emitter_cli_enable_trace_includes(self);
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES */
				{
				}
				break;

			case 'd':
#if TPP_EMITTER_HAVE_CLI_DASH_DUMP
				if (tpp_streq(arg, "ump")) { /* --dump= */
					arg += (sizeof("ump") - sizeof(char));
					if (*arg == '=') {
						return tpp_emitter_cli_enable_dumps(self, arg + 1);
					} else if (*arg == '\0') {
						self->tcl_state = TPP_EMITTER_CLI_LOADER_STATE_DUMP;
						return TPP_EOK;
					}
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP */
				{
				}
				break;

			case 'm':
#if TPP_EMITTER_HAVE_CLI_DASH_MODE
				if (tpp_streq(arg, "ode=")) {
					arg += (sizeof("ode=") - sizeof(char));
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT
					if (tpp_streq(arg, "emit\0")) {
						tpp_errno error;
						tpp_emitter_setmode(self->tcl_emitter, TPP_EMITTER_MODE_EMIT);
						tpp_emitter_disable_NOLINE(self->tcl_emitter);
						/* Turn off emission of `SPACE` and `LF` tokens */
						error = tpp_lexer_disable_TOK_SPACE(tpp_emitter_getlexer(self->tcl_emitter));
						if (!TPP_ISERR(error))
							error = tpp_lexer_disable_TOK_LF(tpp_emitter_getlexer(self->tcl_emitter));
						return error;
					} else
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT */
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE
					if (tpp_streq(arg, "dispose\0")) {
						tpp_emitter_setmode(self->tcl_emitter, TPP_EMITTER_MODE_DISPOSE);
						return TPP_EOK;
					} else
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE */
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET
					if (tpp_streq(arg, "bracket\0")) {
						tpp_errno error;
						tpp_emitter_setmode(self->tcl_emitter, TPP_EMITTER_MODE_BRACKET);
						error = tpp_lexer_enable_TOK_SPACE(tpp_emitter_getlexer(self->tcl_emitter));
						if (!TPP_ISERR(error))
							error = tpp_lexer_enable_TOK_LF(tpp_emitter_getlexer(self->tcl_emitter));
						return error;
					} else
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET */
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED
					if (tpp_streq(arg, "typed\0")) {
						tpp_errno error;
						tpp_emitter_setmode(self->tcl_emitter, TPP_EMITTER_MODE_TYPED);
						error = tpp_lexer_enable_TOK_SPACE(tpp_emitter_getlexer(self->tcl_emitter));
						if (!TPP_ISERR(error))
							error = tpp_lexer_enable_TOK_LF(tpp_emitter_getlexer(self->tcl_emitter));
						return error;
					} else
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED */
					{
					}
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_MODE */
				{
				}
				break;

			default: break;
			}
			break;

		case 'P':
#if TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS
			if (*arg == '\0')
				return tpp_emitter_cli_enable_no_line_commands(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS */
			break;

		case 'H':
#if TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES
			if (*arg == '\0')
				return tpp_emitter_cli_enable_trace_includes(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES */
			break;

		case 'd':
#if TPP_EMITTER_HAVE_CLI_DASH_DUMP
			if (*arg)
				return tpp_emitter_cli_enable_dumps(self, arg);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP */
			break;

		case 'f': {
#if (TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN || \
     TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA)
			bool no = false;
			if (tpp_streq(arg, "no-"))
				no = true, arg += 3;
#endif /* ... */

#if TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN
			if (tpp_streq(arg, "relaxed-macro-column\0")) {
				tpp_emitter_setfeature(self->tcl_emitter, TPP_EMITTER_FEAT_RELAXED_MACRO_COLUMN, !no);
				return TPP_EOK;
			} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN */
#if TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA
			if (tpp_streq(arg, "reemit-unknown-pragma\0")) {
				tpp_emitter_set_reemit_unknown_pragma(self->tcl_emitter, !no);
				return TPP_EOK;
			} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA */
#if (TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE ||     \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF ||        \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS ||   \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS ||  \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE ||       \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS || \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS)
			if (tpp_streq(arg, "normalize-")) {
				arg += (sizeof("normalize-") - sizeof(char));
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE
				if (tpp_streq(arg, "space\0")) {
					tpp_emitter_set_NORMALIZE_SPACE(self->tcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF
				if (tpp_streq(arg, "lf\0")) {
					tpp_emitter_set_NORMALIZE_LF(self->tcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS
				if (tpp_streq(arg, "strings\0")) {
					tpp_emitter_set_NORMALIZE_C_STRING(self->tcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS
				if (tpp_streq(arg, "keywords\0")) {
					tpp_emitter_set_NORMALIZE_KEYWORDS(self->tcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE
				if (tpp_streq(arg, "bse\0")) {
					tpp_emitter_set_NORMALIZE_BSE(self->tcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS
				if (tpp_streq(arg, "trigraphs\0")) {
					tpp_emitter_set_NORMALIZE_TRIGRAPHS(self->tcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS
				if (tpp_streq(arg, "digraphs\0")) {
					tpp_emitter_set_NORMALIZE_DIGRAPHS(self->tcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS */
				{
				}
		} else
#endif /* ... */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE
			if (tpp_streq(arg, "normalize\0")) {
				tpp_emitter_set_NORMALIZE_SPACE(self->tcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_LF(self->tcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_C_STRING(self->tcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_KEYWORDS(self->tcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_BSE(self->tcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_TRIGRAPHS(self->tcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_DIGRAPHS(self->tcl_emitter, !no);
				return TPP_EOK;
			} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE */
			{
			}
		}	break;

		default: break;
		}
	}	break;

	case TPP_EMITTER_CLI_LOADER_STATE_DDASH:
		break; /* Don't accept any more arguments after having encountered a "--" arguments */

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP
	case TPP_EMITTER_CLI_LOADER_STATE_DUMP:
		self->tcl_state = TPP_EMITTER_CLI_LOADER_STATE_NORMAL;
		return tpp_emitter_cli_enable_dumps(self, arg);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP */

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
 * @return: TPP_ENOMEM:     HARD_ERROR: Out of memory
 * @return: TPP_EIO:        HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR:  HARD_ERROR: A emitter error was thrown
 * @return: TPP_EWARNPRINT: HARD_ERROR: An error happened within a warning printer */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_cli_loader_parseflag(tpp_emitter_cli_loader *tpp_restrict self, char const **p_arg) {
	char const *arg = *p_arg;
	char flag = *arg++;
	(void)self;
	(void)flag;
	(void)arg;
	switch (flag) {

#if TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS
	case 'P': {
		*p_arg = arg;
		return tpp_emitter_cli_enable_no_line_commands(self);
	}	break;
#endif /* TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS */

#if TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES
	case 'H': {
		*p_arg = arg;
		return tpp_emitter_cli_enable_trace_includes(self);
	}	break;
#endif /* TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES */

	default: break;
	}
	return TPP_ENOENT;
}



/* Convenience wrapper around `tpp_emitter_cli_loader_parsearg()`.
 * For more information, see `tpp_cli_loader_parseargv()`.
 *
 * @return: TPP_EOK:        Success (`*p_argc` and `*p_argv` were updated such that
 *                          they contain all unrecognized arguments, as well as all
 *                          input files for the emitter).
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O Error
 * @return: TPP_ELEXERROR:  A emitter error was thrown
 * @return: TPP_EWARNPRINT: An error happened within a warning printer */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_emitter_cli_loader_parseargv(tpp_emitter_cli_loader *tpp_restrict self,
                                 int *p_argc, char ***p_argv) {
	tpp_errno result = TPP_EOK;
	int argc    = *p_argc;
	char **argv = *p_argv;
	unsigned int unknown_count = 0;
	while (argc > 0) {
		char *arg = argv[0];
		result = tpp_emitter_cli_loader_parsearg(self, arg);
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
		if (tpp_emitter_cli_loader_hasddash(self)) {
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

/* Ensure that `self` is in a *normal* state (meaning that there aren't any remaining,
 * unterminated multi-argument parameters). If that is not the case, then a warning
 * `TPP_W_MISSING_CLI_ARGUMENT` is emitted on `tpp_emitter_cli_loader_getemitter(self)`
 *
 * Unlike the other CLI loader functions above, this one *MUST* be called
 * *AFTER* the lexer's initial input file has been initialized, as it may
 * need to push additional files onto the `#include`-stack.
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O Error
 * @return: TPP_ELEXERROR:  A emitter error was thrown
 * @return: TPP_EWARNPRINT: An error happened within a warning printer */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_loader_flush(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Emit a warning if the CLI loader isn't in a neutral state */
#if TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT
	if (self->tcl_state != TPP_EMITTER_CLI_LOADER_STATE_NORMAL &&
	    self->tcl_state != TPP_EMITTER_CLI_LOADER_STATE_DDASH) {
		tpp_errno error = tpp_emitter_cli_warnf(self->tcl_emitter, NULL, 0,
		                                        TPP_W_MISSING_CLI_ARGUMENT);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT */

#if (TPP_EMITTER_HAVE_CLI_DASH_DUMP_M || TPP_EMITTER_HAVE_CLI_DASH_DUMP_D || TPP_EMITTER_HAVE_CLI_DASH_DUMP_N) && TPP_HAVE_LEXER_DUMP_DEFINITIONS
	if (self->tcl_flags & _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M) {
		tpp_ssize temp;
		temp = tpp_lexer_dump_definitions(tpp_emitter_getlexer(self->tcl_emitter),
		                                  tpp_emitter_getoutput(self->tcl_emitter),
		                                  self->tcl_emitter,
		                                  TPP_LEXER_DUMP_DEFINITIONS_BUILTIN_MACROS);
		if (temp < 0)
			return TPP_SSIZE_ASERR(temp);
	}
#endif /* (TPP_EMITTER_HAVE_CLI_DASH_DUMP_M || TPP_EMITTER_HAVE_CLI_DASH_DUMP_D || TPP_EMITTER_HAVE_CLI_DASH_DUMP_N) && TPP_HAVE_LEXER_DUMP_DEFINITIONS */

	return TPP_EOK;
}

#endif /* TPP_EMITTER_HAVE_CLI */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EMITTER_CLI_C */
