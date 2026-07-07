# Hook configuration

TPP supports a number of either compile-time, or runtime-time (or builtin, or disabled entirely) hooks. Each hook serves a different purpose to allow the API user to define additional behavior, or be notified of certain out-of-band events.

Every hook consists of 2 macros:
- `TPP_HAVE_SOMEEVENT_HOOK`: Configures how (and if) the hook is called / can be configured (see below)
- `TPP_HOOK_SOMEEVENT`: Defined as either a direct macro implementing the hook (only possible for certain hooks, and only when `TPP_HAVE_SOMEEVENT_HOOK == TPP_HOOK_CONST_USER`), or the name of user-defined function that implements the hook.

The `TPP_HAVE_SOMEEVENT_HOOK` must be defined to one of the following values:

- `TPP_HOOK_DISABLED`: The hook is completely disabled
- `TPP_HOOK_CONST_USER`: The hook is compile-time hard-coded to call a user-supplied function (this function is specified via `TPP_HOOK_SOMEEVENT`)
- `TPP_HOOK_CONST_BUILTIN`: The hook is compile-time hard-coded to call a builtin implementation. If no builtin implementation exists, a no-op implementation is called, or the hook is simply omitted
- `TPP_HOOK_RT_USER`: The hook can be overwritten at runtime, but is default-configured to a function pointer formed by `&TPP_HOOK_SOMEEVENT`. As such, use of this option requires you to also define a macro `#define TPP_HOOK_SOMEEVENT my_default_impl_for_someevent`
- `TPP_HOOK_RT_BUILTIN`: The hook can be overwritten at runtime, and is default-configured to the builtin implementation of the hook. If there is no builtin implementation, this behaves the same as `TPP_HOOK_RT_NOOP`
- `TPP_HOOK_RT_NOOP`: The hook can be overwritten at runtime, and is default-configured to `NULL`. Additionally, if the hook is `NULL` during invocation, it behaves as a no-op.

Depending on being needed or not, hooks default-configure themselves as follows:

```c
#ifdef TPP_HOOK_SOMEEVENT
#define TPP_HAVE_SOMEEVENT_HOOK TPP_HOOK_DEFAULT_USER
#elif <HOOK_NEEDED> && <HOOK_HAS_BUILTIN_IMPL>
#define TPP_HAVE_SOMEEVENT_HOOK TPP_HOOK_DEFAULT_BUILTIN
#elif <HOOK_NEEDED>
#define TPP_HAVE_SOMEEVENT_HOOK TPP_HOOK_DEFAULT_NOOP
#else
#define TPP_HAVE_SOMEEVENT_HOOK TPP_HOOK_DISABLED
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
TPP_HOOK_SOMEEVENT == TPP_HOOK_CONST_BUILTIN ||
TPP_HOOK_SOMEEVENT == TPP_HOOK_RT_BUILTIN
```

<!--BEGIN:builtin_hook-->
- `TPP_HAVE_BUILTIN_WARNPRINTER_HOOK`
- `TPP_HAVE_BUILTIN_MESGPRINTER_HOOK`
- `TPP_HAVE_BUILTIN_PARSEEXPR_HOOK`
<!--END:builtin_hook-->


## Hooks

The following hooks exist and can each be overwritten individually:

<!--BEGIN:hooks-->
## TPP_HAVE_WARNPRINTER_HOOK

```c
tpp_formatprinter TPP_HOOK_WARNPRINTER;
```

Called by `tpp_lexer_warnf()` to print warning messages

<details><summary>Default</summary>

```c
TPP_HAVE_WARNINGS
```
</details>


## TPP_HAVE_MESGPRINTER_HOOK

```c
tpp_formatprinter TPP_HOOK_MESGPRINTER;
```

Used by `#pragma message` to print messages

<details><summary>Default</summary>

```c
TPP_HAVE_PRAGMA_MESSAGE
```
</details>


## TPP_HAVE_PARSEEXPR_HOOK

```c
tpp_errno (TPPCALL *TPP_HOOK_PARSEEXPR)(tpp_lexer *tpp_restrict self, tpp_expr_value *tpp_restrict result);
```

User-defined callback for parsing `#if`-style expressions
- This callback is invoked in a context where `self` points
  before the expression's first token (meaning that this
  callback is responsible to do the initial yield using
  whatever method it wants to use).
- When it is known that the expression has finite length,
  as in: it has to end before EOF, or at the next unmatched
  `)`-token, the caller will have configured the lexer's
  current EOF accordingly (and disabled file-popping)
- When this function returns an error, the caller will rewind
  back to the start of the expression (or even further, if
  applicable; meaning this callback doesn't need to concern
  itself with rollback)


<details><summary>Default</summary>

```c
TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_MACRO___TPP_EVAL || TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed
```
</details>


## TPP_HAVE_UNKNOWN_PRAGMA_HOOK

```c
tpp_errno (TPPCALL *TPP_HOOK_UNKNOWN_PRAGMA)(tpp_lexer *tpp_restrict self);
```

Called whenever a `#pragma` is encountered that is not recognized.
When called, the lexer is set-up to point at the first token after the `#pragma`.

<details><summary>Default</summary>

```c
TPP_HAVE_PRAGMA && TPP_PROFILE == TPP_PROFILE_ALL
```
</details>


## TPP_HAVE_NEW_DEPENDENCY_HOOK

```c
tpp_errno (TPPCALL *TPP_HOOK_NEW_DEPENDENCY)(tpp_lexer *tpp_restrict self, tpp_keyword *filename_kwd);
```

Called whenever some file is `#include`-ed for the first time

<details><summary>Default</summary>

```c
TPP_HAVE_LEXER_OPENFILE && TPP_HAVE_USER_KEYWORDS && TPP_PROFILE == TPP_PROFILE_ALL
```
</details>


## TPP_HAVE_IDENT_SCCS_HOOK

```c
tpp_errno (TPPCALL *TPP_HOOK_IDENT_SCCS)(tpp_lexer *tpp_restrict self, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len);
```

Called to handle `#ident` and `#sccs` directives

<details><summary>Default</summary>

```c
TPP_HAVE_CPP_IDENT_SCCS
```
</details>


## TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK

```c
tpp_errno (TPPCALL *TPP_HOOK_SYSTEM_INCLUDE_PATH)(tpp_lexer *tpp_restrict self, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg);
```

Extra callback invoked by `tpp_lexer_foreach_include_path()` at diffrent
points during the process of enumerating include paths. This callback is
then allowed to enumerate some additional include paths that may exist, but
for one reason or another (mainly: speed) aren't known to TPP via its system
include path APIs (`tpp_lexer_includes_add*`)

<details><summary>Default</summary>

```c
TPP_HAVE_LEXER_OPEN_INCLUDE_STRING && TPP_PROFILE == TPP_PROFILE_ALL
```
</details>


## TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK

```c
tpp_ssize (TPPCALL *TPP_HOOK_UNKNOWN_STRING_ESCAPE)(tpp_lexer *tpp_restrict self, tpp_char const **p_pos, tpp_char const *end, tpp_formatprinter data_printer, tpp_formatprinter utf8_printer, void *arg);
```

Called by `tpp_lexer_decodestring()` when an unknown `\`-escape sequence is encountered
This hook can be used to define additional, user-defined escape sequences, or any other
arbitrary behavior to-be performed when specific escape-sequences are found.
On entry, `*p_pos` points at the first (unrecognized) character after the leading `\`, and
if the hook was able to parse said escape sequence, it should update `*p_pos` to point after
it before returning

<details><summary>Default</summary>

```c
TPP_HAVE_STRING_ESCAPE && TPP_HAVE_LEXER_DECODESTRING && TPP_PROFILE == TPP_PROFILE_ALL
```
</details>


## TPP_HAVE_RAISE_LEXERROR_HOOK

```c
tpp_errno (TPPCALL *TPP_HOOK_RAISE_LEXERROR)(tpp_lexer *tpp_restrict self);
```

Called by `tpp_lexer_warnf()` just before it's about to return `TPP_ELEXERROR`
This hook can be used to do additional state changes that may be necessary by the
hosting application in order to handle the resulting `TPP_ELEXERROR`

<details><summary>Default</summary>

```c
TPP_HAVE_STRING_ESCAPE && TPP_HAVE_LEXER_DECODESTRING && TPP_PROFILE == TPP_PROFILE_ALL
```
</details>


<!--END:hooks-->
