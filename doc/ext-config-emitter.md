# Emitter

The **EMITTER** is a [*Source extension*](./config.md#source_extensions) for TPP that allows you to re-emit a stream of tokens (as produced by a `tpp_lexer`) back into something that is both human-readable, and understood by (possibly another) preprocessor.

The TPP **EMITTER** uses either `#line` or `# <linenum>` (with additional support for dummy files and context flags `1/2/3/4`):

Input:

```c
#include <stdio.h>

int main() {
	printf("Hello World\n");
	return 0;
}
```

Possible outputs (examples):

```c
# 1 "input.c"
# 1 "/opt/my-project//"
# 1 "input.c"
# 1 "/usr/include/stdio.h" 1 3
[...]
# 3 "input.c" 2
int main() {
    fprintf((__acrt_iob_func(1)),"Hello World\n");
    return 0;
}
```

```c
#line 1 "/usr/include/stdio.h"
[...]
#line 3 "input.c"
int main() {
    fprintf((__acrt_iob_func(1)),"Hello World\n");
    return 0;
}
```

---

# Configuration

The following configuration options are available and can be used to customize the behavior of emitters.

<!--BEGIN:all-->
## TPP_EMITTER_HAVE_MODE_EMIT

Provide support for `TPP_EMITTER_MODE_EMIT`, which emits tokens

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
1
```
</details>

## TPP_EMITTER_HAVE_MODE_DISPOSE

Provide support for `TPP_EMITTER_MODE_DISPOSE`, which discards tokens

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_EMITTER_HAVE_MODE_BRACKET

Provide support for `TPP_EMITTER_MODE_BRACKET`, where
tokens are emitted surrounded by brackets.

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_EMITTER_HAVE_MODE_TYPED

Provide support for `TPP_EMITTER_MODE_TYPED`, where tokens are
emitted surrounded as `[{TYPE}:{TOKEN}]`, where `TYPE` is the
result of [`tpp_strtokenid()`](../src/tpp-amalgamation.h#L17293) and the canonical keyword name.

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRTOKENID && (TPP_PROFILE == TPP_PROFILE_ALL)
```
</details>

## TPP_EMITTER_HAVE_MODE_ZERO

Provide support for `TPP_EMITTER_MODE_ZERO`, where tokens are
emitted in their normalized form (see `TPP_EMITTER_HAVE_NORMALIZE_*`),
with every token followed by a NUL-byte. This format is primarily
meant for preprocessing small files such that another process can
(fairly easily) consume those tokens without having to worry about
the boundaries between tokens.

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_EMITTER_HAVE_NORMALIZE_SPACE

When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, any [`TPP_TOK_SPACE`](../src/tpp-amalgamation.h#L15854)-token
is emitted as an (appropriately long) sequence of ` `-characters, rather
than as an echo of the original token's space characters (thereby normalizing
any unicode whitespace or other control characters to `U+0020 SPACE`).

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_EMIT_TOKEN ? TPP_CONF_FEAT1 : 0
```
</details>

## TPP_EMITTER_HAVE_NORMALIZE_LF

When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, any [`TPP_TOK_LF`](../src/tpp-amalgamation.h#L15853)-token is
emitted as a `\n`-character, rather than as an echo of the original token's
linefeed bytes (thereby normalizing any unicode linefeed, CR, or CRLF
sequences to LF).

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_EMIT_TOKEN ? TPP_CONF_FEAT1 : 0
```
</details>

## TPP_EMITTER_HAVE_NORMALIZE_C_STRING

When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, any [`TPP_TOK_ISSTRING`](../src/tpp-amalgamation.h#L16314)-token
is emitted as a [`TPP_TOK_C_STRING`](../src/tpp-amalgamation.h#L16116) (or [`TPP_TOK_C_CHAR`](../src/tpp-amalgamation.h#L16194), when
[`TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS`](config-conf.md#tpp_have_builtin_expr_character_literals) is enabled in the lexer) token
(though only done if the desired target token is enabled).

In order to do this normalization, the string is decoded and re-encoded via
use of [`tpp_lexer_decodestring()`](../src/tpp-amalgamation.h#L28854) and [`tpp_token_encodestring()`](../src/tpp-amalgamation.h#L17378), thereby
allowing a consumer of the preprocessor output to only have to support a
greatly reduced set of string tokens (and escape sequences) in order to
fully understand *any* kind of string token that may be produced by TPP.

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
(TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_TOK_C_STRING && TPP_HAVE_TOKEN_ENCODESTRING && TPP_HAVE_LEXER_DECODESTRING) ? TPP_CONF_FEAT1 : 0
```
</details>

## TPP_EMITTER_HAVE_NORMALIZE_C_INT

When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, any
[`TPP_TOK_ISINT`](../src/tpp-amalgamation.h#L15928)-token is emitted as a [`TPP_TOK_C_INT`](../src/tpp-amalgamation.h#L15912) token.

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
(TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_TOK_C_INT && TPP_HAVE_LEXER_DECODEINT) ? TPP_CONF_FEAT1 : 0
```
</details>

## TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS

When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, normalize `\u`, `\U`
and `\N` escape sequences in keywords names to their actual utf-8 character
representation (also causes BSE sequences to be normalized, though if that's
all you want (and not just for keywords), you could also enable
[`TPP_EMITTER_HAVE_NORMALIZE_BSE`](#tpp_emitter_have_normalize_bse)).

This feature is also required to emit `__TPP_IDENTIFIER()` as the actual
identifier, rather than as a copy of the identifier itself.

NOTE: Keywords containing unicode characters that aren't SYMCONT will
      still see those characters get escaped by `\U`, though that's the
      only escape sequence that will still be emitted (and thus needs
      to be understood by a consuming preprocessor).

NOTE: The *empty* keyword (i.e. `__TPP_IDENTIFIER("")`) continues to
      be emitted as `__TPP_IDENTIFIER("")`, since there's no other way
      to write that identifier.

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
(TPP_EMITTER_HAVE_EMIT_TOKEN && (TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED)) ? TPP_CONF_FEAT1 : 0
```
</details>

## TPP_EMITTER_HAVE_NORMALIZE_BSE

When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, remove `\`-escaped
line-feeds from generic tokens.

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
(TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_BSE) ? TPP_CONF_FEAT1 : 0
```
</details>

## TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS

When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, normalize trigraph
sequences in generic tokens.

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
(TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_TRIGRAPHS) ? TPP_CONF_FEAT1 : 0
```
</details>

## TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS

When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, normalize digraph
sequences in generic tokens.

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
(TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_DIGRAPHS) ? TPP_CONF_FEAT1 : 0
```
</details>

## TPP_EMITTER_HAVE_NOLINE

When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, inhibit emission of
`#line` directives, as well as (re-)alignment of the output stream in
order to match source L/C info. When this is enabled, it is *highly*
suggested that you turn on emission of SPACE+LF tokens in the source
lexer, since otherwise the emitter will (probably) put everything on
1 line.

Can be used to implement the `-P` CLI switch
(see [`TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS`](#tpp_emitter_have_cli_dash_no_line_commands))

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_MODE_EMIT ? TPP_CONF_FEAT0 : 0
```
</details>

## TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN

When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, tokens emitted
from within a macro do not require proper alignment with `__COLUMN__`.

So-as to prevent every token from causing another `#line`-directive
being emitted, don't be too precise in terms of *all* tokens needing
to have the proper column:

```c
#define my_macro  10+20+30+40
5+my_macro+50
```

Without this (technically more correct):

```c
5+10
#line 2
  +
#line 2
  20
#line 2
  +
#line 2
  30
#line 2
  +
#line 2
  40      +50
```

With this:

```c
5+10+20+30+40
#line 2
          +50
```

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
(TPP_EMITTER_HAVE_MODE_EMIT && TPP_HAVE_CPP_MACROS) ? TPP_CONF_FEAT1 : 0
```
</details>

## TPP_EMITTER_HAVE_USE_CPP_DIGIT

Use `# <linenum>` instead of `#line` to align tokens

s.a.:

- [`TPP_HAVE_CPP_DIGIT_LINE`](config-conf.md#tpp_have_cpp_digit_line)
- [`TPP_HAVE_CPP_LINE`](config-conf.md#tpp_have_cpp_line)

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_MODE_EMIT ? TPP_CONF_FEAT1 : 0
```
</details>

## TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS

Extension to [`TPP_EMITTER_HAVE_USE_CPP_DIGIT`](#tpp_emitter_have_use_cpp_digit): also emit `1/2/3/4` flags:

- `1`: Push a dummy-file containing the old file/line/column onto the `#include`-stack,
       before applying the new line/filename.
- `2`: Do the inverse of flag `1` and pop a dummy-file off the `#include`-stack.
- `3`: Set [`TPP_FILE_FLAGS_SYSHDR`](../src/tpp-amalgamation.h#L20656) for the current text-file. When this flag is not
       supplied, [`TPP_FILE_FLAGS_SYSHDR`](../src/tpp-amalgamation.h#L20656) is instead cleared for the current text-file.
- `4`: Same as flag `3`, except for the [`TPP_FILE_FLAGS_EXTERN_C`](../src/tpp-amalgamation.h#L20659) flag.

s.a. [`TPP_HAVE_CPP_DIGIT_LINE`](config-conf.md#tpp_have_cpp_digit_line)

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
(TPP_EMITTER_HAVE_USE_CPP_DIGIT && TPP_HAVE_FILE_PUSHED_HOOK && TPP_HAVE_FILE_POPPED_HOOK && (TPP_HAVE_FILE_SYSHDR || TPP_HAVE_FILE_EXTERN_C)) ? TPP_CONF_FEAT1 : 0
```
</details>

## TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY

Extension to [`TPP_EMITTER_HAVE_USE_CPP_DIGIT`](#tpp_emitter_have_use_cpp_digit):
the first time a `# <linenum>` directive is emitted,
emit it 2 times, and in-between those to emissions,
emit another line `# <linenum> "$(PWD)//"`:

```c
# 1 "input.c"
# 1 "/opt/my-project//"
# 1 "input.c"
```

A(nother) preprocessor consuming TPP's emitter output
will then know how relative filenames can be resolved
if the intend is to turn them into absolute paths.

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_USE_CPP_DIGIT ? TPP_CONF_FEAT0 : 0
```
</details>

## TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA

Enable support for re-emission of unknown pragmas. Requires that the TPP core
is configured to allow runtime override of its [`TPP_HAVE_UNKNOWN_PRAGMA_HOOK`](config-hook.md#tpp_have_unknown_pragma_hook)
hook (since the emitter needs to be able to override that hook during its
initialization)

Can be configured in one of 3 ways:

- `0`:  Disabled (unknown pragmas cause warnings and are not (re-)emitted
- `1`:  Enabled
- `-1`: Available (but not enabled by default)

When not *Disabled*, can be turned on/off using:

- [`tpp_emitter_get_reemit_unknown_pragma()`](../src/tpp-emitter-amalgamation.h#L1403)
- [`tpp_emitter_set_reemit_unknown_pragma()`](../src/tpp-emitter-amalgamation.h#L1420)
- [`tpp_emitter_enable_reemit_unknown_pragma()`](../src/tpp-emitter-amalgamation.h#L1401)
- [`tpp_emitter_disable_reemit_unknown_pragma()`](../src/tpp-emitter-amalgamation.h#L1402)

<details><summary>Details</summary>

Default:

```c
TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) ? 1 : 0
```
</details>

## TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS

Enable support for re-emission of `#define` and `#undef` directives.
Requires that the TPP core is configured to allow runtime override of
its [`TPP_HAVE_MACRO_DEFINED_HOOK`](config-hook.md#tpp_have_macro_defined_hook) and [`TPP_HAVE_MACRO_UNDEFINED_HOOK`](config-hook.md#tpp_have_macro_undefined_hook)
hooks (since the emitter needs to be able to override these hook during
its initialization)

Can be configured in one of 3 ways:

- `0`:  Disabled
- `1`:  Enabled (`#define`/`#undef` are re-emitted)
- `-1`: Available (but not enabled by default)

When not *Disabled*, can be turned on/off using:

- [`tpp_emitter_get_reemit_macro_definitions()`](../src/tpp-emitter-amalgamation.h#L1435)
- [`tpp_emitter_set_reemit_macro_definitions()`](../src/tpp-emitter-amalgamation.h#L1486)
- [`tpp_emitter_enable_reemit_macro_definitions()`](../src/tpp-emitter-amalgamation.h#L1482)
- [`tpp_emitter_disable_reemit_macro_definitions()`](../src/tpp-emitter-amalgamation.h#L1483)

<details><summary>Details</summary>

Default:

```c
(TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK) && TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK)) ? -1 : 0
```
</details>

## TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY

Similar end result to [`TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS`](#tpp_emitter_have_reemit_macro_definitions),
but taking a completely different approach in order to get there:

- Hook [`TPP_HAVE_FILE_PUSHED_HOOK`](config-hook.md#tpp_have_file_pushed_hook) to get informed whenever a file
  is pushed onto the `#include`-stack. If that file turns out to
  be a macro, see if that macro's most-recent definition has already
  been dumped.
  - If not, or if the macro's definition has changed, dump it now.
    If there was a different definition, emit a `#undef` first.
- Whenever a [`TPP_TOK_ISKEYWORD()`](../src/tpp-amalgamation.h#L17281)-token is emitted ([`tpp_emitter_emitcurrent()`](../src/tpp-emitter-amalgamation.h#L1395)
  is called while a keyword-token is loaded into the lexer), and the
  linked keyword doesn't have a user-defined macro definition (i.e.
  `!tpp_keyword_hasmacro()`), check what was most-recently emitted
  about that keyword in regards to macro definitions:
  - If the thing that was last emitted was a `#define`-directive, then
    emit a `#undef`-directive and delete the saved macro definition.
- In order to remember the *most-recently-dumped* macro definition
  linked to a keyword, [`TPP_HAVE_KEYWORD_USERDATA`](config-core.md#tpp_have_keyword_userdata) is used to store
  a reference to the [`tpp_macro`](../src/tpp-amalgamation.h#L21798) that was most-recently dumped

NOTE: In order to determine the name of the macro when it is used
      as a result of being expanded onto the #include-stack, this
      feature also requires [`TPP_HAVE_MACRO_NAME`](config-core.md#tpp_have_macro_name) to be enabled.

Because this feature also requires a hook, it must be turned on
using the following APIs, rather than directly setting its feature:

- [`tpp_emitter_get_reemit_macro_definitions_lazy()`](../src/tpp-emitter-amalgamation.h#L1647)
- [`tpp_emitter_set_reemit_macro_definitions_lazy()`](../src/tpp-emitter-amalgamation.h#L1649)
- [`tpp_emitter_enable_reemit_macro_definitions_lazy()`](../src/tpp-emitter-amalgamation.h#L1643)
- [`tpp_emitter_disable_reemit_macro_definitions_lazy()`](../src/tpp-emitter-amalgamation.h#L1644)

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
(TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK) && TPP_HAVE_KEYWORD_USERDATA && TPP_HAVE_MACRO_NAME) ? TPP_CONF_FEAT0 : 0
```
</details>

## TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY

Extension to [`TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS`](#tpp_emitter_have_reemit_macro_definitions) and
[`TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY`](#tpp_emitter_have_reemit_macro_definitions_lazy): when emitting
`#define` directives, only emit `#define <MACRO_NAME>`, excluding
the macro's actual definition.

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS || TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) ? TPP_CONF_FEAT0 : 0
```
</details>

## TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES

Enable support for re-emission of `#include`, `#include_next`, `#import`
and `#embed` directives.

Requires that the TPP core is configured to allow runtime override of
its [`TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK`](config-hook.md#tpp_have_include_encountered_hook) hook (since the emitter needs
to be able to override that hook during its initialization)

Can be configured in one of 3 ways:

- `0`:  Disabled
- `1`:  Enabled (`#include` are re-emitted)
- `-1`: Available (but not enabled by default)

When not *Disabled*, can be turned on/off using:

- [`tpp_emitter_get_reemit_include_directives()`](../src/tpp-emitter-amalgamation.h#L1500)
- [`tpp_emitter_set_reemit_include_directives()`](../src/tpp-emitter-amalgamation.h#L1517)
- [`tpp_emitter_enable_reemit_include_directives()`](../src/tpp-emitter-amalgamation.h#L1498)
- [`tpp_emitter_disable_reemit_include_directives()`](../src/tpp-emitter-amalgamation.h#L1499)

<details><summary>Details</summary>

Default:

```c
TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) ? -1 : 0
```
</details>

## TPP_EMITTER_HAVE_TRACE_INCLUDES

Trace includes (and the depth of the `#include`-stack in terms of IO files)
by emitting a line like the following to [`tpp_lexer_gethook_mesgprinter()`](../src/tpp-amalgamation.h#L27169)
whenever an I/O file is pushed to the `#include`-stack:

```deemon
print("." * NUMBER_OF_IO_FILES_ON_INCLUDE_STACK, " ", tpp_file_getrealfilename(file));
```

Because this feature uses the [`TPP_HAVE_FILE_PUSHED_HOOK`](config-hook.md#tpp_have_file_pushed_hook) hook, it
must be turned on using the following APIs, rather than directly
setting its feature:

- [`tpp_emitter_get_trace_includes()`](../src/tpp-emitter-amalgamation.h#L1662)
- [`tpp_emitter_set_trace_includes()`](../src/tpp-emitter-amalgamation.h#L1664)
- [`tpp_emitter_enable_trace_includes()`](../src/tpp-emitter-amalgamation.h#L1658)
- [`tpp_emitter_disable_trace_includes()`](../src/tpp-emitter-amalgamation.h#L1659)

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6617), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6616), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6621): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6620): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_FILE_PUSHED_HOOK && TPP_HAVE_MESGPRINTER_HOOK) ? TPP_CONF_FEAT0 : 0
```
</details>

## TPP_EMITTER_CONFIG_LINE_THRESHOLD

The max # of blank adjustment lines before the emitter will
instead emit a(nother) `#line` (or `# <linenum>`) directive.

When negative, the threshold is configurable at runtime.
When configured as `0`, the threshold becomes infinite (at
runtime, use [`tpp_emitter_disablelinethreshold()`](../src/tpp-emitter-amalgamation.h#L1322) for this)

**Getter**: [`tpp_emitter_getlinethreshold(emitter)`](../src/tpp-emitter-amalgamation.h#L1320)<br/>
**Setter**: [`tpp_emitter_setlinethreshold(emitter, v)`](../src/tpp-emitter-amalgamation.h#L1321)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) ? -4 : 4
```
</details>

## TPP_EMITTER_HAVE_IO_PRINTPWD

Provide an API [`tpp_io_printpwd()`](../src/tpp-emitter-amalgamation.h#L948) that can be used to print the hosting
process's current working directory to a given [`tpp_formatprinter`](../src/tpp-amalgamation.h#L5891).

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY
```
</details>

## TPP_EMITTER_HAVE_CLI

Enable support for [`tpp_emitter_cli_loader`](../src/tpp-emitter-amalgamation.h#L1693)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI
```
</details>

## TPP_EMITTER_HAVE_CLI_HELP

Enable support for `tpp_emitter_cli_loader_help`, which exposes a small
database of supported commandline flags in a human-readable format that
can also be rendered (fairly) easily.

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_HAVE_CLI_HELP
```
</details>

## TPP_EMITTER_HAVE_CLI_HELP_ALL_SPELLINGS

Include extra spellings (i.e.: in addition to the primary spelling) of CLI options.

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI_HELP && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS

`-P`, `--no-line-commands`:
Disable emission of `#line`-directives, but also turn
on emission of SPACE/LF tokens (if runtime-configurable).

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_NOLINE
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_DUMP_M

`-dM`, `--dump=M`:
Dump builtin/predefined macros to the emitters output during the
CLI flush phase (i.e.: when [`tpp_emitter_cli_loader_flush()`](../src/tpp-emitter-amalgamation.h#L1801) is called):
[`tpp_lexer_dump_definitions(TPP_LEXER_DUMP_DEFINITIONS_BUILTIN_MACROS)`](../src/tpp-amalgamation.h#L29153)

Also turns on [`TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS`](#tpp_emitter_have_reemit_macro_definitions), and sets the emitter's
mode of operations to `TPP_EMITTER_MODE_DISPOSE` (see [`TPP_EMITTER_HAVE_MODE_DISPOSE`](#tpp_emitter_have_mode_dispose)).

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_HAVE_LEXER_DUMP_DEFINITIONS && TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS && TPP_EMITTER_HAVE_MODE_DISPOSE
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_DUMP_D

`-dD`, `--dump=D`:
Same as [`TPP_EMITTER_HAVE_CLI_DASH_DUMP_M`](#tpp_emitter_have_cli_dash_dump_m),
but doesn't turn on `TPP_EMITTER_MODE_DISPOSE`

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_HAVE_LEXER_DUMP_DEFINITIONS && TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_DUMP_N

`-dN`, `--dump=N`:
Same as [`TPP_EMITTER_HAVE_CLI_DASH_DUMP_D`](#tpp_emitter_have_cli_dash_dump_d), but also turns on
[`TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY`](#tpp_emitter_have_reemit_macro_definitions_name_only)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_HAVE_LEXER_DUMP_DEFINITIONS && TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS && TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_DUMP_I

`-dI`, `--dump=I`:
Turn on [`TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES`](#tpp_emitter_have_reemit_include_directives)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_DUMP_U

`-dU`, `--dump=U`:
Turn on [`TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY`](#tpp_emitter_have_reemit_macro_definitions_lazy)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES

`-H`, `--trace-includes`:
Turn on [`TPP_EMITTER_HAVE_TRACE_INCLUDES`](#tpp_emitter_have_trace_includes)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_TRACE_INCLUDES
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN

`-frelaxed-macro-column`, `-fno-relaxed-macro-column`:
Turn [`TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN`](#tpp_emitter_have_relaxed_macro_column) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA

`-freemit-unknown-pragma`, `-fno-reemit-unknown-pragma`:
Turn [`TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA`](#tpp_emitter_have_reemit_unknown_pragma) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY

`-fworking-directory`, `-fno-working-directory`:
Turn [`TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY`](#tpp_emitter_have_use_cpp_digit_working_directory) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT

`-fuse-cpp-digit`, `-fno-use-cpp-digit`:
Turn [`TPP_EMITTER_HAVE_USE_CPP_DIGIT`](#tpp_emitter_have_use_cpp_digit) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS

`-fuse-cpp-digit-flags`, `-fno-use-cpp-digit-flags`:
Turn [`TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS`](#tpp_emitter_have_use_cpp_digit_flags) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD

`-fline-threshold=COUNT`, `-fno-line-threshold`:
Configure [`TPP_EMITTER_CONFIG_LINE_THRESHOLD`](#tpp_emitter_config_line_threshold)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_EMITTER_CONFIG_LINE_THRESHOLD < 0
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE

`-fnormalize=space`, `-fno-normalize=space`:
Turn [`TPP_EMITTER_HAVE_NORMALIZE_SPACE`](#tpp_emitter_have_normalize_space) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF

`-fnormalize=lf`, `-fno-normalize=lf`:
Turn [`TPP_EMITTER_HAVE_NORMALIZE_LF`](#tpp_emitter_have_normalize_lf) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_LF)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS

`-fnormalize=strings`, `-fno-normalize=strings`:
Turn [`TPP_EMITTER_HAVE_NORMALIZE_C_STRING`](#tpp_emitter_have_normalize_c_string) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT

`-fnormalize=int`, `-fno-normalize=int`:
Turn [`TPP_EMITTER_HAVE_NORMALIZE_C_INT`](#tpp_emitter_have_normalize_c_int) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_C_INT)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS

`-fnormalize=keywords`, `-fno-normalize=keywords`:
Turn [`TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS`](#tpp_emitter_have_normalize_keywords) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE

`-fnormalize=bse`, `-fno-normalize=bse`:
Turn [`TPP_EMITTER_HAVE_NORMALIZE_BSE`](#tpp_emitter_have_normalize_bse) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_BSE)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS

`-fnormalize=trigraphs`, `-fno-normalize=trigraphs`:
Turn [`TPP_EMITTER_HAVE_NORMALIZE_BSE`](#tpp_emitter_have_normalize_bse) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS

`-fnormalize=digraphs`, `-fno-normalize=digraphs`:
Turn [`TPP_EMITTER_HAVE_NORMALIZE_BSE`](#tpp_emitter_have_normalize_bse) on/off

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE

`-fnormalize`, `-fno-normalize`:
Turn all *normalization*-features (that can be runtime-configured) on/off at once:

- [`TPP_EMITTER_HAVE_NORMALIZE_SPACE`](#tpp_emitter_have_normalize_space)
- [`TPP_EMITTER_HAVE_NORMALIZE_LF`](#tpp_emitter_have_normalize_lf)
- [`TPP_EMITTER_HAVE_NORMALIZE_C_STRING`](#tpp_emitter_have_normalize_c_string)
- [`TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS`](#tpp_emitter_have_normalize_keywords)
- [`TPP_EMITTER_HAVE_NORMALIZE_BSE`](#tpp_emitter_have_normalize_bse)
- [`TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS`](#tpp_emitter_have_normalize_trigraphs)
- [`TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS`](#tpp_emitter_have_normalize_digraphs)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && (TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_SPACE) || TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_LF) || TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) || TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) || TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_BSE) || TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) || TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS))
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT

`--mode=emit`
- Set emitter mode to `TPP_EMITTER_MODE_EMIT`
- Turn off [`TPP_EMITTER_HAVE_NOLINE`](#tpp_emitter_have_noline)
- Turn off emission of `SPACE` and `LF` tokens

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_EMIT
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE

`--mode=dispose`
- Set emitter mode to `TPP_EMITTER_MODE_DISPOSE`

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_DISPOSE
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET

`--mode=bracket`
- Set emitter mode to `TPP_EMITTER_MODE_BRACKET`
- Turn on emission of `SPACE` and `LF` tokens

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_BRACKET
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED

`--mode=typed`
- Set emitter mode to [`TPP_EMITTER_HAVE_MODE_TYPED`](#tpp_emitter_have_mode_typed)
- Turn on emission of `SPACE` and `LF` tokens

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_TYPED
```
</details>

## TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO

`--mode=zero`
- Set emitter mode to [`TPP_EMITTER_HAVE_MODE_ZERO`](#tpp_emitter_have_mode_zero)
- Turn off emission of `SPACE` and `LF` tokens

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_ZERO
```
</details>

<!--END:all-->


# Glossary

<!--BEGIN:glossary-->
[`TPP_EMITTER_HAVE_MODE_EMIT`](#tpp_emitter_have_mode_emit) [`TPP_EMITTER_HAVE_MODE_DISPOSE`](#tpp_emitter_have_mode_dispose) [`TPP_EMITTER_HAVE_MODE_BRACKET`](#tpp_emitter_have_mode_bracket) [`TPP_EMITTER_HAVE_MODE_TYPED`](#tpp_emitter_have_mode_typed) [`TPP_EMITTER_HAVE_MODE_ZERO`](#tpp_emitter_have_mode_zero) [`TPP_EMITTER_HAVE_NORMALIZE_SPACE`](#tpp_emitter_have_normalize_space) [`TPP_EMITTER_HAVE_NORMALIZE_LF`](#tpp_emitter_have_normalize_lf) [`TPP_EMITTER_HAVE_NORMALIZE_C_STRING`](#tpp_emitter_have_normalize_c_string) [`TPP_EMITTER_HAVE_NORMALIZE_C_INT`](#tpp_emitter_have_normalize_c_int) [`TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS`](#tpp_emitter_have_normalize_keywords) [`TPP_EMITTER_HAVE_NORMALIZE_BSE`](#tpp_emitter_have_normalize_bse) [`TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS`](#tpp_emitter_have_normalize_trigraphs) [`TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS`](#tpp_emitter_have_normalize_digraphs) [`TPP_EMITTER_HAVE_NOLINE`](#tpp_emitter_have_noline) [`TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN`](#tpp_emitter_have_relaxed_macro_column) [`TPP_EMITTER_HAVE_USE_CPP_DIGIT`](#tpp_emitter_have_use_cpp_digit) [`TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS`](#tpp_emitter_have_use_cpp_digit_flags) [`TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY`](#tpp_emitter_have_use_cpp_digit_working_directory) [`TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA`](#tpp_emitter_have_reemit_unknown_pragma) [`TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS`](#tpp_emitter_have_reemit_macro_definitions) [`TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY`](#tpp_emitter_have_reemit_macro_definitions_lazy) [`TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY`](#tpp_emitter_have_reemit_macro_definitions_name_only) [`TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES`](#tpp_emitter_have_reemit_include_directives) [`TPP_EMITTER_HAVE_TRACE_INCLUDES`](#tpp_emitter_have_trace_includes) [`TPP_EMITTER_CONFIG_LINE_THRESHOLD`](#tpp_emitter_config_line_threshold) [`TPP_EMITTER_HAVE_IO_PRINTPWD`](#tpp_emitter_have_io_printpwd) [`TPP_EMITTER_HAVE_CLI`](#tpp_emitter_have_cli) [`TPP_EMITTER_HAVE_CLI_HELP`](#tpp_emitter_have_cli_help) [`TPP_EMITTER_HAVE_CLI_HELP_ALL_SPELLINGS`](#tpp_emitter_have_cli_help_all_spellings) [`TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS`](#tpp_emitter_have_cli_dash_no_line_commands) [`TPP_EMITTER_HAVE_CLI_DASH_DUMP_M`](#tpp_emitter_have_cli_dash_dump_m) [`TPP_EMITTER_HAVE_CLI_DASH_DUMP_D`](#tpp_emitter_have_cli_dash_dump_d) [`TPP_EMITTER_HAVE_CLI_DASH_DUMP_N`](#tpp_emitter_have_cli_dash_dump_n) [`TPP_EMITTER_HAVE_CLI_DASH_DUMP_I`](#tpp_emitter_have_cli_dash_dump_i) [`TPP_EMITTER_HAVE_CLI_DASH_DUMP_U`](#tpp_emitter_have_cli_dash_dump_u) [`TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES`](#tpp_emitter_have_cli_dash_trace_includes) [`TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN`](#tpp_emitter_have_cli_dash_frelaxed_macro_column) [`TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA`](#tpp_emitter_have_cli_dash_freemit_unknown_pragma) [`TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY`](#tpp_emitter_have_cli_dash_fworking_directory) [`TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT`](#tpp_emitter_have_cli_dash_fuse_cpp_digit) [`TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS`](#tpp_emitter_have_cli_dash_fuse_cpp_digit_flags) [`TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD`](#tpp_emitter_have_cli_dash_line_threshold) [`TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE`](#tpp_emitter_have_cli_dash_fnormalize_space) [`TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF`](#tpp_emitter_have_cli_dash_fnormalize_lf) [`TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS`](#tpp_emitter_have_cli_dash_fnormalize_strings) [`TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT`](#tpp_emitter_have_cli_dash_fnormalize_int) [`TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS`](#tpp_emitter_have_cli_dash_fnormalize_keywords) [`TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE`](#tpp_emitter_have_cli_dash_fnormalize_bse) [`TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS`](#tpp_emitter_have_cli_dash_fnormalize_trigraphs) [`TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS`](#tpp_emitter_have_cli_dash_fnormalize_digraphs) [`TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE`](#tpp_emitter_have_cli_dash_fnormalize) [`TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT`](#tpp_emitter_have_cli_dash_mode_emit) [`TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE`](#tpp_emitter_have_cli_dash_mode_dispose) [`TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET`](#tpp_emitter_have_cli_dash_mode_bracket) [`TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED`](#tpp_emitter_have_cli_dash_mode_typed) [`TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO`](#tpp_emitter_have_cli_dash_mode_zero)
<!--END:glossary-->
