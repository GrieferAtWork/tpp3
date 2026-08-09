# Hook configuration

TPP supports a number of either compile-time, or runtime-time (or builtin, or disabled entirely) hooks. Each hook serves a different purpose to allow the API user to define additional behavior, or be notified of certain out-of-band events.

Every hook consists of 2 macros:
- `TPP_HAVE_{NAME}_HOOK`: Configures how (and if) the hook is called / can be assigned (see below)
- `TPP_HOOK_{NAME}`: Either defined as a function-like macro implementing the hook (only possible for certain hooks, and only when `TPP_HAVE_{NAME}_HOOK == TPP_HOOK_CONST_USER`), or as a keyword-style macro expanding to the name of a user-defined function implementing the hook.

The `TPP_HAVE_{NAME}_HOOK` macro must be defined to one of the following values:

| Value | Description |
| ----- | ----------- |
| `TPP_HOOK_DISABLED` | The hook is completely disabled (`TPP_HOOK_{NAME}` is unused) |
| `TPP_HOOK_CONST_USER` | The hook is compile-time hard-coded to call a user-supplied function (or function-like macro), as specified by `TPP_HOOK_{NAME}` |
| `TPP_HOOK_CONST_BUILTIN` | The hook is compile-time hard-coded to call a builtin implementation. If no builtin implementation exists, a no-op implementation is called, or the hook is simply omitted like it would be with `TPP_HOOK_DISABLED` |
| `TPP_HOOK_RT_USER` | The hook can be overwritten at runtime, but is default-initialized as `&TPP_HOOK_{NAME}`. As such, use of this option requires you to also define a macro `#define TPP_HOOK_{NAME} my_handler_for_{NAME}`, which also cannot be a function-like macro |
| `TPP_HOOK_RT_BUILTIN` | The hook can be overwritten at runtime, and is default-configured to the builtin implementation of the hook. If there is no builtin implementation, this behaves the same as `TPP_HOOK_RT_NOOP` |
| `TPP_HOOK_RT_NOOP` | The hook can be overwritten at runtime, and is default-configured to `NULL`. Additionally, if the hook is `NULL` when TPP tries to invoke it, it behaves as a no-op. |

Depending on being needed or not, hooks default-configure themselves as follows:

```c
#ifdef TPP_HOOK_{NAME}
#define TPP_HAVE_{NAME}_HOOK TPP_HOOK_DEFAULT_USER
#elif <HOOK_NEEDED> && <HOOK_HAS_BUILTIN_IMPL>
#define TPP_HAVE_{NAME}_HOOK TPP_HOOK_DEFAULT_BUILTIN
#elif <HOOK_NEEDED>
#define TPP_HAVE_{NAME}_HOOK TPP_HOOK_DEFAULT_NOOP
#else
#define TPP_HAVE_{NAME}_HOOK TPP_HOOK_DISABLED
#endif
```

For this purpose, the `TPP_HOOK_DEFAULT_*` macros can each be overwritten as well, but their default definitions look like this:

```c
#define TPP_HOOK_DEFAULT_BUILTIN ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_RT_BUILTIN : TPP_HOOK_CONST_BUILTIN)
#define TPP_HOOK_DEFAULT_USER    ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_RT_USER : TPP_HOOK_CONST_USER)
#define TPP_HOOK_DEFAULT_NOOP    ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_RT_NOOP : TPP_HOOK_DISABLED)
```


Generation of the default implementations of hooks is controlled by the following configuration macros. Each of these macros is default-configured as follows (meaning the default implementation is provided only if it is needed based on the hooks configuration):

```c
#define TPP_HOOK_USESBUILTIN(x) ((x) == TPP_HOOK_CONST_BUILTIN || (x) == TPP_HOOK_RT_BUILTIN)
TPP_HOOK_USESBUILTIN(TPP_HOOK_{NAME})
```

<!--BEGIN:builtin_hook-->
- <a name="tpp_have_builtin_warnprinter_hook"></a>`TPP_HAVE_BUILTIN_WARNPRINTER_HOOK`
- <a name="tpp_have_builtin_warnhandler_hook"></a>`TPP_HAVE_BUILTIN_WARNHANDLER_HOOK`
- <a name="tpp_have_builtin_mesgprinter_hook"></a>`TPP_HAVE_BUILTIN_MESGPRINTER_HOOK`
- <a name="tpp_have_builtin_parseexpr_hook"></a>`TPP_HAVE_BUILTIN_PARSEEXPR_HOOK`
<!--END:builtin_hook-->


## Hooks

The following hooks exist and can each be overwritten individually:

<!--BEGIN:hooks-->
## TPP_HAVE_WARNPRINTER_HOOK

```c
TPP_FORMATPRINTER_DEFINE(TPP_HOOK_WARNPRINTER, arg, text, num_bytes);
```

Called by [`tpp_lexer_warnf()`](../src/tpp-amalgamation.h#L26750) to print warning messages.
Potentially unused if [`TPP_HAVE_WARNHANDLER_HOOK`](../src/tpp-amalgamation.h#L9133) is also overwritten

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS
```

Disabled (mock):

```c
TPP_FORMATPRINTER_DEFINE(_tpp_disabled_hook_warnprinter, arg, text, num_bytes) {
	return 0;
}
```

Builtin (internal; don't call directly):

```c
#if TPP_HAVE_BUILTIN_WARNPRINTER_HOOK
TPP_FORMATPRINTER_DEFINE(_tpp_lexer_builtin_warn_or_mesg_printer, arg, text, num_bytes);
#endif /* TPP_HAVE_BUILTIN_WARNPRINTER_HOOK */
```
</details>


## TPP_HAVE_WARNHANDLER_HOOK

```c
tpp_errno TPP_HOOK_WARNHANDLER(tpp_hook_cookie cookie, tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args);
```

Called by [`tpp_lexer_warnf()`](../src/tpp-amalgamation.h#L26750) to handle warning notifications. Can be
overwritten to implement custom behavior in regards to handling of warnings.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_warnhandler(tpp_hook_cookie cookie, tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args) {
	return TPP_EOK;
}
```

Builtin (internal; don't call directly):

```c
#if TPP_HAVE_BUILTIN_WARNHANDLER_HOOK
tpp_errno _tpp_lexer_builtin_warnhandler(tpp_hook_cookie cookie, tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args);
#endif /* TPP_HAVE_BUILTIN_WARNHANDLER_HOOK */
```
</details>


## TPP_HAVE_MESGPRINTER_HOOK

```c
TPP_FORMATPRINTER_DEFINE(TPP_HOOK_MESGPRINTER, arg, text, num_bytes);
```

Used by `#pragma message` to print messages (see [`TPP_HAVE_PRAGMA_MESSAGE`](config-conf.md#tpp_have_pragma_message))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA_MESSAGE
```

Disabled (mock):

```c
TPP_FORMATPRINTER_DEFINE(_tpp_disabled_hook_mesgprinter, arg, text, num_bytes) {
	return 0;
}
```

Builtin (internal; don't call directly):

```c
#if TPP_HAVE_BUILTIN_MESGPRINTER_HOOK
TPP_FORMATPRINTER_DEFINE(_tpp_lexer_builtin_warn_or_mesg_printer, arg, text, num_bytes);
#endif /* TPP_HAVE_BUILTIN_MESGPRINTER_HOOK */
```
</details>


## TPP_HAVE_PARSEEXPR_HOOK

```c
tpp_errno TPP_HOOK_PARSEEXPR(tpp_hook_cookie cookie, tpp_expr_value *tpp_restrict result);
```

User-defined callback for parsing `#if`-style expressions
- This callback is invoked in a context where the lexer
  points before the expression's first token (meaning that
  this callback is responsible to do the initial yield
  using whatever method it wants to use).
- When it is known that the expression has finite length,
  as in: it has to end before EOF, or at the next unmatched
  `)`-token, the caller will have configured the lexer's
  current EOF accordingly (and disabled file-popping)
- When this function returns an error, the caller will rewind
  back to the start of the expression (or even further, if
  applicable; meaning this callback doesn't need to concern
  itself with rollback)


<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_MACRO___TPP_EVAL || TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_parseexpr(tpp_hook_cookie cookie, tpp_expr_value *tpp_restrict result) {
	return tpp_expr_value_init_zero(result);
}
```

Builtin (internal; don't call directly):

```c
#if TPP_HAVE_BUILTIN_PARSEEXPR_HOOK
tpp_errno _tpp_lexer_builtin_parseexpr(tpp_hook_cookie cookie, tpp_expr_value *tpp_restrict result);
#endif /* TPP_HAVE_BUILTIN_PARSEEXPR_HOOK */
```
</details>


## TPP_HAVE_UNKNOWN_PRAGMA_HOOK

```c
tpp_errno TPP_HOOK_UNKNOWN_PRAGMA(tpp_hook_cookie cookie);
```

Called whenever a `#pragma` is encountered that is not recognized.
When called, the lexer is set-up to point at the first token after the `#pragma`.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA && (TPP_PROFILE == TPP_PROFILE_ALL)
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_unknown_pragma(tpp_hook_cookie cookie) {
	return TPP_ENOENT;
}
```
</details>


## TPP_HAVE_NEW_DEPENDENCY_HOOK

```c
tpp_errno TPP_HOOK_NEW_DEPENDENCY(tpp_hook_cookie cookie, tpp_keyword *filename_kwd);
```

Called whenever some file is `#include`-ed for the first time

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_OPENFILE && TPP_HAVE_USER_KEYWORDS && (TPP_PROFILE == TPP_PROFILE_ALL)
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_new_dependency(tpp_hook_cookie cookie, tpp_keyword *filename_kwd) {
	return TPP_EOK;
}
```
</details>


## TPP_HAVE_FILE_PUSHED_HOOK

```c
tpp_errno TPP_HOOK_FILE_PUSHED(tpp_hook_cookie cookie);
```

Called whenever a file was just pushed onto the `#include`-stack. Information
about the just-pushed file can be retrieved by examining [`tpp_lexer_getfile(LEXER)`](../src/tpp-amalgamation.h#L24405).

Notes:

- This hook can be used by a frontend to implement stuff like GCC's `--trace-includes`.
- This hook is *NOT* called for [`tpp_file_subtext_push()`](../src/tpp-amalgamation.h#L20256) or [`tpp_file_pushdummy()`](../src/tpp-amalgamation.h#L20648)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_INCLUDE_STACK
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_file_pushed(tpp_hook_cookie cookie) {
	return TPP_EOK;
}
```
</details>


## TPP_HAVE_FILE_POPPED_HOOK

```c
void TPP_HOOK_FILE_POPPED(tpp_hook_cookie cookie);
```

Called whenever a file is about to be popped off the `#include`-stack.
Information about the file that's about-to-be popped can be retrieved
by examining [`tpp_lexer_getfile(LEXER)`](../src/tpp-amalgamation.h#L24405).

Notes:

- When files are popped by [`tpp_lexer_manualpopfile_popfile()`](../src/tpp-amalgamation.h#L25768) within a
  [`tpp_lexer_manualpopfile_start()`](../src/tpp-amalgamation.h#L25766)-region, this hook is called during the
  *commit* phase (i.e.: by [`tpp_lexer_manualpopfile_break_commit()`](../src/tpp-amalgamation.h#L25773)),
  rather than [`tpp_lexer_manualpopfile_popfile()`](../src/tpp-amalgamation.h#L25768) as one might suspect at first.
- This hook is *NOT* called by [`tpp_file_subtext_pop()`](../src/tpp-amalgamation.h#L20279) or [`tpp_file_popdummy()`](../src/tpp-amalgamation.h#L20661)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_INCLUDE_STACK
```

Disabled (mock):

```c
void _tpp_disabled_hook_file_popped(tpp_hook_cookie cookie) {
	return (void)0;
}
```
</details>


## TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK

```c
tpp_errno TPP_HOOK_INCLUDE_ENCOUNTERED(tpp_hook_cookie cookie, tpp_hook_include_kind include_kind);
```

Called when a `#include` (or `#include_next`, `#import` or `#embed`)-directive
is encountered, at the point in time when the lexer's current token has already
been populated by [`tpp_lexer_yieldraw_at_include_string_blocking()`](../src/tpp-amalgamation.h#L25944) (and macros
were also already expanded), and the current token is [`TPP_TOK_INCPATH_DQUOTE`](../src/tpp-amalgamation.h#L25913) or
[`TPP_TOK_INCPATH_LANGLE`](../src/tpp-amalgamation.h#L25914).

This hook is primarily here for the purpose of implementing GCC's `-dI` switch, but
it could also be used for other purposes, such as intentionally skipping certain
`#include`-directives.

To gain access to the `#include`-string, you must use [`tpp_lexer_decode_include_string_cb()`](../src/tpp-amalgamation.h#L25979)


<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && (TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED)
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_include_encountered(tpp_hook_cookie cookie, tpp_hook_include_kind include_kind) {
	return TPP_EOK;
}
```
</details>


## TPP_HAVE_INCLUDE_NOT_FOUND_HOOK

```c
tpp_errno TPP_HOOK_INCLUDE_NOT_FOUND(tpp_hook_cookie cookie, tpp_hook_include_kind include_kind);
```

Called when the file specified by a `#include` (or `#include_next`, `#import` or
`#embed`)-directive could not be found. This hook may be used to either suppress
the error (by returning something other than [`TPP_ENOENT`](../src/tpp-amalgamation.h#L12805)), or log the error to
implement something like GCC's `-MG` commandline switch.

This hook is called just before [`TPP_W_NO_SUCH_FILE`](../src/tpp-amalgamation.h#L4800) would be emitted, with the lexer's
current token still being the `<stdio.h>` or `"file.h"` string, meaning if you want
to know what that string says, you can use [`tpp_lexer_decode_include_string_cb()`](../src/tpp-amalgamation.h#L25979) to decode it.


<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && (TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED)
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_include_not_found(tpp_hook_cookie cookie, tpp_hook_include_kind include_kind) {
	return TPP_ENOENT;
}
```
</details>


## TPP_HAVE_MACRO_DEFINED_HOOK

```c
tpp_errno TPP_HOOK_MACRO_DEFINED(tpp_hook_cookie cookie, tpp_keyword *tpp_restrict name, tpp_macro *tpp_restrict macro);
```

Called whenever a `#define` directive has just been fully
parsed (macro was has not yet been registered with keyword).

- This hook is *ONLY* invoked when `#define` is encountered, or
  `#pragma pop_macro("foo")` was used to restore a macro's previous
  definition.
- Calls to [`tpp_lexer_define()`](../src/tpp-amalgamation.h#L25417) or other related functions will
  *NOT* invoke this hook.


<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_CPP_DEFINE
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_macro_defined(tpp_hook_cookie cookie, tpp_keyword *tpp_restrict name, tpp_macro *tpp_restrict macro) {
	return TPP_EOK;
}
```
</details>


## TPP_HAVE_MACRO_UNDEFINED_HOOK

```c
tpp_errno TPP_HOOK_MACRO_UNDEFINED(tpp_hook_cookie cookie, tpp_keyword *tpp_restrict name);
```

Called whenever a `#undef` directive has just been fully
parsed (macro hasn't been deleted from keyword, yet). Note
that this hook is still called, even if the keyword doesn't
have a macro (and might have even already been marked as having
no predefined definition: [`_TPP_KEYWORD_MACRO_UNDEFINED`](../src/tpp-amalgamation.h#L21006)). This
hook is imply called as part of the process of evaluating `#undef`

This hook is *ONLY* invoked when `#undef` is encountered.
Calls to [`tpp_lexer_undef()`](../src/tpp-amalgamation.h#L25425), [`tpp_keyword_undef()`](../src/tpp-amalgamation.h#L21454), or other
related functions will *NOT* invoke this hook.

NOTE: this hook *will* actually also be called by `#pragma push_macro(undef, "foo")`


<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_CPP_DEFINE
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_macro_undefined(tpp_hook_cookie cookie, tpp_keyword *tpp_restrict name) {
	return TPP_EOK;
}
```
</details>


## TPP_HAVE_IDENT_SCCS_HOOK

```c
tpp_errno TPP_HOOK_IDENT_SCCS(tpp_hook_cookie cookie, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len);
```

Called to handle `#ident` and `#sccs` directives

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_IDENT_SCCS
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_ident_sccs(tpp_hook_cookie cookie, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len) {
	return TPP_EOK;
}
```
</details>


## TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK

```c
tpp_errno TPP_HOOK_SYSTEM_INCLUDE_PATH(tpp_hook_cookie cookie, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg);
```

Extra callback invoked by [`tpp_lexer_foreach_include_path()`](../src/tpp-amalgamation.h#L26010) at different
points during the process of enumerating include paths. This callback is
then allowed to enumerate some additional include paths that may exist, but
for one reason or another (mainly: speed) aren't known to TPP via its system
include path APIs (`tpp_lexer_includes_add*`)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_OPEN_INCLUDE_STRING && (TPP_PROFILE == TPP_PROFILE_ALL)
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_system_include_path(tpp_hook_cookie cookie, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg) {
	return TPP_ENOENT;
}
```
</details>


## TPP_HAVE_SYSTEM_EMBED_PATH_HOOK

```c
tpp_errno TPP_HOOK_SYSTEM_EMBED_PATH(tpp_hook_cookie cookie, tpp_token_id mode, tpp_hook_system_embed_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to), void *arg);
```

Extra callback invoked by [`tpp_lexer_foreach_embed_path()`](../src/tpp-amalgamation.h#L26052) at different points
during the process of enumerating embed paths. (s.a. [`TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK`](../src/tpp-amalgamation.h#L9656))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_OPEN_EMBED_STRING && (TPP_PROFILE == TPP_PROFILE_ALL)
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_system_embed_path(tpp_hook_cookie cookie, tpp_token_id mode, tpp_hook_system_embed_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to), void *arg) {
	return TPP_ENOENT;
}
```
</details>


## TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK

```c
tpp_ssize TPP_HOOK_UNKNOWN_STRING_ESCAPE(tpp_hook_cookie cookie, tpp_char const **p_pos, tpp_char const *end, tpp_lexer_decodestring_config const *tpp_restrict config);
```

Called by [`tpp_lexer_decodestring()`](../src/tpp-amalgamation.h#L26513) when an unknown `\`-escape sequence is encountered.
This hook can be used to define additional, user-defined escape sequences, or any other
arbitrary behavior to-be performed when specific escape-sequences are found.
On entry, `*p_pos` points at the first (unrecognized) character after the leading `\`, and
if the hook was able to parse said escape sequence, it should update `*p_pos` to point after
it before returning.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRING_ESCAPE && TPP_HAVE_LEXER_DECODESTRING && (TPP_PROFILE == TPP_PROFILE_ALL)
```

Disabled (mock):

```c
tpp_ssize _tpp_disabled_hook_unknown_string_escape(tpp_hook_cookie cookie, tpp_char const **p_pos, tpp_char const *end, tpp_lexer_decodestring_config const *tpp_restrict config) {
	return TPP_SSIZE_OFERR(TPP_ENOENT);
}
```
</details>


## TPP_HAVE_RAISE_LEXERROR_HOOK

```c
tpp_errno TPP_HOOK_RAISE_LEXERROR(tpp_hook_cookie cookie);
```

Called by [`tpp_lexer_warnf()`](../src/tpp-amalgamation.h#L26750) just before it's about to return [`TPP_ELEXERROR`](../src/tpp-amalgamation.h#L12853)
This hook can be used to do additional state changes that may be necessary by the
hosting application in order to handle the resulting [`TPP_ELEXERROR`](../src/tpp-amalgamation.h#L12853)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRING_ESCAPE && TPP_HAVE_LEXER_DECODESTRING && (TPP_PROFILE == TPP_PROFILE_ALL)
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_raise_lexerror(tpp_hook_cookie cookie) {
	return TPP_ELEXERROR;
}
```
</details>


## TPP_HAVE_ISFLOATSUFFIX_HOOK

```c
tpp_errno TPP_HOOK_ISFLOATSUFFIX(tpp_hook_cookie cookie, tpp_char const *pos);
```

Called by [`tpp_lexer_yieldraw()`](../src/tpp-amalgamation.h#L25608) when [`TPP_HAVE_SMART_FLOAT_TOKENS`](config-conf.md#tpp_have_smart_float_tokens) is enabled and
a sequence like `1.f` is encountered where the lexer is unsure if the `f` should be
part of the float-token (in the form of a float-suffix), or if this should actually be
parsed as 3 tokens: `[C_INT:1][DOT:.][f:f]`. For this purpose, this hook is called
with `pos` pointing at the `f` (additional characters thereafter may not be loaded
yet, though can be loaded using [`tpp_lexer_readchar()`](../src/tpp-amalgamation.h#L25512) and [`tpp_lexer_readunichar()`](../src/tpp-amalgamation.h#L25521))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TOK_C_FLOAT && TPP_HAVE_SMART_FLOAT_TOKENS
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_isfloatsuffix(tpp_hook_cookie cookie, tpp_char const *pos) {
	return TPP_ENOENT;
}
```
</details>


<!--END:hooks-->
