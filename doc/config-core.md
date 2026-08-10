# CORE API/System/Lexer Features

These are "core" features that cannot be enabled/disabled at runtime. They must be set/known when TPP is compiled, and mainly control which (sets of) API/runtime features should be supported.

In terms of configuration, these feature macros only have 2 states: `#if TPP_FEATURE` and `#if !TPP_FEATURE` (meaning that the only distinctly recognized integer values are `== 0` and `!= 0`)


<!--BEGIN:core-->
## TPP_IGNORE_INVALID_CONFIGURATION

When defined to non-zero, disable checks for invalid/nonsensical configurations.
Such configurations may still be able to compile, but will include definitely
redundant code, or other features that are meaningless in relation to some other
configuration.

You should only enable this if you _need_ _TPP_ _to_ _build_ _**NOW**_ and one of
those internal checks stands in your way. You should not leave this enabled, as
every one of the errors this disables is there for a reason!

<details><summary>Details</summary>

Default:

```c
0
```
</details>

## TPP_HAVE_FILE_NONBLOCK

API support for non-blocking I/O. Must also be enabled on a per-file basis
by setting the file's [`TPP_FILE_FLAGS_NONBLOCK`](../src/tpp-amalgamation.h#L19691) flag. Also: calls made to
[`tpp_lexer_yield_blocking()`](../src/tpp-amalgamation.h#L25876) & friends can be used to force calls to become
blocking, even when the associated file is confirmed as non-blocking (s.a.
[`TPP_EWOULDBLOCK`](../src/tpp-amalgamation.h#L12825) and [`TPP_TOK_EWOULDBLOCK`](../src/tpp-amalgamation.h#L14836))

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) && (TPP_OS_WINDOWS || TPP_OS_UNIX)
```
</details>

## TPP_HAVE_UNICODE

Unicode support:

- Use of a unicode traits database that is either user-supplied,
  a dummy (where all unicode character are allowed in identifiers),
  or built-in when [`TPP_HAVE_BUILTIN_CTYPE_UNICODE`](#tpp_have_builtin_ctype_unicode) is enabled.
- Automatic detection of `utf-8`, `utf-8-bom`, `utf-16[le/be]`, `utf-32[le/be]`
  in input files.<br/>
  The Lexer assumes `utf-8` by default, unless it encounters an invalid `utf-8`
  byte sequence, at which point it will emit a warning [`TPP_W_ILLEGAL_UTF8_SEQUENCE`](../src/tpp-amalgamation.h#L5010)
  (see [`TPP_HAVE_TPP_W_ILLEGAL_UTF8_SEQUENCE`](config-warn.md#tpp_have_tpp_w_illegal_utf8_sequence)) and downgrade to `ASCII`-only for
  the remainder of the file containing said invalid sequence.

<details><summary>Details</summary>

Default:

```c
1
```
</details>

## TPP_HAVE_BUILTIN_CTYPE_UNICODE

Supply a built-in unicode character traits database (adds ~21KiB data to final executable).
Some examples of stuff that is supported when this is enabled:

- `U+0085` (`NEL`)  will be treated like `U+000A` (`LF` `\n`)
- `U+00A0` (`NBSP`) will be treated like `U+0020` (`space` `\s`)
- `U+00DF` (`ß`)    will be accepted as part of identifiers

As such, TPP3 follows C23+-specifications, in that the builtin unicode
database is set-up to accept identifiers made up of `XID_Start`/`XID_Cont`.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_UNICODE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_STRERROR

Provide a function [`tpp_strerror()`](../src/tpp-amalgamation.h#L12911) to get a description of a given `tpp_errno` error code

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_EUSER

Provide an extra error code `TPP_EUSER(*)` that is never raised
by TPP internally, but may be returned by user-defined hooks and
will be propagated like any other `HARD_ERROR` error.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || !TPP_HAVE_STRERROR
```
</details>

## TPP_HAVE_STRTOKENID

Provide a function [`tpp_strtokenid()`](../src/tpp-amalgamation.h#L16351) to get the API name of a (non-keyword) token ID

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_KEYWORD_USERDATA

Enable support for storing custom user-data in keywords

**Getter**: [`tpp_keyword_getuserdata(keyword)`](../src/tpp-amalgamation.h#L21442)<br/>
**Setter**: [`tpp_keyword_setuserdata(keyword, v)`](../src/tpp-amalgamation.h#L21463)

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_KEYWORD_ASSTRING

Provide support for [`tpp_keyword_asstring()`](../src/tpp-amalgamation.h#L21435) that allows
keywords to be binary-compatible with [`tpp_string`](../src/tpp-amalgamation.h#L13354), at the
expense of adding an (otherwise unused) reference counter
field to [`tpp_keyword`](../src/tpp-amalgamation.h#L21351).

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_KEYWORD_INCLCOUNT

Include a counter for how often a specific I/O-file appears on the
`#include`-stack, with that counter being stored within its filename
keyword (used to speed up [`TPP_MAX_INCLUDE_DEPTH`](config-limit.md#tpp_max_include_depth) error checks, though
those checks also work without this feature)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) && TPP_MAX_INCLUDE_DEPTH != 0
```
</details>

## TPP_HAVE_EXTENSIONS

Enable support for runtime-configurable extensions.

s.a.:

- [`TPP_CONF_EXT0`](../src/tpp-amalgamation.h#L6360)
- [`TPP_CONF_EXT1`](../src/tpp-amalgamation.h#L6359)
- [`TPP_HAVE_MACRO___has_extension`](config-conf.md#tpp_have_macro___has_extension)
- [`TPP_HAVE_MACRO___has_known_extension`](config-conf.md#tpp_have_macro___has_known_extension)
- [`TPP_HAVE_PRAGMA_EXTENSION`](config-conf.md#tpp_have_pragma_extension)
- [`TPP_HAVE_PRAGMA_TPP_EXTENSION`](#tpp_have_pragma_tpp_extension)

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE != TPP_PROFILE_MINIMAL
```
</details>

## TPP_HAVE_EXTENSIONS_PUSH_POP

Enable support to push/pop the extension state

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_EXTENSIONS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_WARNINGS

Support for: compiler warnings (else: behave as though all warnings were being suppressed)

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE != TPP_PROFILE_MINIMAL
```
</details>

## TPP_HAVE_WARNINGS_PUSH_POP

Enable support to push/pop the warning state

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_WARNING_NUMBERS

Support for: [`tpp_warning_ofnumber()`](../src/tpp-amalgamation.h#L22182) and [`tpp_warning_getnumbers()`](../src/tpp-amalgamation.h#L22188)

When [`TPP_HAVE_PRAGMA_WARNING`](config-conf.md#tpp_have_pragma_warning) is also enabled, user-code will also
be able to configure warnings based on their ID.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_WARNING_ERROR

Support for: [`TPP_WSTATE_ERROR`](../src/tpp-amalgamation.h#L22081) (else: only [`TPP_WSTATE_FATAL`](../src/tpp-amalgamation.h#L22086) is available)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && TPP_ERROR_LIMIT != 0
```
</details>

## TPP_HAVE_WARNING_SUPPRESS

Support for: [`TPP_WSTATE_SUPPRESS`](../src/tpp-amalgamation.h#L22090)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_WARNING_DEFAULT

Support for: [`TPP_WSTATE_DEFAULT`](../src/tpp-amalgamation.h#L22095)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_FILE_NOCLOSE

Enable support for [`TPP_FILE_FLAGS_NOCLOSE`](../src/tpp-amalgamation.h#L19694)

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_FILE_NOKWD

Enable support for [`TPP_FILE_FLAGS_NOKWD`](../src/tpp-amalgamation.h#L19697)

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE != TPP_PROFILE_MINIMAL
```
</details>

## TPP_HAVE_FILE_LC_CACHE

Speed up calls to [`tpp_file_getlcinfo()`](../src/tpp-amalgamation.h#L20496) by caching the last-read
position and determining line/column information as a delta from
what was previously cached

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE != TPP_PROFILE_MINIMAL
```
</details>

## TPP_HAVE_CR_LF_DETECTION

Enable support for detecting `\r\n` as a singular line-feed

<details><summary>Details</summary>

Default:

```c
1
```
</details>

## TPP_HAVE_LEXER_COPY

Provide a function [`tpp_lexer_copy()`](../src/tpp-amalgamation.h#L25139) that can be used to duplicate a lexer.

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_LEXER_WARNING_COUNT

Lexers keep track of the # of warnings they've emitted over their lifetime. Not
actually used for anything, but can be read using [`tpp_lexer_getwarningcount()`](../src/tpp-amalgamation.h#L24343)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && (TPP_PROFILE == TPP_PROFILE_ALL)
```
</details>

## TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE

Controls if [`tpp_keyword`](../src/tpp-amalgamation.h#L21351) should have support for a runtime-override
of what `has_attribute()` should expand to when applied to that keyword.
Needed to implement `#pragma TPP has_attribute(keyword) = "expansion"`.

see:

- [`TPP_HAVE_CLANG_MACRO___has_attribute`](config-conf.md#tpp_have_clang_macro___has_attribute)
- [`TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES`](#tpp_have_pragma_tpp_keyword_features)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLANG_MACRO___has_attribute && TPP_COMMON_HAVE_KEYWORD_FEATURES
```
</details>

## TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN

Controls if [`tpp_keyword`](../src/tpp-amalgamation.h#L21351) should have support for a runtime-override
of what `has_builtin()` should expand to when applied to that keyword.
Needed to implement `#pragma TPP has_builtin(keyword) = "expansion"`.

see:

- [`TPP_HAVE_CLANG_MACRO___has_builtin`](config-conf.md#tpp_have_clang_macro___has_builtin)
- [`TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES`](#tpp_have_pragma_tpp_keyword_features)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLANG_MACRO___has_builtin && TPP_COMMON_HAVE_KEYWORD_FEATURES
```
</details>

## TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE

Controls if [`tpp_keyword`](../src/tpp-amalgamation.h#L21351) should have support for a runtime-override
of what `has_cpp_attribute()` should expand to when applied to that keyword.
Needed to implement `#pragma TPP has_cpp_attribute(keyword) = "expansion"`.

see:

- [`TPP_HAVE_CLANG_MACRO___has_cpp_attribute`](config-conf.md#tpp_have_clang_macro___has_cpp_attribute)
- [`TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES`](#tpp_have_pragma_tpp_keyword_features)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLANG_MACRO___has_cpp_attribute && TPP_COMMON_HAVE_KEYWORD_FEATURES
```
</details>

## TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE

Controls if [`tpp_keyword`](../src/tpp-amalgamation.h#L21351) should have support for a runtime-override
of what `has_declspec_attribute()` should expand to when applied to that keyword.
Needed to implement `#pragma TPP has_declspec_attribute(keyword) = "expansion"`.

see:

- [`TPP_HAVE_CLANG_MACRO___has_declspec_attribute`](config-conf.md#tpp_have_clang_macro___has_declspec_attribute)
- [`TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES`](#tpp_have_pragma_tpp_keyword_features)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLANG_MACRO___has_declspec_attribute && TPP_COMMON_HAVE_KEYWORD_FEATURES
```
</details>

## TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION

Controls if [`tpp_keyword`](../src/tpp-amalgamation.h#L21351) should have support for a runtime-override
of what `has_extension()` should expand to when applied to that keyword.
Needed to implement `#pragma TPP has_extension(keyword) = "expansion"`.

see:

- [`TPP_HAVE_CLANG_MACRO___has_extension`](config-conf.md#tpp_have_clang_macro___has_extension)
- [`TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES`](#tpp_have_pragma_tpp_keyword_features)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLANG_MACRO___has_extension && TPP_COMMON_HAVE_KEYWORD_FEATURES
```
</details>

## TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE

Controls if [`tpp_keyword`](../src/tpp-amalgamation.h#L21351) should have support for a runtime-override
of what `has_feature()` should expand to when applied to that keyword.
Needed to implement `#pragma TPP has_feature(keyword) = "expansion"`.

see:

- [`TPP_HAVE_CLANG_MACRO___has_feature`](config-conf.md#tpp_have_clang_macro___has_feature)
- [`TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES`](#tpp_have_pragma_tpp_keyword_features)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLANG_MACRO___has_feature && TPP_COMMON_HAVE_KEYWORD_FEATURES
```
</details>

## TPP_HAVE_KEYWORD_FEATURE_HAS_C_ATTRIBUTE

Controls if [`tpp_keyword`](../src/tpp-amalgamation.h#L21351) should have support for a runtime-override
of what `has_c_attribute()` should expand to when applied to that keyword.
Needed to implement `#pragma TPP has_c_attribute(keyword) = "expansion"`.

see:

- [`TPP_HAVE_CLANG_MACRO___has_c_attribute`](config-conf.md#tpp_have_clang_macro___has_c_attribute)
- [`TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES`](#tpp_have_pragma_tpp_keyword_features)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLANG_MACRO___has_c_attribute && TPP_COMMON_HAVE_KEYWORD_FEATURES
```
</details>

## TPP_HAVE_PRAGMA_TPP_WARNING

Support for: `#pragma TPP warning(...)`  (same as [`TPP_HAVE_PRAGMA_WARNING`](config-conf.md#tpp_have_pragma_warning), but doesn't require `"-fpragma-warning"`)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA_WARNING
```
</details>

## TPP_HAVE_PRAGMA_TPP_EXTENSION

Support for: `#pragma TPP extension(...)`  (same as [`TPP_HAVE_PRAGMA_EXTENSION`](config-conf.md#tpp_have_pragma_extension), but doesn't require `"-fpragma-extension"`)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA_EXTENSION
```
</details>

## TPP_HAVE_PRAGMA_TPP_TPP_EXEC

Support for: `#pragma TPP tpp_exec(...)`  (same as [`TPP_HAVE_PRAGMA_TPP_EXEC`](config-conf.md#tpp_have_pragma_tpp_exec), but doesn't require `"-fpragma-tpp-exec"`)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA_TPP_EXEC
```
</details>

## TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS

Support for: `#pragma TPP tpp_set_keyword_flags("foo", 0x7f)`  (same as [`TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS`](config-conf.md#tpp_have_pragma_tpp_set_keyword_flags), but doesn't require `"-fpragma-tpp-set-keyword-flags"`)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
```
</details>

## TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH

Support for: `#pragma TPP include_path(...)`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA && TPP_HAVE_INCLUDE_PATH
```
</details>

## TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES

Support for a number of pragma directives in the `#pragma TPP` namespace
that can be used to define custom overrides for `__has_*`-style feature-test
macros:

```c
#pragma TPP __has_attribute(feature_test_keyword) = "expansion"
#pragma TPP __has_builtin(feature_test_keyword) = "expansion"
#pragma TPP __has_cpp_attribute(feature_test_keyword) = "expansion"
#pragma TPP __has_declspec_attribute(feature_test_keyword) = "expansion"
#pragma TPP __has_extension(feature_test_keyword) = "expansion"
#pragma TPP __has_feature(feature_test_keyword) = "expansion"
#pragma TPP __has_c_attribute(feature_test_keyword) = "expansion"
```

The default feature-test expansion of some keyword can be restored
by writing `default` instead of `"expansion"` like so:

```c
#pragma TPP __has_attribute(feature_test_keyword) = default
```

Each of these resp. only works if the corresponding `TPP_HAVE_CLANG_MACRO_*`
hasn't been hard-disabled (as per [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6358)):

- `__has_attribute`: [`TPP_HAVE_CLANG_MACRO___has_attribute`](config-conf.md#tpp_have_clang_macro___has_attribute)
- `__has_builtin`: [`TPP_HAVE_CLANG_MACRO___has_builtin`](config-conf.md#tpp_have_clang_macro___has_builtin)
- `__has_cpp_attribute`: [`TPP_HAVE_CLANG_MACRO___has_cpp_attribute`](config-conf.md#tpp_have_clang_macro___has_cpp_attribute)
- `__has_declspec_attribute`: [`TPP_HAVE_CLANG_MACRO___has_declspec_attribute`](config-conf.md#tpp_have_clang_macro___has_declspec_attribute)
- `__has_extension`: [`TPP_HAVE_CLANG_MACRO___has_extension`](config-conf.md#tpp_have_clang_macro___has_extension)
- `__has_feature`: [`TPP_HAVE_CLANG_MACRO___has_feature`](config-conf.md#tpp_have_clang_macro___has_feature)
- `__has_c_attribute`: [`TPP_HAVE_CLANG_MACRO___has_c_attribute`](config-conf.md#tpp_have_clang_macro___has_c_attribute)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA && TPP_HAVE_KEYWORD_FEATURES
```
</details>

## TPP_HAVE_LEXER_OPEN_INCLUDE_STRING

Provide a function [`tpp_lexer_open_include_string()`](../src/tpp-amalgamation.h#L26088)
to open the file associated with an `#include`-string.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_MACRO___has_include || TPP_HAVE_MACRO___has_include_next || TPP_HAVE_MACRO___TPP_LOAD_FILE || TPP_HAVE_PRAGMA_GCC_DEPENDENCY
```
</details>

## TPP_HAVE_LEXER_OPEN_EMBED_STRING

Provide a function [`tpp_lexer_open_embed_string()`](../src/tpp-amalgamation.h#L26104)
to open the file associated with an `#embed`-string.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed
```
</details>

## TPP_HAVE_LEXER_DECODE_INCLUDE_STRING

Provide a function [`tpp_lexer_decode_include_string()`](../src/tpp-amalgamation.h#L26016)
to decode the actual contents of an `#include`-string.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_LEXER_OPEN_INCLUDE_STRING || TPP_HAVE_LEXER_OPEN_EMBED_STRING
```
</details>

## TPP_HAVE_LEXER_YIELD_INCLUDE_STRING

Provide a function [`tpp_lexer_yield_include_string()`](../src/tpp-amalgamation.h#L25982) to
do yield the next token with special handling if the next
token's first character is `<` or `"`in which case the
token is parsed as a `#include`-string

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_LEXER_OPEN_INCLUDE_STRING
```
</details>

## TPP_HAVE_LEXER_OPENFILE

Enable support for [`tpp_lexer_openfile()`](../src/tpp-amalgamation.h#L25260)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_OPEN_INCLUDE_STRING || TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED || 1
```
</details>

## TPP_HAVE_FILE_GETREALFILENAMEKWD

Provide a function [`tpp_file_getrealfilenamekwd()`](../src/tpp-amalgamation.h#L20543)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_PRAGMA_ONCE
```
</details>

## TPP_HAVE_IFNDEF_INCLUDE_GUARDS

Enable support for detecting `#ifndef`-style `#include`-guards.
Has no effect on semantics/behavior, but tends to speed up
processing of files with many (repeated) includes:

`foo.h`:

```c
#ifndef FOO_H
#define FOO_H
...
#endif // !FOO_H
```

`bar.h`:

```c
#include "foo.h"
```

`bar.c`:

```c
#include "bar.h" // Already includes "foo.h"
#include "foo.h" // This one's a no-op (won't even try to open("foo.h"))
#undef FOO_H
#include "foo.h" // This one once again opens "foo.h", behaving as expected
```

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT) && TPP_HAVE_CPP_IF_ELSE_ENDIF && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_USER_KEYWORDS

Enable support for TPP generating new [`tpp_keyword`](../src/tpp-amalgamation.h#L21351) definitions
on-the-fly, as keywords are parsed (the first time any unique
keyword is parsed, [`tpp_keywords_newkeyword()`](../src/tpp-amalgamation.h#L21845) is used to give
it a unique ID and [`tpp_keyword`](../src/tpp-amalgamation.h#L21351) structure). This is also needed
for macros and a number of other features that need to store some
kind of state alongside keywords.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) || TPP_HAVE_FILE_GETREALFILENAMEKWD || TPP_HAVE_CPP_MACROS || TPP_HAVE_PRAGMA_ONCE || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CLANG_MACRO___has_attribute || TPP_HAVE_CLANG_MACRO___has_builtin || TPP_HAVE_CLANG_MACRO___has_cpp_attribute || TPP_HAVE_CLANG_MACRO___has_declspec_attribute || TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_CLANG_MACRO___has_feature || TPP_HAVE_CLANG_MACRO___has_c_attribute || TPP_HAVE_MACRO___is_deprecated || TPP_HAVE_MACRO___is_poisoned || TPP_HAVE_PRAGMA_DEPRECATED || TPP_HAVE_PRAGMA_GCC_POISON || TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_IFNDEF_INCLUDE_GUARDS || TPP_HAVE_KEYWORD_INCLCOUNT || TPP_HAVE_PRAGMA_PUSH_MACRO || TPP_HAVE_MACRO___TPP_COUNTER || TPP_HAVE_KEYWORD_USERDATA
```
</details>

## TPP_HAVE_LEXER_DECODESTRING

Provide a couple of functions to decode the data contained within strings:

- [`tpp_lexer_decodestring_config`](../src/tpp-amalgamation.h#L26469)
- [`tpp_lexer_decodestring()`](../src/tpp-amalgamation.h#L26559)
- [`tpp_lexer_parsestring_ex()`](../src/tpp-amalgamation.h#L26598)
- [`tpp_lexer_parsestring()`](../src/tpp-amalgamation.h#L26614)
- [`tpp_lexer_parsestring_cb()`](../src/tpp-amalgamation.h#L26638)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TOK_STRINGLIKE
```
</details>

## TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS

Provide an optional performance-optimization flag [`TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS`](../src/tpp-amalgamation.h#L26569)
that may be passed to [`tpp_lexer_parsestring_cb()`](../src/tpp-amalgamation.h#L26638) to speed up decoding of certain kinds
of strings (by slightly weakening what callbacks are allowed to do when this flag is set)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_DECODESTRING && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_STRING_ESCAPE_BIGCHAR

Enable support for large (> 1 byte) character constants in [`tpp_lexer_decodestring()`](../src/tpp-amalgamation.h#L26559)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) && (TPP_HAVE_STRING_ESCAPE_HEX_BIG || TPP_HAVE_STRING_ESCAPE_HEX_BRACE || TPP_HAVE_STRING_ESCAPE_OCT_BRACE)
```
</details>

## TPP_HAVE_STRING_WARN_MULTILINE

Alter behavior of [`TPP_HAVE_STRING_ALLOW_MULTILINE`](config-conf.md#tpp_have_string_allow_multiline)
(that only takes effect when that feature is enabled):

- Multi-line strings continue to be allowed (does not affect behavior)
- When a multi-line string is encountered, emit a
  warning [`TPP_W_STRING_CONTINUED_AFTER_LINEFEED`](../src/tpp-amalgamation.h#L4549)
  (see [`TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED`](config-warn.md#tpp_have_tpp_w_string_continued_after_linefeed))

If this warning isn't wanted, it can be disabled by `-Wno-multiline-string`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRING_ALLOW_MULTILINE && TPP_HAVE_WARNINGS
```
</details>

## TPP_HAVE_DECODE_NAMED_ESCAPE

Enable API support for [`tpp_decode_named_escape()`](../src/tpp-amalgamation.h#L13334), which can be used to
decode the contents of `\N{...}` escape sequences. Needed to implement
[`TPP_HAVE_STRING_ESCAPE_NAMED`](config-conf.md#tpp_have_string_escape_named) and [`TPP_HAVE_IDENTIFIER_ESCAPE_NAMED`](config-conf.md#tpp_have_identifier_escape_named),
whilst implementing the rules specified by:

- [`TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES`](config-conf.md#tpp_have_escape_named_unicode_names)
- [`TPP_HAVE_ESCAPE_NAMED_UNICODE_ORD`](config-conf.md#tpp_have_escape_named_unicode_ord)
- [`TPP_HAVE_ESCAPE_NAMED_XML`](config-conf.md#tpp_have_escape_named_xml)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRING_ESCAPE_NAMED || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED
```
</details>

## TPP_HAVE_XML_ENTITY_LOOKUP

Provide a function [`tpp_xml_entity_lookup()`](../src/tpp-amalgamation.h#L13280) that can be used to lookup
a XML entity given its name. e.g. [`tpp_xml_entity_lookup("Agrave", true)`](../src/tpp-amalgamation.h#L13280)
will return `0x00C0`.

Adds ~6KiB to the final executable.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_ESCAPE_NAMED_XML || TPP_HAVE_STRING_ESCAPE_XML
```
</details>

## TPP_HAVE_UNICODE_BYNAME_LOOKUP

Provide a function [`tpp_unicode_byname_lookup()`](../src/tpp-amalgamation.h#L13301) that can
be used to lookup a unicode character given its name. e.g.
[`tpp_unicode_byname_lookup("NO-BREAK SPACE")`](../src/tpp-amalgamation.h#L13301) will return
`0x00A0`.

Enabling this feature adds a whole **~360KiB** to the final executable.
(Sorry that it's that much, but unicode defines over 35_000 names here)

Recognized names here are as defined by unicode:

- [UnicodeData.txt](https://ftp.unicode.org/Public/UNIDATA/UnicodeData.txt)
  - `\N{LATIN SMALL LETTER B}`: Basic unicode character name
- [NameAliases.txt](https://ftp.unicode.org/Public/UNIDATA/NameAliases.txt)
  - `\N{NULL}`: `control` name
  - `\N{BYTE ORDER MARK}`: `alternate` name
  - `\N{PADDING CHARACTER}`: `figment` name
  - `\N{NUL}`: `abbreviation` name
  - `\N{LATIN CAPITAL LETTER GHA}`: `correction` name
- [NamedSequences.txt](https://ftp.unicode.org/Public/UNIDATA/NamedSequences.txt)
  - `\N{LATIN CAPITAL LETTER A WITH MACRON AND GRAVE}`: multi-ordinal named sequence
- [emoji-zwj-sequences.txt](https://ftp.unicode.org/Public/UCD/latest/emoji/emoji-zwj-sequences.txt)
  - `\N{FAMILY: MAN, WOMAN, GIRL, BOY}`: emoji sequence
  - Note that due to internal limitations, emoji sequences must also be written
    in all-uppercase characters by default (even though unicode specifies that
    these sequences should be written in lowercase). If this is a problem you
    can enable [`TPP_HAVE_UNICODE_BYNAME_LOOKUP_ICASE`](config-conf.md#tpp_have_unicode_byname_lookup_icase) to ignore casing in all
    unicode names (which will include emoji sequence names)
- [emoji-sequences.txt](https://ftp.unicode.org/Public/UCD/latest/emoji/emoji-sequences.txt)
  - `\N{FLAG: GERMANY}`: emoji sequence
  - Like with `emoji-zwj-sequences.txt`, [`TPP_HAVE_UNICODE_BYNAME_LOOKUP_ICASE`](config-conf.md#tpp_have_unicode_byname_lookup_icase)
    needs to be enabled for TPP to under casings other than all-uppercase here

A few notes on the internal implementation:

- All space characters, as well as `_` are treated identically,
  and any sequence of such characters is treated the same as a
  single space ` `
- Name matching is greedy: it will always try to consume as
  much input as it can. This only becomes relevant when you
  enable other extensions which add extra meaning to `,`
  characters (`emoji-zwj-sequences.txt` already defines some
  names that contain `,` characters):
  - [`TPP_HAVE_STRING_ESCAPE_NAMED_MANY`](config-conf.md#tpp_have_string_escape_named_many)
  - [`TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY`](config-conf.md#tpp_have_identifier_escape_named_many)
- Space characters between SYMCONT-like and non-SYMCONT-like
  tokens is always optional. The following are all accepted:
  - `\N{MAN TIPPING HAND: LIGHT SKIN TONE}`
  - `\N{MAN TIPPING HAND:LIGHT SKIN TONE}`
  - `\N{MAN TIPPING HAND : LIGHT SKIN TONE}`

  The same also goes for `.`, `,`, `-`, `(` and `)`, all of
  which appear in one name or another.
- In order to save space, TPP's unicode name database detects and
  compressed ranges of *numbered* unicode characters that don't
  have dedicated names, but instead feature lists of names that
  all end with some number. Sometimes, unicode specifies that
  this number be 0-padded. However, the amount of 0-padding here
  cannot be stored in TPP's database, so any amount is accepted:
  - `\N{TANGUT COMPONENT-001}`: Unicode's listed name for `U+18800`
  - `\N{TANGUT COMPONENT-1}`: TPP also accepts this spelling...
  - `\N{TANGUT COMPONENT-0001}`: ... as well as this spelling.

Other extensions that affect the behavior of [`tpp_unicode_byname_lookup()`](../src/tpp-amalgamation.h#L13301):

- [`TPP_HAVE_UNICODE_BYNAME_LOOKUP_ICASE`](config-conf.md#tpp_have_unicode_byname_lookup_icase): Ignore casing when matching names
- [`TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE`](config-conf.md#tpp_have_unicode_byname_lookup_ispace): Space within and between tokens becomes optional

The STDC Proposal for [Named universal character escapes](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2071r2.html#extensions)
lists a couple of extensions to name lookup. All of these have been implemented by TPP:

- *Allow comma separated names*: [`TPP_HAVE_STRING_ESCAPE_NAMED_MANY`](config-conf.md#tpp_have_string_escape_named_many) and [`TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY`](config-conf.md#tpp_have_identifier_escape_named_many)
- *Allow code point numbers as names*: [`TPP_HAVE_ESCAPE_NAMED_UNICODE_ORD`](config-conf.md#tpp_have_escape_named_unicode_ord)
- *Allow names to match ISO/IEC 10646 named sequences*: enabled unconditionally (baked into name database)
- *Allow names to match Unicode emoji named sequences*: enabled unconditionally (baked into name database)
- *Allow names to match Unicode emoji ZWJ named sequences*: enabled unconditionally (baked into name database)
- *Allow names to match HTML 5 named character references*: [`TPP_HAVE_ESCAPE_NAMED_XML`](config-conf.md#tpp_have_escape_named_xml)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES
```
</details>

## TPP_HAVE_UNICODE_BYNAME_LOOKUP_ENTRY_TABLE

Extension to [`TPP_HAVE_UNICODE_BYNAME_LOOKUP`](#tpp_have_unicode_byname_lookup): use an extra
table to speed up the initial entry into the internal database
of unicode names.

Disabling this saves ~5KiB by making a (not insignificant)
sacrifice in performance related to unicode name lookup.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_UNICODE_BYNAME_LOOKUP && !defined(__OPTIMIZE_SIZE__)
```
</details>

## TPP_HAVE_FILE_SUBTEXT

Enable support for [`TPP_FILE_KIND_SUBTEXT`](../src/tpp-amalgamation.h#L19589)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO__Pragma || TPP_HAVE_MACRO___pragma || TPP_HAVE_MACRO___TPP_IDENTIFIER || TPP_HAVE_MACRO___TPP_STR_DECOMPILE || TPP_HAVE_MACRO___TPP_STR_PACK || TPP_HAVE_MACRO___TPP_COUNT_TOKENS || TPP_HAVE_PRAGMA_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_TPP_EXEC
```
</details>

## TPP_HAVE_FILE_DUMMY

Enable support for [`TPP_FILE_KIND_DUMMY`](../src/tpp-amalgamation.h#L19595), which is
needed to support gcc's `# <linenum>` -> `1`/`2` flags

These flags push so-called *dummy* files onto the
`#include`-stack (without altering the actual current
file), with those dummy files acting as additional
entries for `#include` tracebacks.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIGIT_LINE
```
</details>

## TPP_HAVE_INCLUDE_STACK

Enable API support for having an `#include`-stack. Despite the name,
this is also needed for macro expansion (see [`TPP_HAVE_CPP_MACROS`](config-conf.md#tpp_have_cpp_macros))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED || TPP_HAVE_FILE_SUBTEXT || TPP_HAVE_FILE_DUMMY
```
</details>

## TPP_HAVE_HOOK_COOKIES

API hooks have user-configurable cookies. When not otherwise configured,
(such as when a default user-defined hook is defined, or when the non-
extended hook setter (that doesn't take a cookie argument) is used), then
the lexer *itself* will be passed as cookie

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE != TPP_PROFILE_MINIMAL
```
</details>

## TPP_HAVE_RT_FILE_AND_LINE_FORMAT

Allow the file-and-line format used by warning to be overwritten on a per-lexer basis.

When this is disabled, [`TPP_CONFIG_FILE_AND_LINE_FORMAT`](config-value.md#tpp_config_file_and_line_format) is always used instead.

**Getter**: [`tpp_lexer_getfileandlineformat(lexer)`](../src/tpp-amalgamation.h#L24427)<br/>
**Setter**: [`tpp_lexer_setfileandlineformat(lexer, format)`](../src/tpp-amalgamation.h#L24428)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && (TPP_HAVE_WARNINGS || (TPP_HAVE_PRAGMA_MESSAGE && TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION))
```
</details>

## TPP_HAVE_QUALITY_WARNINGS

General config for `-Wquality` warnings. When overwritten
to `0`, all `-Wquality` warnings will be disabled.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_TPP_UNICODE_WRITEUTF8

Provide an API function [`tpp_unicode_writeutf8()`](../src/tpp-amalgamation.h#L13226)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TOK_C_STRING || TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TOK_C_CHAR || TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL || TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL || TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL || TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL || TPP_HAVE_TOK_BLOCK_CHAR_LITERAL || TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_DECODE_NAMED_ESCAPE
```
</details>

## TPP_HAVE_TIME_API

Provide an API for loading the current date/time

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_MACRO___TIME__ || TPP_HAVE_MACRO___DATE__ || TPP_HAVE_MACRO___TIMESTAMP__ || TPP_HAVE_NUMERIC_DATE_MACROS || TPP_HAVE_NUMERIC_TIME_MACROS
```
</details>

## TPP_HAVE_FILE_SYSHDR

Enable support for [`TPP_FILE_FLAGS_SYSHDR`](../src/tpp-amalgamation.h#L19703)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER || TPP_HAVE_CPP_DIGIT_LINE
```
</details>

## TPP_HAVE_FILE_EXTERN_C

Enable support for [`TPP_FILE_FLAGS_EXTERN_C`](../src/tpp-amalgamation.h#L19706)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_CPP_DIGIT_LINE
```
</details>

## TPP_HAVE_INCLUDE_PATH

Support for: custom string list describing the available
`-I/usr/include`-style -> `#include <foo.h>`-paths

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_STACK
```
</details>

## TPP_HAVE_INCLUDE_PATH_QUOTE

[`tpp_include_paths`](../src/tpp-amalgamation.h#L22562) contains a 2nd path-list that is only searched during `"`-strings

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_STACK && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_INCLUDE_PATH_SYSHDR

[`tpp_include_paths`](../src/tpp-amalgamation.h#L22562) contains a 3rd path-list whose files are treated as [`TPP_FILE_FLAGS_SYSHDR`](../src/tpp-amalgamation.h#L19703)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_STACK && TPP_HAVE_FILE_SYSHDR && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_INCLUDE_PATH_AFTER

[`tpp_include_paths`](../src/tpp-amalgamation.h#L22562) contains a 4th path-list that is searched after all other paths

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_STACK && ((TPP_PROFILE == TPP_PROFILE_ALL) || ((TPP_PROFILE != TPP_PROFILE_MINIMAL) && TPP_HAVE_INCLUDE_SYSTEM_INCLUDE_PATH))
```
</details>

## TPP_HAVE_INCLUDE_PATH_EMBED

Add another `#include`-path list specifically for `#embed` and `__has_embed`. This list
is used for filenames specified in `#embed <file>` and `#embed "file"`, whereas use
of `#embed "file"` will also try to open relative to the current file.

When this feature is disabled, trying to opening a `#embed <file>` always fails, and
`#embed "file"` will only find files relative to the directory containing the current
input file.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_PATH && (TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed) && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_INCLUDE_PATH_PUSH_POP

Enable support to push/pop the `#include`-path state

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_PATH && (TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH || (TPP_PROFILE == TPP_PROFILE_ALL))
```
</details>

## TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING

Use `TPP_REF tpp_string *` instead of `char *` in `#include`-path lists.
Doing so greatly reduces the overhead when `#include`-path lists are
pushed/popped, since [`tpp_string`](../src/tpp-amalgamation.h#L13354) can be incref'd, whereas `char`
must be hard-copied.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_PATH_PUSH_POP
```
</details>

## TPP_HAVE_LEXER_SEEKPP_RPAREN

Provide a function [`tpp_lexer_seekpp_rparen()`](../src/tpp-amalgamation.h#L26278) that can be used
to find the position of a matching `)`-token for the purpose
of macro argument lists.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS
```
</details>

## TPP_HAVE_LEXER_SEEKPP_RPAREN_EX

Provide a function [`tpp_lexer_seekpp_rparen_ex()`](../src/tpp-amalgamation.h#L26269) that is pretty much
the same as [`tpp_lexer_seekpp_rparen()`](../src/tpp-amalgamation.h#L26278), but is also able to deal with
alternate parenthesis pairs: `[ ]` `{ }` `< >` in addition to `( )`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_SEEKPP_RPAREN && TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
```
</details>

## TPP_HAVE_IFDEF_STACK

Enable support for [`tpp_file`](../src/tpp-amalgamation.h#L19798) keeping track of the state of active `#ifdef` directives
via an embedded [`tpp_ifdef_stack`](../src/tpp-amalgamation.h#L19740) strcture (accessible via [`tpp_file_getifdef()`](../src/tpp-amalgamation.h#L19977))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_IF_ELSE_ENDIF
```
</details>

## TPP_HAVE_FILE_SETFILENAME

Enable support for [`tpp_file_setfilename()`](../src/tpp-amalgamation.h#L20570)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_CPP_DIGIT_LINE || TPP_HAVE_CPP_LINE
```
</details>

## TPP_HAVE_FILE_SETLINE

Enable support for [`tpp_file_setline()`](../src/tpp-amalgamation.h#L20584)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_CPP_DIGIT_LINE || TPP_HAVE_CPP_LINE
```
</details>

## TPP_HAVE_FILE_KEEPPOS

Enable support for setting a special pointer in files to
represent a lower bound of memory that must be kept loaded.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_CPP_MACROS
```
</details>

## TPP_HAVE_FILE_MACRO_TRACKARGS

Keep track of the original [`tpp_lexer_arginfo`](../src/tpp-amalgamation.h#L26196) used during macro invocation,
in order to improve [`tpp_file_getlcinfo_ex()`](../src/tpp-amalgamation.h#L20642)'s `tlcix_proj*` return values,
by making them less error-prone.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS && TPP_HAVE_LEXER_SEEKPP_RPAREN && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_FILE_ENCODING_EMBED

Provide a special [`TPP_FILE_ENCODING_EMBED`](../src/tpp-amalgamation.h#L19611) file encoding
to convert bytes into `,`-separated decimals on-the-fly.

NOTE: Even when this is disabled, `#embed` directives work,
      but will have to pre-load the entire input file into
      memory, rather than allowing the file to be streamed
      by converting it on-the-fly.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_UNICODE && TPP_HAVE_CPP_EMBED && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_LEXER_RAND

Provide an API [`tpp_lexer_nextrand()`](../src/tpp-amalgamation.h#L25527) that can be used to generate random
numbers using the already-parsed contents of of I/O and TEXT files as seed.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_MACRO___TPP_RANDOM
```
</details>

## TPP_HAVE_FILE_GETHASH

Provide an API [`tpp_file_gethash()`](../src/tpp-amalgamation.h#L20597) that can be used to calculate a hash
of all already-processed bytes within a file up to a given position.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_LEXER_RAND
```
</details>

## TPP_HAVE_FILE_GETFULLHASH

Provide an API [`tpp_file_getfullhash()`](../src/tpp-amalgamation.h#L20607) that behaves similar to [`tpp_file_gethash()`](../src/tpp-amalgamation.h#L20597),
but also traverses the `#include`-stack to include the hash values of all parent files
found therein, such that the return values describes not only the current file leading
up to a given position, but also all other files that came before.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_FILE_GETHASH && TPP_HAVE_INCLUDE_STACK
```
</details>

## TPP_HAVE_ESCAPED_KEYWORDS

Provide a secondary set of keyword APIs that include support for `\`-escape sequences.
Needed to implement [`TPP_HAVE_BSE`](config-conf.md#tpp_have_bse), [`TPP_HAVE_IDENTIFIER_ESCAPE_UNI`](config-conf.md#tpp_have_identifier_escape_uni) and [`TPP_HAVE_IDENTIFIER_ESCAPE_NAMED`](config-conf.md#tpp_have_identifier_escape_named)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BSE || TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED
```
</details>

## TPP_HAVE_IO_COMPARE_MTIME

Enable support for [`tpp_io_compare_mtime()`](../src/tpp-amalgamation.h#L13639).
This is the underlying system function needed for
`#pragma GCC dependency` (see [`TPP_HAVE_PRAGMA_GCC_DEPENDENCY`](config-conf.md#tpp_have_pragma_gcc_dependency))

<details><summary>Details</summary>

Default:

```c
((TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_PRAGMA_GCC_DEPENDENCY) && (defined(tpp_io_compare_mtime) || (TPP_OS_WINDOWS || TPP_OS_UNIX))
```
</details>

## TPP_HAVE_IO_SKIP_BLOCKING



<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_CPP_EMBED_OFFSET
```
</details>

## TPP_HAVE_IO_NORMALIZE_FILENAME

Enable support for [`tpp_io_normalize_filename()`](../src/tpp-amalgamation.h#L13688).
This function is needed to detect+fix (see [`TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING`](config-warn.md#tpp_have_tpp_w_nonportable_filename_casing))
incorrect casing in `#include`-paths on host platforms with case-insensitive filenames
(i.e.: windows).

In particular, this function is necessary such that `#pragma once` in a header file
`<stdio.h>` will still prevent multiple inclusion if that file is included multiple
times with different path casing:

```c
#include <stdio.h>
#include <STDIO.H> // `TPP_HAVE_IO_NORMALIZE_FILENAME` saves the day
```

<details><summary>Details</summary>

Default:

```c
TPP_OS_WINDOWS && (TPP_HAVE_USER_KEYWORDS || (TPP_PROFILE != TPP_PROFILE_MINIMAL))
```
</details>

## TPP_HAVE_JOINPATH

Enable support for [`tpp_joinpath()`](../src/tpp-amalgamation.h#L25448), a wrapper around another internal function
used to implement [`tpp_lexer_openfile()`](../src/tpp-amalgamation.h#L25260) (see [`TPP_HAVE_LEXER_OPENFILE`](#tpp_have_lexer_openfile)) and is also
needed to implement `#pragma GCC dependency` (see [`TPP_HAVE_PRAGMA_GCC_DEPENDENCY`](config-conf.md#tpp_have_pragma_gcc_dependency))

This function takes 2 paths, resolves `.` and `..` path references between them,
whilst pasting them onto each other in order to form a singular, normalized path

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_PRAGMA_GCC_DEPENDENCY || TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH || (TPP_HAVE_CLI_DASH_IPREFIX && (TPP_HAVE_CLI_DASH_IWITHPREFIX || TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE))
```
</details>

## TPP_HAVE_STRING_BUILDER

Provide an API [`tpp_string_builder`](../src/tpp-amalgamation.h#L13429) centered around building [`tpp_string`](../src/tpp-amalgamation.h#L13354)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_LEXER_DECODESTRING || TPP_HAVE_LEXER_SEEKPP_RPAREN || (TPP_HAVE_LEXER_DECODE_INCLUDE_STRING && (TPP_CONF_MAYBE_1(TPP_HAVE_BSE) || TPP_CONF_MAYBE_1(TPP_HAVE_TRIGRAPHS))) || TPP_HAVE_MACRO___FILE__ || TPP_HAVE_MACRO___BASE_FILE__ || TPP_HAVE_MACRO___FILE_NAME__ || TPP_HAVE_MACRO___TPP_EVAL || TPP_HAVE_MACRO___TPP_STR_PACK || TPP_HAVE_MACRO___TPP_EXEC || TPP_HAVE_MACRO___TPP_STR_SUBSTR || TPP_HAVE_MACRO___TPP_LOAD_FILE || TPP_HAVE_CPP_EMBED
```
</details>

## TPP_HAVE_STRING_BUILDER_TRYALLOC

Provide a function [`tpp_string_builder_tryalloc()`](../src/tpp-amalgamation.h#L13468)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_MACRO___TPP_LOAD_FILE
```
</details>

## TPP_HAVE_LEXER_SKIP

Enable support for [`tpp_lexer_skip()`](../src/tpp-amalgamation.h#L26128) and [`tpp_lexer_require()`](../src/tpp-amalgamation.h#L26142)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) || TPP_HAVE_PRAGMA_PUSH_MACRO || TPP_HAVE_PRAGMA_DEPRECATED || TPP_HAVE_PRAGMA_MESSAGE || TPP_HAVE_PRAGMA_ERROR || TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_WARNING || TPP_HAVE_PRAGMA_TPP_EXTENSION || TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH || TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES || TPP_HAVE_MACRO__Pragma || TPP_HAVE_MACRO___TPP_EVAL || TPP_HAVE_MACRO___has_include || TPP_HAVE_MACRO___has_include_next || TPP_HAVE_MACRO___has_embed || TPP_HAVE_MACRO___TPP_COUNT_TOKENS || TPP_HAVE_MACRO___TPP_STR_SIZE || TPP_HAVE_MACRO___TPP_STR_PACK || TPP_HAVE_MACRO___TPP_RANDOM || TPP_HAVE_MACRO___TPP_STR_SUBSTR || TPP_HAVE_CPP_ASSERT || TPP_HAVE_CPP_EMBED || TPP_HAVE_BUILTIN_PARSEEXPR_HOOK
```
</details>

## TPP_HAVE_LEXER_TIME

Enable support for storing a time value in [`tpp_lexer`](../src/tpp-amalgamation.h#L24275)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TIME_API
```
</details>

## TPP_HAVE_LEXER_TRYSKIP_RAW

Enable support for [`tpp_lexer_tryskip_raw()`](../src/tpp-amalgamation.h#L26175), a function that is used-
and needed in order to seek- and skip-over the `(` token following a
macro's name (with support for searching for tokens in parent files
of the current one, but rolling back all changes if the next token isn't
as expected). It also offers a flag [`TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV`](../src/tpp-amalgamation.h#L26151)
that will retain the `[tpp_file_getlastpos(),*)` regions of files as
they are scanned, allowing it to keep the previous token (which is
probably the name of a macro) loaded in memory.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_CPP_MACROS
```
</details>

## TPP_HAVE_LEXER_REPRTOKENID

Provide a function [`tpp_lexer_reprtokenid()`](../src/tpp-amalgamation.h#L26843) to
return the string-representation of a given token ID

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_SKIP
```
</details>

## TPP_HAVE_REPRTOKENID

Provide a function [`tpp_reprtokenid()`](../src/tpp-amalgamation.h#L16360) that does the
same as [`tpp_lexer_reprtokenid()`](../src/tpp-amalgamation.h#L26843) (and is also used to
implement that function), but fails for user-defined
keyword token IDs

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_REPRTOKENID
```
</details>

## TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS

Provide a lexer state flag [`TPP_LEXER_STATE_FLAG_ALLTOKENS`](../src/tpp-amalgamation.h#L24256) that forces
[`tpp_lexer_yieldpp()`](../src/tpp-amalgamation.h#L25851) to always re-emit *all* tokens (rather than skip
over space/lf/comment tokens based on [`TPP_HAVE_TOK_SPACE`](config-conf.md#tpp_have_tok_space),
[`TPP_HAVE_TOK_LF`](config-conf.md#tpp_have_tok_lf) and [`TPP_HAVE_TOK_COMMENT`](config-conf.md#tpp_have_tok_comment))

This flag is also needed internally when TPP needs to expand
the arguments supplied to a user-defined macro

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS && (TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SPACE) || TPP_CONF_MAYBE_0(TPP_HAVE_TOK_LF) || TPP_CONF_MAYBE_0(TPP_HAVE_TOK_COMMENT))
```
</details>

## TPP_HAVE_LEXER_GETKEYWORDFEATURE

Provide a function [`tpp_lexer_getkeywordfeature()`](../src/tpp-amalgamation.h#L26351) to determine
what a given keyword should expand to within a `__has_*`-context.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLANG_MACRO___has_attribute || TPP_HAVE_CLANG_MACRO___has_builtin || TPP_HAVE_CLANG_MACRO___has_cpp_attribute || TPP_HAVE_CLANG_MACRO___has_declspec_attribute || TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_CLANG_MACRO___has_feature || TPP_HAVE_CLANG_MACRO___has_c_attribute
```
</details>

## TPP_HAVE_LEXER_GETKEYWORDDEFINED

Provide a function [`tpp_lexer_getkeyworddefined()`](../src/tpp-amalgamation.h#L26375) to check
if a given keyword is `defined()` (meaning it can be expanded
as a (potentially builtin) macro)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_CPP_MACROS || TPP_HAVE_BUILTIN_EXPR_DEFINED
```
</details>

## TPP_HAVE_LEXER_ISIDENTIFIER

Provide a function [`tpp_lexer_isidentifier()`](../src/tpp-amalgamation.h#L26391) to check if
a given keyword is considered to be an `__is_identifier()`

When that is the case, `__is_identifier()` (s.a. [`TPP_HAVE_MACRO___is_identifier`](config-conf.md#tpp_have_macro___is_identifier))
expands to `1` (rather than `0`) for that keyword, and the user attempting to
define a macro of the same name triggers a `-Wkeyword-macro` warning (s.a.
[`TPP_HAVE_TPP_W_MACRO_NAME_IS_IDENTIFIER`](config-warn.md#tpp_have_tpp_w_macro_name_is_identifier))

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_MACRO___is_identifier || TPP_HAVE_TPP_W_MACRO_NAME_IS_IDENTIFIER
```
</details>

## TPP_HAVE_LEXER_ISIDENTIFIER_DEFAULT

Default return value for [`tpp_lexer_isidentifier()`](../src/tpp-amalgamation.h#L26391) (see [`TPP_HAVE_LEXER_ISIDENTIFIER`](#tpp_have_lexer_isidentifier))
for keywords where this property hasn't been explicitly defined by [`TPP_KWD_IS_IDENTIFIER()`](../src/tpp-amalgamation.h#L243),
and also don't have macro expansions as per [`TPP_PREDEFINED_MACRO()`](../src/tpp-amalgamation.h#L272).

This only affects additional keywords that you've defined. All of TPP's builtin keywords
come pre-configured with sensible defaults, though those defaults can also be overwritten
on a per-keyword basis by pre-defining a macro `TPP_KWDIDENTIFIER_<ident>` to `0` or `1`:

```c
#define TPP_KWDIDENTIFIER_if 0  // Causes `__has_identifier(if)` to expand to `0`
```

<details><summary>Details</summary>

Default:

```c
0
```
</details>

## TPP_HAVE_LEXER_MANUALPOPFILE

Provide a set of macros/functions `tpp_lexer_manualpopfile_*`
that can be used to seek through the contents of files further
up the `#include`-stack in a way that allows for rollback.

- [`tpp_lexer_manualpopfile_start()`](../src/tpp-amalgamation.h#L25812)
- [`tpp_lexer_manualpopfile_popfile()`](../src/tpp-amalgamation.h#L25814)
- [`tpp_lexer_manualpopfile_canpopfile()`](../src/tpp-amalgamation.h#L25815)
- [`tpp_lexer_manualpopfile_break_rollback()`](../src/tpp-amalgamation.h#L25818)
- [`tpp_lexer_manualpopfile_break_commit()`](../src/tpp-amalgamation.h#L25819)
- [`tpp_lexer_manualpopfile_break()`](../src/tpp-amalgamation.h#L25820)
- [`tpp_lexer_manualpopfile_end_rollback()`](../src/tpp-amalgamation.h#L25824)
- [`tpp_lexer_manualpopfile_end_commit()`](../src/tpp-amalgamation.h#L25827)
- [`tpp_lexer_manualpopfile_end()`](../src/tpp-amalgamation.h#L25830)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_STACK && (TPP_HAVE_CPP_MACROS || TPP_HAVE_LEXER_TRYSKIP_RAW || TPP_HAVE_LEXER_SKIP)
```
</details>

## TPP_HAVE_MACRO_EQUALS

Provide a function [`tpp_macro_equals()`](../src/tpp-amalgamation.h#L20912) to compare macro definitions.
Because the C standard specifies that re-defining a macro should only
be worthy of a warning if its new definition differs from its old one,
this function is used to only empty [`TPP_W_REDEFINE_MACRO`](../src/tpp-amalgamation.h#L4977) if exactly
that happened.

```c
#define foo 10
#define foo 10  // Don't warn here (not even because of this comment)
#define foo 11  // But *do* warn here!
```

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TPP_W_REDEFINE_MACRO
```
</details>

## TPP_HAVE_MACRO_NAME

Add an extra field to [`tpp_macro`](../src/tpp-amalgamation.h#L20845) for the [`tpp_keyword`](../src/tpp-amalgamation.h#L21351) describing
the macro's name. Other than allowing for some slight improvements
to error messages (and allowing the *EMITTER* amalgamation to
implement `TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY`), this
being enabled/disabled doesn't have any effects on semantics.

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE != TPP_PROFILE_MINIMAL
```
</details>

## TPP_HAVE_EXPR_VALUE_PRINTREPR

Provide a function [`tpp_expr_value_printrepr()`](../src/tpp-amalgamation.h#L13978) to construct the result
of `__TPP_EVAL` (see [`TPP_HAVE_MACRO___TPP_EVAL`](config-conf.md#tpp_have_macro___tpp_eval))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_MACRO___TPP_EVAL
```
</details>

## TPP_HAVE_LEXER_DUMP_DEFINITIONS

Provide a function [`tpp_lexer_dump_definitions()`](../src/tpp-amalgamation.h#L26854) that can be
used to re-print all user-defined macro definitions and asserts.

This can be used to implement GCC's CPP's `-dM` CLI flag.

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_LEXER_DUMP_DEFINITIONS_SORTED

Extension to [`TPP_HAVE_LEXER_DUMP_DEFINITIONS`](#tpp_have_lexer_dump_definitions): provide an
additional flag [`TPP_LEXER_DUMP_DEFINITIONS_SORTED`](../src/tpp-amalgamation.h#L26895) that causes
macro ([`TPP_HAVE_CPP_MACROS`](config-conf.md#tpp_have_cpp_macros)) / assert ([`TPP_HAVE_CPP_ASSERT`](config-conf.md#tpp_have_cpp_assert))
definition to be sorted based on the ID of the associated keyword
(which corresponds with the first time the associated keyword was
seen, which also usually lines up with the order in which macros
were defined)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_DUMP_DEFINITIONS
```
</details>

## TPP_HAVE_LEXER_DUMP_DEFINITIONS_EXTRAINFO

Extension to [`TPP_HAVE_LEXER_DUMP_DEFINITIONS`](#tpp_have_lexer_dump_definitions): provide an
additional flag [`TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO`](../src/tpp-amalgamation.h#L26900) that causes every
dumped macro definition to be preceded by a comment containing (among
other things) the file/line/column where that definition comes from.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_DUMP_DEFINITIONS
```
</details>

## TPP_HAVE_TOKEN_ENCODESTRING

Provide a function [`tpp_token_encodestring()`](../src/tpp-amalgamation.h#L16436) to perform `\`-escaping of arbitrary
data, such that [`tpp_lexer_yieldraw()`](../src/tpp-amalgamation.h#L25654) and [`tpp_lexer_decodestring()`](../src/tpp-amalgamation.h#L26559) will understand
the encoded representation in such a way that the original data is reproducible.

This is primarily needed to implement stuff like `#define STR(x) #x`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT || (TPP_HAVE_EXPR_VALUE_PRINTREPR && TPP_HAVE_BUILTIN_EXPR_STRINGS) || TPP_HAVE_MACRO___FILE__ || TPP_HAVE_MACRO___BASE_FILE__ || TPP_HAVE_MACRO___FILE_NAME__ || TPP_HAVE_MACRO___TPP_STR_PACK || TPP_HAVE_MACRO___TPP_STR_SUBSTR || (TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH && TPP_HAVE_LEXER_DUMP_DEFINITIONS)
```
</details>

## TPP_HAVE_LEXER_REQUIRE_WHITESPACE

Provide a function [`tpp_lexer_require_whitespace()`](../src/tpp-amalgamation.h#L25541) to check if 2 tokens,
when written directly adjacent to each other, *might* produce a different
(set of) token(s) when re-parsed.

This function is used to implement [`TPP_HAVE_MAGIC_WHITESPACE`](config-conf.md#tpp_have_magic_whitespace), which in
turn is needed to inject additional whitespace when failure to do so could
result in accidental token concatenation during reparsing.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_MAGIC_WHITESPACE
```
</details>

## TPP_HAVE_LEXER_DECODEINT_EXPR

Provide a function [`tpp_lexer_decodeint_expr()`](../src/tpp-amalgamation.h#L26433) to parse an integer into a [`tpp_expr_value`](../src/tpp-amalgamation.h#L13772)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_TOK_C_INT
```
</details>

## TPP_HAVE_LEXER_PARSEEMBED

Provide a function [`tpp_lexer_parseembed()`](../src/tpp-amalgamation.h#L25949) to quickly parse ,-separated sequences
of integer tokens with values in range [0,0xff]. Extra optimization is done if the
current file turns out to be the result of a `#embed` directive, in which case the
data doesn't need to be converted to decimals (if [`TPP_HAVE_FILE_ENCODING_EMBED`](#tpp_have_file_encoding_embed) is
also enabled)

<details><summary>Details</summary>

Default:

```c
((TPP_PROFILE == TPP_PROFILE_ALL) || (TPP_HAVE_MACRO___TPP_STR_PACK && TPP_HAVE_FILE_ENCODING_EMBED)) && TPP_HAVE_TOK_INT
```
</details>

## TPP_HAVE_LEXER_DECODEINT

Provide a function [`tpp_lexer_decodeint()`](../src/tpp-amalgamation.h#L26422) to parse an integer

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TOK_INT && (TPP_HAVE_LEXER_DECODEINT_EXPR || TPP_HAVE_LEXER_PARSEEMBED || TPP_HAVE_CPP_LINE || TPP_HAVE_CPP_DIGIT_LINE || TPP_HAVE_MACRO___TPP_RANDOM || TPP_HAVE_MACRO___TPP_STR_SUBSTR || TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING)
```
</details>

## TPP_HAVE_LEXER_DECODEFLOAT_EXPR

Provide a function [`tpp_lexer_decodefloat_expr()`](../src/tpp-amalgamation.h#L26460) to parse a float into a [`tpp_expr_value`](../src/tpp-amalgamation.h#L13772)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_BUILTIN_EXPR_FLOATS && TPP_HAVE_TOK_FLOAT
```
</details>

## TPP_HAVE_LEXER_DECODEFLOAT

Provide a function [`tpp_lexer_decodefloat()`](../src/tpp-amalgamation.h#L26449) to parse a float

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_DECODEFLOAT_EXPR
```
</details>

## TPP_HAVE_LEXER_PARSECHARACTER_LITERAL

Provide a function [`tpp_lexer_parsecharacter_literal()`](../src/tpp-amalgamation.h#L26656) to parse character literals

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
```
</details>

## TPP_HAVE_LEXER_PARSESTRING_EXPR

Provide a function [`tpp_lexer_parsestring_expr()`](../src/tpp-amalgamation.h#L26675) to parse a string into a [`tpp_expr_value`](../src/tpp-amalgamation.h#L13772)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_BUILTIN_EXPR_STRINGS && TPP_HAVE_LEXER_DECODESTRING
```
</details>

## TPP_HAVE_LEXER_PARSECHARACTER_EXPR

Provide a function [`tpp_lexer_parsecharacter_expr()`](../src/tpp-amalgamation.h#L26697) to parse a character literal into a [`tpp_expr_value`](../src/tpp-amalgamation.h#L13772)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS && TPP_HAVE_LEXER_DECODESTRING
```
</details>

## TPP_HAVE_FTOA

Provide a function [`tpp_ftoa()`](../src/tpp-amalgamation.h#L13210) to convert a float into a string

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_EXPR_VALUE_PRINTREPR
```
</details>

## TPP_HAVE_TPP_EXTENSION_NEAREST

Provide a function [`tpp_extension_nearest()`](../src/tpp-amalgamation.h#L22048) that does fuzzy matching to find
the closest match of a given extension name. Used in the warning message emitted
when trying to set an unknown extension: [`TPP_HAVE_TPP_W_UNKNOWN_EXTENSION`](config-warn.md#tpp_have_tpp_w_unknown_extension)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TPP_W_UNKNOWN_EXTENSION && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_TPP_WARNING_GROUP_NEAREST

Provide a function [`tpp_warning_group_nearest()`](../src/tpp-amalgamation.h#L22150) that does fuzzy matching to find
the closest match of a given warning name. Used in the warning message emitted
when trying to set an unknown warning: [`TPP_HAVE_TPP_W_UNKNOWN_WARNING`](config-warn.md#tpp_have_tpp_w_unknown_warning)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TPP_W_UNKNOWN_WARNING && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_TPP_FUZZY_MEMCMP

Provide a function [`tpp_fuzzy_memcmp()`](../src/tpp-amalgamation.h#L13242) to quantify the
*fuzziness* of how close 2 memory-blocks are to each other.

Needed to implement [`TPP_HAVE_TPP_EXTENSION_NEAREST`](#tpp_have_tpp_extension_nearest) and
[`TPP_HAVE_TPP_WARNING_GROUP_NEAREST`](#tpp_have_tpp_warning_group_nearest).

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TPP_EXTENSION_NEAREST || TPP_HAVE_TPP_WARNING_GROUP_NEAREST
```
</details>

## TPP_HAVE_API_TOKEN_NAMES_IN_GLOBAL_NAMESPACE

Every token/keyword that TPP needs to pre-define for one reason or another
is defined as an enum in `tpp_token_id` under the name `TPP_KWD_<keyword>`
for keywords and `TPP_TOK_<DESCRIPTION>` for tokens.
For the sake of making it easier to determine which keywords have been
pre-defined by TPP, every `TPP_KWD_<keyword>` is also defined as a macro
expanding to itself.

When this feature is enabled, every `TPP_KWD_<keyword>` and `TPP_TOK_<DESCRIPTION>`
is accompanied by another macro, exposing keyword and token identifiers within
the global namespace, similar to what TPP2 used to do:

```c
#define KWD_<keyword>     TPP_KWD_<keyword>`
#define TOK_<DESCRIPTION> TPP_TOK_<DESCRIPTION>`
```

This option does *NOT* affect user-defined keywords. Those are always defined
as `<name>` just the way you wrote them using [`TPP_KWD(<name>, "<repr>")`](../src/tpp-amalgamation.h#L240).

<details><summary>Details</summary>

Default:

```c
0
```
</details>

## TPP_HAVE_KEYWORDS_UNDEFALLUSER

Provide a function [`tpp_keywords_undefalluser()`](../src/tpp-amalgamation.h#L21885) + [`tpp_lexer_undefalluser()`](../src/tpp-amalgamation.h#L25477)
that can be used to quickly delete *all* macro definitions.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_CPP_MACROS
```
</details>

## TPP_HAVE_KEYWORDS_UNASSERTALL

Provide a function [`tpp_keywords_unassertall()`](../src/tpp-amalgamation.h#L21896) + [`tpp_lexer_unassertall2()`](../src/tpp-amalgamation.h#L25509)
that can be used to quickly delete *all* keyword assertions.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_CPP_ASSERT
```
</details>

## TPP_HAVE_KEYWORDS_RESETFLAGS

Provide a function [`tpp_keywords_resetflags()`](../src/tpp-amalgamation.h#L21907) + [`tpp_lexer_kwds_resetflags()`](../src/tpp-amalgamation.h#L24604)
that can be used to delete the flags of all keywords.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_KEYWORD_FLAGS
```
</details>

## TPP_HAVE_KEYWORDS_RESETFEATURES

Provide a function [`tpp_keywords_resetfeatures()`](../src/tpp-amalgamation.h#L21918) + [`tpp_lexer_kwds_resetfeatures()`](../src/tpp-amalgamation.h#L24609)
that can be used to delete (restore defaults) of any custom keyword feature override.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_KEYWORD_FEATURES
```
</details>

## TPP_HAVE_KEYWORDS_RESETCOUNTERS

Provide a function [`tpp_keywords_resetcounters()`](../src/tpp-amalgamation.h#L21929) +
[`tpp_lexer_kwds_resetcounters()`](../src/tpp-amalgamation.h#L24615) that can be used to
reset the state of all `__TPP_COUNTER()` macro expansions.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_MACRO___TPP_COUNTER
```
</details>

## TPP_HAVE_LEXER_OPENFILE_EX

Enable support for [`tpp_lexer_openfile_ex()`](../src/tpp-amalgamation.h#L25242)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_OPENFILE && (TPP_HAVE_CPP_IMPORT || (TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next) || TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED || TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING || (TPP_HAVE_CPP_INCLUDE && TPP_HAVE_PRAGMA_ONCE))
```
</details>

## TPP_HAVE_PREPARSE_SKIPSPACE_FWD

Provide an API [`tpp_preparse_skipspace_fwd()`](../src/tpp-amalgamation.h#L21021) that can be used to easily skip an arbitrary
amount of whitespace (but not comments), as well as BSE sequences in a forward-direction.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY || TPP_HAVE_STRING_ESCAPE_NAMED_MANY || TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING
```
</details>

## TPP_HAVE_PREPARSE_SKIPSPACE_BCK

Provide an API [`tpp_preparse_skipspace_bck()`](../src/tpp-amalgamation.h#L21035) that can be used to easily skip an arbitrary
amount of whitespace (but not comments), as well as BSE sequences in a backwards-direction.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) || ((TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY || TPP_HAVE_STRING_ESCAPE_NAMED_MANY) && TPP_HAVE_TPP_W_UNKNOWN_NAMED_ESCAPE_SEQUENCE) || TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING
```
</details>

## TPP_HAVE_LEXER_INIT_OPEN

Enable support for [`tpp_lexer_initfile_open()`](../src/tpp-amalgamation.h#L25315), a function that lets you directly
initialize the lexer by passing in a filename that should be opened as input.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) && TPP_HAVE_LEXER_OPENFILE
```
</details>

## TPP_HAVE_LEXER_PUSHFILE_IO

Provide an API [`tpp_lexer_pushfile_io_ex()`](../src/tpp-amalgamation.h#L25340) and [`tpp_lexer_pushfile_io()`](../src/tpp-amalgamation.h#L25342)
that can be used to push [`tpp_io_handle`](../src/tpp-amalgamation.h#L13537) onto the lexer's `#include`-stack.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_INCLUDE_STACK && TPP_HAVE_FILE_NOKWD
```
</details>

## TPP_HAVE_LEXER_PUSHFILE_OPEN

Provide an API [`tpp_lexer_pushfile_open()`](../src/tpp-amalgamation.h#L25356) that can be used to quickly open
a file, given its name, and push that file onto the lexer's `#include`-stack.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_INCLUDE_STACK && TPP_HAVE_LEXER_INIT_OPEN
```
</details>

## TPP_HAVE_LEXER_PUSHFILE_OFR

Provide an API [`tpp_lexer_pushfile_ofr()`](../src/tpp-amalgamation.h#L25368) that can be used to quickly push
a [`tpp_lexer_openfile_result`](../src/tpp-amalgamation.h#L25144) object onto the lexer's `#include`-stack.

<details><summary>Details</summary>

Default:

```c
((TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_INCLUDE_STACK && TPP_HAVE_LEXER_INIT_OPEN) || TPP_HAVE_CLI_DASH_INCLUDE
```
</details>

## TPP_HAVE_LEXER_PUSHFILE_TEXT

Provide an API [`tpp_lexer_pushfile_text_ex()`](../src/tpp-amalgamation.h#L25393) that can be used to
quickly push pre-loaded text files onto the lexer's `#include`-stack:

- [`tpp_lexer_pushfile_text()`](../src/tpp-amalgamation.h#L25405)
- [`tpp_lexer_pushfile_text_ascii()`](../src/tpp-amalgamation.h#L25407)
- [`tpp_lexer_pushfile_text_utf8()`](../src/tpp-amalgamation.h#L25410)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_INCLUDE_STACK
```
</details>

## TPP_HAVE_LEXER_CLI_DEFINE

Provide a function [`tpp_lexer_define()`](../src/tpp-amalgamation.h#L25463) + [`tpp_lexer_undef()`](../src/tpp-amalgamation.h#L25471)
that can be used to define/undef commandline-defined macros.

<details><summary>Details</summary>

Default:

```c
((TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_CPP_MACROS) || TPP_HAVE_CLI_DASH_DEFINE_MACRO || TPP_HAVE_CLI_DASH_UNDEFINE_MACRO
```
</details>

## TPP_HAVE_LEXER_CLI_ASSERT

Provide a function [`tpp_lexer_assert()`](../src/tpp-amalgamation.h#L25487) + [`tpp_lexer_unassert()`](../src/tpp-amalgamation.h#L25496) + [`tpp_lexer_unassertall()`](../src/tpp-amalgamation.h#L25503)
that can be used to add/delete keyword assertions.

<details><summary>Details</summary>

Default:

```c
((TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_CPP_ASSERT) || TPP_HAVE_CLI_DASH_ASSERT
```
</details>

<!--END:core-->
