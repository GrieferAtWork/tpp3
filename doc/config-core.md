# CORE API/System/Lexer Features

These are "core" features that cannot be enabled/disabled at runtime. They must be set/known when TPP is compiled, and mainly control which (sets of) runtime features should be supported.

In terms of configuration, these feature macros only have 2 states: `#if TPP_FEATURE` and `#if !TPP_FEATURE` (meaning that the only distinctly recognized integer values are `0` and non-`0`)


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
by setting the file's [`TPP_FILE_FLAGS_NONBLOCK`](../src/tpp-amalgamation.h#L14363) flag. Also: calls made to
[`tpp_lexer_yield_blocking()`](../src/tpp-amalgamation.h#L18753) & friends can be used to force calls to become
blocking, even when enabled for the current file (s.a. [`TPP_EWOULDBLOCK`](../src/tpp-amalgamation.h#L9063)
and [`TPP_TOK_EWOULDBLOCK`](../src/tpp-amalgamation.h#L10000))

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) && (TPP_OS_WINDOWS || TPP_OS_UNIX)
```
</details>

## TPP_HAVE_UNICODE

Unicode support:

- Use of a unicode traits database that is either user-supplied,
  a dummy (where all unicode character are allows in identifiers),
  or built-in when [`TPP_HAVE_BUILTIN_CTYPE_UNICODE`](#tpp_have_builtin_ctype_unicode) is enabled.
- Automatic detection of `utf-8`, `utf-8-bom`, `utf-16[le/be]`, `utf-32[le/be]` in input files.<br/>
  NOTE: The Lexer assumes `utf-8` by default, unless it encounters an invalid `utf-8`
        byte sequence, at which point it will automatically downgrade to `ASCII`-only
        for the remainder of the relevant file

<details><summary>Details</summary>

Default:

```c
1
```
</details>

## TPP_HAVE_BUILTIN_CTYPE_UNICODE

Supply a built-in unicode character traits database (adds ~36KiB data to final executable).
Some examples of stuff that is supported when this is enabled:

- `U+0085` (`NEL`)  will be treated like `U+000A` (`LF` `\n`)
- `U+00A0` (`NBSP`) will be treated like `U+0020` (`space` `\s`)
- `U+00DF` (`ß`)    will be treated accepted as part of an identifier

As such, TPP3 follows C23+-specifications, in that the builtin unicode
database is set-up to accept identifiers made up of `XID_Start`/`XID_Cont`.

XXX: Config where `[C99,C23)`-compliant identifiers are recognized (i.e.: where 🐱 is an identifier)
     https://en.cppreference.com/c/language/identifier

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_UNICODE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_STRERROR

Provide a function [`tpp_strerror()`](../src/tpp-amalgamation.h#L9134) to get a description of a given `tpp_errno` error code

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_STRTOKENID

Provide a function [`tpp_strtokenid()`](../src/tpp-amalgamation.h#L11412) to get the API name of a (non-keyword) token ID

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_KEYWORD_USERDATA

Enable support for storing custom user-data in keywords

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_KEYWORD_ASSTRING

Provide support for [`tpp_keyword_asstring()`](../src/tpp-amalgamation.h#L15888) that allows
keywords to be binary-compatible with `tpp_string`

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

Enable support for runtime-configurable extensions

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

Support for: `tpp_warning_id_fromnumber()`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_WARNING_ERROR

Support for: [`TPP_WSTATE_ERROR`](../src/tpp-amalgamation.h#L16360) (else: only [`TPP_WSTATE_FATAL`](../src/tpp-amalgamation.h#L16365) is available)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && TPP_ERROR_LIMIT != 0
```
</details>

## TPP_HAVE_WARNING_SUPPRESS

Support for: [`TPP_WSTATE_SUPPRESS`](../src/tpp-amalgamation.h#L16369)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_WARNING_DEFAULT

Support for: [`TPP_WSTATE_DEFAULT`](../src/tpp-amalgamation.h#L16374)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_FILE_NOCLOSE

Enable support for [`TPP_FILE_FLAGS_NOCLOSE`](../src/tpp-amalgamation.h#L14366)

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_FILE_NOKWD

Enable support for [`TPP_FILE_FLAGS_NOKWD`](../src/tpp-amalgamation.h#L14369)

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE != TPP_PROFILE_MINIMAL
```
</details>

## TPP_HAVE_FILE_LC_CACHE

Speed up calls to [`tpp_file_getlcinfo()`](../src/tpp-amalgamation.h#L15099) by caching the last-read
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

Provide a function [`tpp_lexer_copy()`](../src/tpp-amalgamation.h#L18102) that can be used to duplicate a lexer.

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_LEXER_WARNING_COUNT

Lexers keep track of the # of warnings they've emitted over their lifetime. Not
actually used for anything, but can be read using [`tpp_lexer_getwarningcount()`](../src/tpp-amalgamation.h#L17683)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && (TPP_PROFILE == TPP_PROFILE_ALL)
```
</details>

## TPP_HAVE_PRAGMA_TPP_WARNING

Support for: `#pragma TPP warning(...)`  (same as [`TPP_HAVE_PRAGMA_WARNING`](config-conf.md#tpp_have_pragma_warning), but doesn't require `"-fpragma-warning"`)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA_WARNING
```

Detect:

```c
#if __has_known_extension("-fpragma-warning")
...
#endif
```
</details>

## TPP_HAVE_PRAGMA_TPP_EXTENSION

Support for: `#pragma TPP extension(...)`  (same as [`TPP_HAVE_PRAGMA_EXTENSION`](config-conf.md#tpp_have_pragma_extension), but doesn't require `"-fpragma-extension"`)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA_EXTENSION
```

Detect:

```c
#if __has_known_extension("-fpragma-extension")
...
#endif
```
</details>

## TPP_HAVE_PRAGMA_TPP_TPP_EXEC

Support for: `#pragma TPP tpp_exec(...)`  (same as [`TPP_HAVE_PRAGMA_TPP_EXEC`](config-conf.md#tpp_have_pragma_tpp_exec), but doesn't require `"-fpragma-tpp-exec"`)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA_TPP_EXEC
```

Detect:

```c
#if __has_known_extension("-fpragma-tpp-exec")
...
#endif
```
</details>

## TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS

Support for: `#pragma TPP tpp_set_keyword_flags("foo", 0x7f)`  (same as [`TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS`](config-conf.md#tpp_have_pragma_tpp_set_keyword_flags), but doesn't require `"-fpragma-tpp-set-keyword-flags"`)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
```

Detect:

```c
#if __has_known_extension("-fpragma-tpp-set-keyword-flags")
...
#endif
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

## TPP_HAVE_STRING_WARN_MULTILINE

Alter behavior of [`TPP_HAVE_STRING_ALLOW_MULTILINE`](config-conf.md#tpp_have_string_allow_multiline)
(that only takes effect when that feature is enabled):

- Multi-line strings continue to be allowed (does not affect behavior)
- When a multi-line string is encountered, emit a
  warning [`TPP_W_STRING_CONTINUED_AFTER_LINEFEED`](../src/tpp-amalgamation.h#L3424)
  (see [`TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED`](config-warn.md#tpp_have_tpp_w_string_continued_after_linefeed))

If this warning isn't wanted, it can be disabled by `-Wno-multiline-string`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRING_ALLOW_MULTILINE && TPP_HAVE_WARNINGS
```
</details>

## TPP_HAVE_LEXER_OPEN_INCLUDE_STRING

Provide a function [`tpp_lexer_open_include_string()`](../src/tpp-amalgamation.h#L18909)
to open the file associated with an `#include`-string.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_include || TPP_HAVE_MACRO___has_include_next || TPP_HAVE_MACRO___has_embed || TPP_HAVE_PRAGMA_GCC_DEPENDENCY
```
</details>

## TPP_HAVE_LEXER_DECODE_INCLUDE_STRING

Provide a function [`tpp_lexer_decode_include_string()`](../src/tpp-amalgamation.h#L18837)
to decode the actual contents of an `#include`-string.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_OPEN_INCLUDE_STRING
```
</details>

## TPP_HAVE_LEXER_YIELD_INCLUDE_STRING

Provide a function [`tpp_lexer_yield_include_string()`](../src/tpp-amalgamation.h#L18803) to
do yield the next token with special handling if the next
token's first character is `<` or `"`in which case the
token is parsed as a #include-string

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_OPEN_INCLUDE_STRING
```
</details>

## TPP_HAVE_LEXER_OPENFILE

Enable support for [`tpp_lexer_openfile()`](../src/tpp-amalgamation.h#L18397)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_OPEN_INCLUDE_STRING || TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED || 1
```
</details>

## TPP_HAVE_LEXER_OPENFILE_EX

Enable support for [`tpp_lexer_openfile_ex()`](../src/tpp-amalgamation.h#L18379)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_OPENFILE && (TPP_HAVE_CPP_IMPORT || (TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next) || (TPP_HAVE_CPP_INCLUDE && TPP_HAVE_PRAGMA_ONCE))
```
</details>

## TPP_HAVE_FILE_GETREALFILENAMEKWD

Provide a function [`tpp_file_getrealfilenamekwd()`](../src/tpp-amalgamation.h#L15146)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA_ONCE
```
</details>

## TPP_HAVE_IFNDEF_INCLUDE_GUARDS

Enable support for detecting `#ifndef`-style `#include`-guards
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

Enable support for TPP generating new `tpp_keyword` definitions
on-the-fly, as keywords are parsed (the first time any unique
keyword is parsed, [`tpp_keywords_newkeyword()`](../src/tpp-amalgamation.h#L16187) is used to give
it a unique ID and `tpp_keyword` structure). This is also needed
for macros and a number of other features that need to store some
kind of state alongside keywords.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) || TPP_HAVE_FILE_GETREALFILENAMEKWD || TPP_HAVE_CPP_MACROS || TPP_HAVE_PRAGMA_ONCE || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CLANG_MACRO___has_attribute || TPP_HAVE_CLANG_MACRO___has_builtin || TPP_HAVE_CLANG_MACRO___has_cpp_attribute || TPP_HAVE_CLANG_MACRO___has_declspec_attribute || TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_CLANG_MACRO___has_feature || TPP_HAVE_CLANG_MACRO___has_c_attribute || TPP_HAVE_MACRO___is_deprecated || TPP_HAVE_MACRO___is_poisoned || TPP_HAVE_PRAGMA_DEPRECATED || TPP_HAVE_PRAGMA_GCC_POISON || TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_IFNDEF_INCLUDE_GUARDS || TPP_HAVE_KEYWORD_INCLCOUNT || TPP_HAVE_PRAGMA_PUSH_MACRO || TPP_HAVE_MACRO___TPP_COUNTER || TPP_HAVE_KEYWORD_USERDATA
```
</details>

## TPP_HAVE_TPP_UNICODE_WRITEUTF8

Provide an API function [`tpp_unicode_writeutf8()`](../src/tpp-amalgamation.h#L9360)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TPP_TOK_C_STRING || TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_C_CHAR || TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL || TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL || TPP_HAVE_ESCAPE_IN_IDENTIFIERS
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

Enable support for [`TPP_FILE_FLAGS_SYSHDR`](../src/tpp-amalgamation.h#L14375)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER != 0
```
</details>

## TPP_HAVE_FILE_SUBTEXT

Enable support for [`TPP_FILE_KIND_SUBTEXT`](../src/tpp-amalgamation.h#L14315)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO__Pragma || TPP_HAVE_MACRO___pragma || TPP_HAVE_MACRO___TPP_IDENTIFIER || TPP_HAVE_MACRO___TPP_STR_DECOMPILE || TPP_HAVE_MACRO___TPP_STR_PACK || TPP_HAVE_MACRO___TPP_COUNT_TOKENS || TPP_HAVE_PRAGMA_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_TPP_EXEC
```
</details>

## TPP_HAVE_FILE_DUMMY

Enable support for [`TPP_FILE_KIND_DUMMY`](../src/tpp-amalgamation.h#L14321), which is
needed to support gcc's `# <linenum>` -> `1`/`2` flags

These flags push so-called "dummy" files onto the
#include-stack (without altering the actual current
file), with those dummy files acting as additional
entries for #include tracebacks.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIGIT_LINE
```
</details>

## TPP_HAVE_FILE_EXTERN_C

Enable support for [`TPP_FILE_FLAGS_EXTERN_C`](../src/tpp-amalgamation.h#L14378)

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL && TPP_HAVE_CPP_DIGIT_LINE
```
</details>

## TPP_HAVE_INCLUDE_STACK

Enable support for `tpp_file::tf_prev`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED || TPP_HAVE_FILE_SUBTEXT || TPP_HAVE_FILE_DUMMY
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

`tpp_include_paths` contains a 2nd path-list that is only searched during `"`-strings

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_STACK && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_INCLUDE_PATH_SYSHDR

`tpp_include_paths` contains a 3rd path-list whose files are treated as [`TPP_FILE_FLAGS_SYSHDR`](../src/tpp-amalgamation.h#L14375)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_STACK && TPP_HAVE_FILE_SYSHDR && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_INCLUDE_PATH_AFTER

`tpp_include_paths` contains a 4th path-list that is searched after all other paths

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_STACK && ((TPP_PROFILE == TPP_PROFILE_ALL) || ((TPP_PROFILE != TPP_PROFILE_MINIMAL) && TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH))
```
</details>

## TPP_HAVE_INCLUDE_PATH_PUSH_POP

Enable support to push/pop the `#include`-path state

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_PATH
```
</details>

## TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING

Use `TPP_REF tpp_string *` instead of `char *` in `#include`-path lists.
Doing so greatly reduces the overhead when `#include`-path lists are
pushed/popped, since `tpp_string` can be incref'd, whereas `char`
must be hard-copied.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_PATH_PUSH_POP
```
</details>

## TPP_HAVE_IFDEF_STACK

Enable support for `tpp_file::tf_ifdef`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_IF_ELSE_ENDIF
```
</details>

## TPP_HAVE_FILE_SETFILENAME

Enable support for [`tpp_file_setfilename()`](../src/tpp-amalgamation.h#L15162)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIGIT_LINE || TPP_HAVE_CPP_LINE
```
</details>

## TPP_HAVE_FILE_SETLINE

Enable support for [`tpp_file_setline()`](../src/tpp-amalgamation.h#L15175)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIGIT_LINE || TPP_HAVE_CPP_LINE
```
</details>

## TPP_HAVE_FILE_KEEPPOS

Enable support for setting a special pointer in files to
represent a lower bound of memory that must be kept loaded.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS
```
</details>

## TPP_HAVE_FILE_MACRO_TRACKARGS

Keep track of the original `tpp_lexer_arginfo` used during macro invocation,
in order to improve [`tpp_file_getlcinfo_ex()`](../src/tpp-amalgamation.h#L15211)'s `tlcix_proj*` return values,
to make them less error-prone.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS && TPP_HAVE_LEXER_SEEKPP_RPAREN && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_FILE_ENCODING_EMBED

Provide a special [`TPP_FILE_ENCODING_EMBED`](../src/tpp-amalgamation.h#L14337) file encoding
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

## TPP_HAVE_ESCAPED_KEYWORDS

Provide a secondary set of keyword APIs that include support for `\`-escape sequences.
Needed to implement [`TPP_HAVE_BSE`](config-conf.md#tpp_have_bse) and [`TPP_HAVE_ESCAPE_IN_IDENTIFIERS`](config-conf.md#tpp_have_escape_in_identifiers)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BSE || TPP_HAVE_ESCAPE_IN_IDENTIFIERS
```
</details>

## TPP_HAVE_IO_COMPARE_MTIME

Enable support for [`tpp_io_compare_mtime()`](../src/tpp-amalgamation.h#L9605).
This is the underlying system function needed for
`#pragma GCC dependency` (see [`TPP_HAVE_PRAGMA_GCC_DEPENDENCY`](config-conf.md#tpp_have_pragma_gcc_dependency))

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL || TPP_HAVE_PRAGMA_GCC_DEPENDENCY
```
</details>

## TPP_HAVE_IO_NORMALIZE_FILENAME

Enable support for [`tpp_io_normalize_filename()`](../src/tpp-amalgamation.h#L9633).
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

Enable support for [`tpp_joinpath()`](../src/tpp-amalgamation.h#L18409), a wrapper around another internal function
used to implement [`tpp_lexer_openfile()`](../src/tpp-amalgamation.h#L18397) (see [`TPP_HAVE_LEXER_OPENFILE`](#tpp_have_lexer_openfile)) and is also
needed to implement `#pragma GCC dependency` (see [`TPP_HAVE_PRAGMA_GCC_DEPENDENCY`](config-conf.md#tpp_have_pragma_gcc_dependency))

This function takes 2 paths, resolves `.` and `..` path references between them,
whilst pasting them onto each other in order to form a singular, normalized path

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL || TPP_HAVE_PRAGMA_GCC_DEPENDENCY
```
</details>

## TPP_HAVE_LEXER_INIT_IO

Enable support for [`tpp_lexer_initfile_io()`](../src/tpp-amalgamation.h#L18163) and [`tpp_lexer_initfile_io_ex()`](../src/tpp-amalgamation.h#L18161)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_FILE_NOKWD
```
</details>

## TPP_HAVE_LEXER_INIT_FILENAME

Enable support for [`tpp_lexer_initfile_open()`](../src/tpp-amalgamation.h#L18175), a function that lets you directly
initialize the lexer by passing in a filename that should be opened as input.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_OPENFILE
```
</details>

## TPP_HAVE_LEXER_SKIP

Enable support for [`tpp_lexer_skip()`](../src/tpp-amalgamation.h#L18931) and [`tpp_lexer_require()`](../src/tpp-amalgamation.h#L18945)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) || TPP_HAVE_PRAGMA_PUSH_MACRO || TPP_HAVE_PRAGMA_DEPRECATED || TPP_HAVE_PRAGMA_MESSAGE || TPP_HAVE_PRAGMA_ERROR || TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_WARNING || TPP_HAVE_PRAGMA_TPP_EXTENSION || TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH || TPP_HAVE_MACRO__Pragma || TPP_HAVE_MACRO___TPP_EVAL || TPP_HAVE_MACRO___has_include || TPP_HAVE_MACRO___has_include_next || TPP_HAVE_MACRO___has_embed || TPP_HAVE_MACRO___TPP_COUNT_TOKENS || TPP_HAVE_MACRO___TPP_STR_SIZE || TPP_HAVE_CPP_ASSERT || TPP_HAVE_CPP_EMBED || TPP_HAVE_BUILTIN_PARSEEXPR_HOOK
```
</details>

## TPP_HAVE_LEXER_TIME

Enable support for storing a time value in `tpp_lexer`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TIME_API
```
</details>

## TPP_HAVE_LEXER_TRYSKIP_RAW

Enable support for `tpp_lexer_rawskip_raw()`, a function that is used-
and needed in order to seek- and skip-over the `(` token following a
macro's name (with support for searching for tokens in parent files
of the current one, but rolling back all changes if the next token
isn't `(`). It also includes some additional functionality that will
retain the `[tf_tpos,*)` regions of files as they are scanned.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS
```
</details>

## TPP_HAVE_LEXER_REPRTOKENID

Provide a function [`tpp_lexer_reprtokenid()`](../src/tpp-amalgamation.h#L19576) to
return the string-representation of a given token ID

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_SKIP
```
</details>

## TPP_HAVE_REPRTOKENID

Provide a function [`tpp_reprtokenid()`](../src/tpp-amalgamation.h#L11421) that does the
same as [`tpp_lexer_reprtokenid()`](../src/tpp-amalgamation.h#L19576) (and is also used to
implement that function), but fails for user-defined
keyword token IDs

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_REPRTOKENID
```
</details>

## TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS

Provide a lexer state flag [`TPP_LEXER_STATE_FLAG_ALLTOKENS`](../src/tpp-amalgamation.h#L17597) that forces
[`tpp_lexer_yieldpp()`](../src/tpp-amalgamation.h#L18728) to always re-emit *all* tokens (rather than skip
over space/lf/comment tokens based on [`TPP_HAVE_TPP_TOK_SPACE`](config-conf.md#tpp_have_tpp_tok_space),
[`TPP_HAVE_TPP_TOK_LF`](config-conf.md#tpp_have_tpp_tok_lf) and [`TPP_HAVE_TPP_TOK_COMMENT`](config-conf.md#tpp_have_tpp_tok_comment))

This flag is also needed internally when TPP needs to expand
the arguments supplied to a user-defined macro

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS && (TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_SPACE) || TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_LF) || TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_COMMENT))
```
</details>

## TPP_HAVE_LEXER_GETKEYWORDDEFINED

Provide a function [`tpp_lexer_getkeyworddefined()`](../src/tpp-amalgamation.h#L19132) to check
if a given keyword is `defined()` (meaning it can be expanded
as a (potentially builtin) macro)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_CPP_MACROS || TPP_HAVE_BUILTIN_EXPR_DEFINED
```
</details>

## TPP_HAVE_LEXER_MANUALPOPFILE

Provide a set of macros/functions `tpp_lexer_manualpopfile_*`
that can be used to seek through the contents of files further
up the #include-stack in a way that allows for rollback.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS
```
</details>

## TPP_HAVE_LEXER_SEEKPP_RPAREN

Provide a function [`tpp_lexer_seekpp_rparen()`](../src/tpp-amalgamation.h#L19081) that can be used
to find the position of a matching `)`-token for the purpose
of macro argument lists.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS
```
</details>

## TPP_HAVE_LEXER_SEEKPP_RPAREN_EX

Provide a function [`tpp_lexer_seekpp_rparen_ex()`](../src/tpp-amalgamation.h#L19072) that is pretty much
the same as [`tpp_lexer_seekpp_rparen()`](../src/tpp-amalgamation.h#L19081), but is also able to deal with
alternate parenthesis pairs: `[ ]` `{ }` `< >` in addition to `( )`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_SEEKPP_RPAREN && TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
```
</details>

## TPP_HAVE_MACRO_EQUALS

Provide a function [`tpp_macro_equals()`](../src/tpp-amalgamation.h#L15481) to compare macro definitions

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TPP_W_REDEFINE_MACRO
```
</details>

## TPP_HAVE_LEXER_DECODESTRING

Provide a function [`tpp_lexer_decodestring()`](../src/tpp-amalgamation.h#L19289) to decode the data contained within strings

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TPP_TOK_C_STRINGLIKE
```
</details>

## TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS

Provide an optional performance-optimization flag [`TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS`](../src/tpp-amalgamation.h#L19301)
that may be passed to [`tpp_lexer_parsestring_cb()`](../src/tpp-amalgamation.h#L19371) to speed up decoding of certain kinds
of strings (by slightly weakening what callbacks are allowed to do when this flag is set)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_DECODESTRING && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_EXPR_VALUE_PRINTREPR

Provide a function [`tpp_expr_value_printrepr()`](../src/tpp-amalgamation.h#L9923) to construct the result of `__TPP_EVAL`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_MACRO___TPP_EVAL
```
</details>

## TPP_HAVE_TOKEN_ENCODESTRING

Provide a function [`tpp_token_encodestring()`](../src/tpp-amalgamation.h#L11497) to perform `\`-escaping of arbitrary data

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT || (TPP_HAVE_EXPR_VALUE_PRINTREPR && TPP_HAVE_BUILTIN_EXPR_STRINGS) || TPP_HAVE_MACRO___FILE__ || TPP_HAVE_MACRO___BASE_FILE__ || TPP_HAVE_MACRO___FILE_NAME__ || TPP_HAVE_MACRO___TPP_STR_PACK || (TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH && TPP_HAVE_LEXER_DUMP_DEFINITIONS)
```
</details>

## TPP_HAVE_TOKEN_REQUIRE_WHITESPACE

Provide a function [`tpp_token_require_whitespace()`](../src/tpp-amalgamation.h#L11506) to check if 2 tokens,
when written directly adjacent to each other, *might* produce a different
(set of) token(s) when re-parsed.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_MAGIC_WHITESPACE
```
</details>

## TPP_HAVE_LEXER_DECODEINT_EXPR

Provide a function [`tpp_lexer_decodeint_expr()`](../src/tpp-amalgamation.h#L19195) to parse an integer

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_TPP_TOK_INT
```
</details>

## TPP_HAVE_LEXER_DECODEINT

Provide a function [`tpp_lexer_decodeint()`](../src/tpp-amalgamation.h#L19183) to parse an integer

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TPP_TOK_INT && (TPP_HAVE_LEXER_DECODEINT_EXPR || TPP_HAVE_CPP_LINE || TPP_HAVE_CPP_DIGIT_LINE || TPP_HAVE_MACRO___TPP_STR_PACK || TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING)
```
</details>

## TPP_HAVE_LEXER_DECODEINT_SUFFIX

Add API support for integer type suffixes

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX || TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX || TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX
```
</details>

## TPP_HAVE_LEXER_DECODEFLOAT_EXPR

Provide a function [`tpp_lexer_decodefloat_expr()`](../src/tpp-amalgamation.h#L19266) to parse a float

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_BUILTIN_EXPR_FLOATS && TPP_HAVE_TPP_TOK_FLOAT
```
</details>

## TPP_HAVE_LEXER_DECODEFLOAT

Provide a function [`tpp_lexer_decodefloat()`](../src/tpp-amalgamation.h#L19254) to parse a float

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_DECODEFLOAT_EXPR
```
</details>

## TPP_HAVE_LEXER_DECODEFLOAT_SUFFIX

Add API support for float type suffixes

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX || TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX || TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX
```
</details>

## TPP_HAVE_LEXER_PARSECHARACTER_LITERAL

Provide a function [`tpp_lexer_parsecharacter_literal()`](../src/tpp-amalgamation.h#L19388) to parse character literals

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
```
</details>

## TPP_HAVE_LEXER_PARSESTRING_EXPR

Provide a function [`tpp_lexer_parsestring_expr()`](../src/tpp-amalgamation.h#L19411) to parse a string

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_BUILTIN_EXPR_STRINGS && TPP_HAVE_LEXER_DECODESTRING
```
</details>

## TPP_HAVE_LEXER_PARSECHARACTER_EXPR

Provide a function [`tpp_lexer_parsecharacter_expr()`](../src/tpp-amalgamation.h#L19439) to parse a character literal

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS && TPP_HAVE_LEXER_DECODESTRING
```
</details>

## TPP_HAVE_FTOA

Provide a function [`tpp_ftoa()`](../src/tpp-amalgamation.h#L9344) to convert a float into a string

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_EXPR_VALUE_PRINTREPR
```
</details>

## TPP_HAVE_TPP_EXTENSION_NEAREST

Provide a function [`tpp_extension_nearest()`](../src/tpp-amalgamation.h#L16327)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TPP_W_UNKNOWN_EXTENSION && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_TPP_WARNING_GROUP_NEAREST

Provide a function [`tpp_warning_group_nearest()`](../src/tpp-amalgamation.h#L16429)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TPP_W_UNKNOWN_WARNING && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_HAVE_TPP_FUZZY_MEMCMP

Provide a function [`tpp_fuzzy_memcmp()`](../src/tpp-amalgamation.h#L9376) to quantify the
*fuzziness* of how close 2 memory-blocks are to each other

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TPP_EXTENSION_NEAREST || TPP_HAVE_TPP_WARNING_GROUP_NEAREST
```
</details>

## TPP_HAVE_LEXER_DUMP_DEFINITIONS

Provide a function [`tpp_lexer_dump_definitions()`](../src/tpp-amalgamation.h#L19587) that can be
used to re-print all user-defined macro definitions and asserts.

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_LEXER_DUMP_DEFINITIONS_SORTED



<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_DUMP_DEFINITIONS
```
</details>

## TPP_HAVE_LEXER_DUMP_DEFINITIONS_EXTRAINFO



<details><summary>Details</summary>

Default:

```c
TPP_HAVE_LEXER_DUMP_DEFINITIONS
```
</details>

## TPP_HAVE_QUALITY_WARNINGS

General config for `-Wquality` warnings

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_WARNINGS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

<!--END:core-->
