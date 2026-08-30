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
| `TPP_HOOK_RT_MANY` | A sort-of hybrid between `TPP_HOOK_RT_USER` and `TPP_HOOK_RT_NOOP` that allows multiple hooks to be dynamically registered at runtime. When a hook `TPP_HOOK_{NAME}` is also defined, that hook will *always* be called *after* all dynamically registered hooks were invoked. |
| `TPP_HOOK_RT_USER_C` | Same as `TPP_HOOK_RT_USER`, but allow a custom `void *cookie` to be defined alongside the hook |
| `TPP_HOOK_RT_BUILTIN_C` | Same as `TPP_HOOK_RT_BUILTIN`, but allow a custom `void *cookie` to be defined alongside the hook |
| `TPP_HOOK_RT_NOOP_C` | Same as `TPP_HOOK_RT_NOOP`, but allow a custom `void *cookie` to be defined alongside the hook |
| `TPP_HOOK_RT_MANY_C` | Same as `TPP_HOOK_RT_MANY`, but allow a custom `void *cookie` to be defined alongside the hook |

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

| Config | `TPP_PROFILE` | `TPP_COMMON_HAVE_HOOK_COOKIES=0` | `TPP_COMMON_HAVE_HOOK_COOKIES=1` |
| ------ | ------------- | -------------------------------- | -------------------------------- |
| `TPP_HOOK_DEFAULT_BUILTIN` | `TPP_PROFILE_ALL` | `TPP_HOOK_RT_BUILTIN` | `TPP_HOOK_RT_BUILTIN_C` |
| `TPP_HOOK_DEFAULT_BUILTIN` | - | `TPP_HOOK_CONST_BUILTIN` | `TPP_HOOK_CONST_BUILTIN` |
| `TPP_HOOK_DEFAULT_USER` | `TPP_PROFILE_ALL` | `TPP_HOOK_RT_MANY` | `TPP_HOOK_RT_MANY_C` |
| `TPP_HOOK_DEFAULT_USER` | `TPP_PROFILE_MINIMAL` | `TPP_HOOK_CONST_USER` | `TPP_HOOK_CONST_USER` |
| `TPP_HOOK_DEFAULT_USER` | - | `TPP_HOOK_RT_USER` | `TPP_HOOK_RT_USER_C` |
| `TPP_HOOK_DEFAULT_NOOP` | `TPP_PROFILE_ALL` | `TPP_HOOK_RT_MANY` | `TPP_HOOK_RT_MANY_C` |
| `TPP_HOOK_DEFAULT_NOOP` | `TPP_PROFILE_MINIMAL` | `TPP_HOOK_DISABLED` | `TPP_HOOK_DISABLED` |
| `TPP_HOOK_DEFAULT_NOOP` | - | `TPP_HOOK_RT_NOOP` | `TPP_HOOK_RT_NOOP_C` |


Generation of the default implementations of hooks is controlled by the following configuration macros. Each of these macros is default-configured as follows (meaning the default implementation is provided only if it is needed based on the hooks configuration):

```c
#define TPP_HOOK_USESBUILTIN(x) ((x) == TPP_HOOK_CONST_BUILTIN || (x) == TPP_HOOK_RT_BUILTIN || (x) == TPP_HOOK_RT_BUILTIN_C)
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

Called by [`tpp_lexer_warnf()`](../src/tpp-amalgamation.h#L29335) to print warning messages.
Potentially unused if [`TPP_HAVE_WARNHANDLER_HOOK`](#tpp_have_warnhandler_hook) is also overwritten

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
tpp_errno TPP_HOOK_WARNHANDLER(COOKIE cookie, tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args);
```

Called by [`tpp_lexer_warnf()`](../src/tpp-amalgamation.h#L29335) to handle warning notifications. Can be
overwritten to implement custom behavior in regards to handling of warnings.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_warnhandler(COOKIE cookie, tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args) {
	return TPP_EOK;
}
```

Builtin (internal; don't call directly):

```c
#if TPP_HAVE_BUILTIN_WARNHANDLER_HOOK
tpp_errno _tpp_lexer_builtin_warnhandler(COOKIE cookie, tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args);
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
tpp_errno TPP_HOOK_PARSEEXPR(COOKIE cookie, tpp_expr_value *tpp_restrict result);
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
tpp_errno _tpp_disabled_hook_parseexpr(COOKIE cookie, tpp_expr_value *tpp_restrict result) {
	return tpp_expr_value_init_zero(result);
}
```

Builtin (internal; don't call directly):

```c
#if TPP_HAVE_BUILTIN_PARSEEXPR_HOOK
tpp_errno _tpp_lexer_builtin_parseexpr(COOKIE cookie, tpp_expr_value *tpp_restrict result);
#endif /* TPP_HAVE_BUILTIN_PARSEEXPR_HOOK */
```
</details>


## TPP_HAVE_UNKNOWN_PRAGMA_HOOK

```c
tpp_errno TPP_HOOK_UNKNOWN_PRAGMA(COOKIE cookie);
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
tpp_errno _tpp_disabled_hook_unknown_pragma(COOKIE cookie) {
	return TPP_ENOENT;
}
```
</details>


## TPP_HAVE_NEW_DEPENDENCY_HOOK

```c
tpp_errno TPP_HOOK_NEW_DEPENDENCY(COOKIE cookie, tpp_keyword *filename_kwd);
```

Called whenever some file is `#include`-ed for the first time

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_OPENFILE && TPP_HAVE_USER_KEYWORDS && (TPP_PROFILE == TPP_PROFILE_ALL)
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_new_dependency(COOKIE cookie, tpp_keyword *filename_kwd) {
	return TPP_EOK;
}
```
</details>


## TPP_HAVE_FILE_PUSHED_HOOK

```c
tpp_errno TPP_HOOK_FILE_PUSHED(COOKIE cookie);
```

Called whenever a file was just pushed onto the `#include`-stack. Information
about the just-pushed file can be retrieved by examining [`tpp_lexer_getfile(LEXER)`](../src/tpp-amalgamation.h#L26768).

Notes:

- This hook can be used by a frontend to implement stuff like GCC's `--trace-includes`.
- This hook is *NOT* called for [`tpp_file_subtext_push()`](../src/tpp-amalgamation.h#L21427) or [`tpp_file_pushdummy()`](../src/tpp-amalgamation.h#L21845)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_INCLUDE_STACK
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_file_pushed(COOKIE cookie) {
	return TPP_EOK;
}
```
</details>


## TPP_HAVE_FILE_POPPED_HOOK

```c
void TPP_HOOK_FILE_POPPED(COOKIE cookie);
```

Called whenever a file is about to be popped off the `#include`-stack.
Information about the file that's about-to-be popped can be retrieved
by examining [`tpp_lexer_getfile(LEXER)`](../src/tpp-amalgamation.h#L26768).

Notes:

- When files are popped by [`tpp_lexer_manualpopfile_popfile()`](../src/tpp-amalgamation.h#L28347) within a
  [`tpp_lexer_manualpopfile_start()`](../src/tpp-amalgamation.h#L28345)-region, this hook is called during the
  *commit* phase (i.e.: by [`tpp_lexer_manualpopfile_break_commit()`](../src/tpp-amalgamation.h#L28352)),
  rather than [`tpp_lexer_manualpopfile_popfile()`](../src/tpp-amalgamation.h#L28347) as one might suspect at first.
- This hook is *NOT* called by [`tpp_file_subtext_pop()`](../src/tpp-amalgamation.h#L21450) or [`tpp_file_popdummy()`](../src/tpp-amalgamation.h#L21858)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_INCLUDE_STACK
```

Disabled (mock):

```c
void _tpp_disabled_hook_file_popped(COOKIE cookie) {
	return (void)0;
}
```
</details>


## TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK

```c
tpp_errno TPP_HOOK_INCLUDE_ENCOUNTERED(COOKIE cookie, tpp_hook_include_kind include_kind);
```

Called when a `#include` (or `#include_next`, `#import` or `#embed`)-directive
is encountered, at the point in time when the lexer's current token has already
been populated by [`tpp_lexer_yieldraw_at_include_string_blocking()`](../src/tpp-amalgamation.h#L28523) (and macros
were also already expanded), and the current token is [`TPP_TOK_INCPATH_DQUOTE`](../src/tpp-amalgamation.h#L28492) or
[`TPP_TOK_INCPATH_LANGLE`](../src/tpp-amalgamation.h#L28493).

This hook is primarily here for the purpose of implementing GCC's `-dI` switch, but
it could also be used for other purposes, such as intentionally skipping certain
`#include`-directives.

To gain access to the `#include`-string, you must use [`tpp_lexer_decode_include_string_cb()`](../src/tpp-amalgamation.h#L28558)


<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && (TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED)
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_include_encountered(COOKIE cookie, tpp_hook_include_kind include_kind) {
	return TPP_EOK;
}
```
</details>


## TPP_HAVE_INCLUDE_NOT_FOUND_HOOK

```c
tpp_errno TPP_HOOK_INCLUDE_NOT_FOUND(COOKIE cookie, tpp_hook_include_kind include_kind);
```

Called when the file specified by a `#include` (or `#include_next`, `#import` or
`#embed`)-directive could not be found. This hook may be used to either suppress
the error (by returning something other than [`TPP_ENOENT`](../src/tpp-amalgamation.h#L13464)), or log the error to
implement something like GCC's `-MG` commandline switch.

This hook is called just before [`TPP_W_NO_SUCH_FILE`](../src/tpp-amalgamation.h#L4820) would be emitted, with the lexer's
current token still being the `<stdio.h>` or `"file.h"` string, meaning if you want
to know what that string says, you can use [`tpp_lexer_decode_include_string_cb()`](../src/tpp-amalgamation.h#L28558) to decode it.


<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && (TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED)
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_include_not_found(COOKIE cookie, tpp_hook_include_kind include_kind) {
	return TPP_ENOENT;
}
```
</details>


## TPP_HAVE_MACRO_DEFINED_HOOK

```c
tpp_errno TPP_HOOK_MACRO_DEFINED(COOKIE cookie, tpp_keyword *tpp_restrict name, tpp_macro *tpp_restrict macro);
```

Called whenever a `#define` directive has just been fully
parsed (macro was has not yet been registered with keyword).

- This hook is *ONLY* invoked when `#define` is encountered, or
  `#pragma pop_macro("foo")` was used to restore a macro's previous
  definition.
- Calls to [`tpp_lexer_define()`](../src/tpp-amalgamation.h#L27998) or other related functions will
  *NOT* invoke this hook.


<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_CPP_DEFINE
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_macro_defined(COOKIE cookie, tpp_keyword *tpp_restrict name, tpp_macro *tpp_restrict macro) {
	return TPP_EOK;
}
```
</details>


## TPP_HAVE_MACRO_UNDEFINED_HOOK

```c
tpp_errno TPP_HOOK_MACRO_UNDEFINED(COOKIE cookie, tpp_keyword *tpp_restrict name);
```

Called whenever a `#undef` directive has just been fully
parsed (macro hasn't been deleted from keyword, yet). Note
that this hook is still called, even if the keyword doesn't
have a macro (and might have even already been marked as having
no predefined definition: [`_TPP_KEYWORD_MACRO_UNDEFINED`](../src/tpp-amalgamation.h#L22205)). This
hook is imply called as part of the process of evaluating `#undef`

This hook is *ONLY* invoked when `#undef` is encountered.
Calls to [`tpp_lexer_undef()`](../src/tpp-amalgamation.h#L28010), [`tpp_keyword_undef()`](../src/tpp-amalgamation.h#L22770), or other
related functions will *NOT* invoke this hook.

NOTE: this hook *will* actually also be called by `#pragma push_macro(undef, "foo")`


<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_CPP_DEFINE
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_macro_undefined(COOKIE cookie, tpp_keyword *tpp_restrict name) {
	return TPP_EOK;
}
```
</details>


## TPP_HAVE_IDENT_SCCS_HOOK

```c
tpp_errno TPP_HOOK_IDENT_SCCS(COOKIE cookie, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len);
```

Called to handle `#ident` and `#sccs` directives

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_IDENT_SCCS
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_ident_sccs(COOKIE cookie, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len) {
	return TPP_EOK;
}
```
</details>


## TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK

```c
tpp_errno TPP_HOOK_SYSTEM_INCLUDE_PATH(COOKIE cookie, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg);
```

Extra callback invoked by [`tpp_lexer_foreach_include_path()`](../src/tpp-amalgamation.h#L28589) at different
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
tpp_errno _tpp_disabled_hook_system_include_path(COOKIE cookie, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg) {
	return TPP_ENOENT;
}
```
</details>


## TPP_HAVE_SYSTEM_EMBED_PATH_HOOK

```c
tpp_errno TPP_HOOK_SYSTEM_EMBED_PATH(COOKIE cookie, tpp_token_id mode, tpp_hook_system_embed_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to), void *arg);
```

Extra callback invoked by [`tpp_lexer_foreach_embed_path()`](../src/tpp-amalgamation.h#L28631) at different points
during the process of enumerating embed paths. (s.a. [`TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK`](#tpp_have_system_include_path_hook))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_OPEN_EMBED_STRING && (TPP_PROFILE == TPP_PROFILE_ALL)
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_system_embed_path(COOKIE cookie, tpp_token_id mode, tpp_hook_system_embed_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to), void *arg) {
	return TPP_ENOENT;
}
```
</details>


## TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK

```c
tpp_ssize TPP_HOOK_UNKNOWN_STRING_ESCAPE(COOKIE cookie, tpp_char const **tpp_restrict p_pos, tpp_char const *end, tpp_lexer_decodestring_config const *tpp_restrict config);
```

Called by [`tpp_lexer_decodestring()`](../src/tpp-amalgamation.h#L29094) when an unknown `\`-escape sequence is encountered.
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
tpp_ssize _tpp_disabled_hook_unknown_string_escape(COOKIE cookie, tpp_char const **tpp_restrict p_pos, tpp_char const *end, tpp_lexer_decodestring_config const *tpp_restrict config) {
	return TPP_SSIZE_OFERR(TPP_ENOENT);
}
```
</details>


## TPP_HAVE_RAISE_LEXERROR_HOOK

```c
tpp_errno TPP_HOOK_RAISE_LEXERROR(COOKIE cookie);
```

Called by [`tpp_lexer_warnf()`](../src/tpp-amalgamation.h#L29335) just before it's about to return [`TPP_ELEXERROR`](../src/tpp-amalgamation.h#L13512)
This hook can be used to do additional state changes that may be necessary by the
hosting application in order to handle the resulting [`TPP_ELEXERROR`](../src/tpp-amalgamation.h#L13512)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRING_ESCAPE && TPP_HAVE_LEXER_DECODESTRING && (TPP_PROFILE == TPP_PROFILE_ALL)
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_raise_lexerror(COOKIE cookie) {
	return TPP_ELEXERROR;
}
```
</details>


## TPP_HAVE_ISFLOATSUFFIX_HOOK

```c
tpp_errno TPP_HOOK_ISFLOATSUFFIX(COOKIE cookie, tpp_char const *pos);
```

Called by [`tpp_lexer_yieldraw()`](../src/tpp-amalgamation.h#L28189) when [`TPP_HAVE_SMART_FLOAT_TOKENS`](config-conf.md#tpp_have_smart_float_tokens) is enabled and
a sequence like `1.f` is encountered where the lexer is unsure if the `f` should be
part of the float-token (in the form of a float-suffix), or if this should actually be
parsed as 3 tokens: `[C_INT:1][DOT:.][f:f]`. For this purpose, this hook is called
with `pos` pointing at the `f` (additional characters thereafter may not be loaded
yet, though can be loaded using [`tpp_lexer_readchar()`](../src/tpp-amalgamation.h#L28099) and [`tpp_lexer_readunichar()`](../src/tpp-amalgamation.h#L28108))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TOK_C_FLOAT && TPP_HAVE_SMART_FLOAT_TOKENS
```

Disabled (mock):

```c
tpp_errno _tpp_disabled_hook_isfloatsuffix(COOKIE cookie, tpp_char const *pos) {
	return TPP_ENOENT;
}
```
</details>


<!--END:hooks-->
