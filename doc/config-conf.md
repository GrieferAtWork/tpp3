# Feature configs

Every one of these config macros can be defined as one of the following values:

| Value | Description |
| ----- | ----------- |
| `TPP_CONF_1` / `1` | Compile-time enabled  (always on; no `#pragma extension("-f...")` / `TPP_FEAT_*` available) |
`TPP_CONF_0` / `0` | Compile-time disabled |
| `TPP_CONF_EXT1` | Runtime-configurable (via `#pragma extension("-f...")` / `TPP_EXT_*`, default = `true`) |
| `TPP_CONF_EXT0` | Runtime-configurable (via `#pragma extension("-f...")` / `TPP_EXT_*`, default = `false`) |
| `TPP_CONF_FEAT1` | Runtime-configurable (via `TPP_FEAT_*`, default = `true`) |
| `TPP_CONF_FEAT0` | Runtime-configurable (via `TPP_FEAT_*`, default = `false`) |

### Hard-coded vs. Extension vs. Feature

- Hard-code the desired state (`TPP_CONF_1` / `1` or `TPP_CONF_0` / `0`) if you don't need (or maybe don't even *want*) the config to be changeable at runtime in any way, shape, or form. This is what you should use most of the time, as hard-coded configs also tend to make TPP faster (less checking of feature flags at runtime) and smaller (disabled configs will literally not produce any code)
- Use an extension (`TPP_CONF_EXT1` or `TPP_CONF_EXT0`) when you want user-code to manipulate (enable/disable) the config on-the-fly via `#pragma extension("-f...")` directives
- Use a feature (`TPP_CONF_FEAT1` or `TPP_CONF_FEAT0`) if you have different use-cases where some require the config to be enabled, while other require it to be disabled. Can also be used if the config should be user-controllable, though not via user-code but instead by (e.g.) a command-line option.


<!--BEGIN:conf-->
## TPP_HAVE_BSE

Support for `\`-escaped line continuation: when a line ends with a `\` character
that is immediately (but see [`TPP_HAVE_BSE_WHITESPACE`](#tpp_have_bse_whitespace)) followed by a linefeed,
then that linefeed is never yielded, and a potential multi-character token is
continued:

```c
foo\
bar // Produces a single token "foobar"
+\
=   // Produces a single token "+="
```

This DOES affect the line-continuation features of `#define` macro definitions.
When this is disabled, `\`-escaped line continuation can't be used there, either.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)
```

Extension name:

```c
#define TPP_EXTNAME_BSE "bse"
```

Detect:

```c
#define HAVE_BSE
// \
#undef HAVE_BSE
#ifdef HAVE_BSE
...
#endif
```
</details>

## TPP_HAVE_BSE_WHITESPACE

Extension to [`TPP_HAVE_BSE`](#tpp_have_bse): the `\` character is allowed to be followed by extra
whitespace preceding the actual linefeed:

```c
#define multi_line line1 \␣␣␣
                   line2
```

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BSE ? TPP_CONF_EXT1 : 0
```

Extension name:

```c
#define TPP_EXTNAME_BSE_WHITESPACE "bse-whitespace"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("a\\ \nb") == 1
...
#endif
```
</details>

## TPP_HAVE_IDENTIFIER_ESCAPE_UNI

Support for `\uABCD` and `\U01234567` in identifier names (will be
replaced with effective UTF-8 encodings when translated to keywords)
```c
int identifier\u0020with\u0020whitespace = 42;
// Same as:
int __TPP_IDENTIFIER("identifier with whitespace") = 42;
```

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)
```

Extension name:

```c
#define TPP_EXTNAME_IDENTIFIER_ESCAPE_UNI "extended-identifiers"
```

Detect:

```c
#define foo\u0062ar
#ifdef foobar
...
#endif
```
</details>

## TPP_HAVE_IDENTIFIER_ESCAPE_NAMED

Support for `\N{...}` in identifier names (see [`TPP_HAVE_DECODE_NAMED_ESCAPE`](config-core.md#tpp_have_decode_named_escape))
```c
int identifier\N{NO-BREAK SPACE}nbsp = 42;
// Same as:
int __TPP_IDENTIFIER("identifier\N{NO-BREAK SPACE}nbsp") = 42;
```

What actually is allowed within `\N{...}` is controlled by:

- [`TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES`](#tpp_have_escape_named_unicode_names)
- [`TPP_HAVE_ESCAPE_NAMED_UNICODE_ORD`](#tpp_have_escape_named_unicode_ord)
- [`TPP_HAVE_ESCAPE_NAMED_XML`](#tpp_have_escape_named_xml)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)
```

Extension name:

```c
#define TPP_EXTNAME_IDENTIFIER_ESCAPE_NAMED "named-extended-identifiers"
```

Detect:

```c
#define foo\N{LATIN SMALL LETTER B}ar
#ifdef foobar
...
#endif
```
</details>

## TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY

Extension to [`TPP_HAVE_IDENTIFIER_ESCAPE_NAMED`](#tpp_have_identifier_escape_named): accept
multiple character names to appear within a `\N{...}`
block. Without this extension, trying to spell multiple
character will result in an unknown-character warning.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_IDENTIFIER_ESCAPE_NAMED ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_IDENTIFIER_ESCAPE_NAMED_MANY "named-extended-identifiers-many"
```
</details>

## TPP_HAVE_CPP_DIRECTIVES

Specifies if *any* CPP directives are supported

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : ((TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL))
```

Extension name:

```c
#define TPP_EXTNAME_CPP_DIRECTIVES "cpp-directives"
```
</details>

## TPP_HAVE_CPP_MACROS

Support for C-style macros

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_MACROS "cpp-macros"
```
</details>

## TPP_HAVE_MAGIC_WHITESPACE

Enable support for magic whitespace insertions where failure
to do so would result in accidental token concatenation:

```c
#define FOO() foo
#define BAR   bar
#define SCAN(x) x
FOO()BAR          // OK: Expands to [foo][bar] (works independent of `TPP_HAVE_MAGIC_WHITESPACE`)
SCAN(FOO()BAR)    // Expands to [foo][ ][bar]  (or [foobar] when `TPP_HAVE_MAGIC_WHITESPACE` is disabled)
```

The extra space (U+0020) character in `SCAN(FOO()BAR)` gets added
during macro argument substitution in the call to `SCAN`, and is
necessary because TPP is a text-based preprocessor. Trying to get
L/C information on the associated [`TPP_TOK_SPACE`](../src/tpp-amalgamation.h#L11128) will fail.

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS || TPP_HAVE_MACRO___TPP_EXEC) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MAGIC_WHITESPACE "magic-whitespace"
```

Detect:

```c
#define FOO()         foo
#define BAR           bar
#define SCAN2(x)      pre##x##post
#define SCAN(x)       SCAN2(x)
#define prefoobarpost 0
#define prefoo        1
#define barpost       +1
#if SCAN(FOO()BAR) // "0" if disabled (non-standard-conforming, like mscv);
                   // "1 +1" if enabled (standard-conforming, like gcc)
...
#endif
```
</details>

## TPP_HAVE_CPP_BUILTIN_MACROS

Support for builtin C-style macros (require [`TPP_HAVE_CPP_MACROS`](#tpp_have_cpp_macros) to be enabled, too)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_BUILTIN_MACROS "cpp-builtin-macros"
```
</details>

## TPP_HAVE_CPP_EXCLAIM

Support for `#!foobar`-directives (which are treated as comments)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_EXCLAIM "shebang-directives"
```
</details>

## TPP_HAVE_CPP_BLANK

Support for `#`-directives (blank directives), which are ignored

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_BLANK "blank-directives"
```
</details>

## TPP_HAVE_CPP_DIGIT_LINE

Support for `# 42 "foo.h" 1 2 3 4`-directives.

These behave similar to `#line`-directives, except that following
the filename, a number of additional "flags" can be specified:

- `1`: Push a dummy-file containing the old file/line/column onto the `#include`-stack,
       before applying the new line/filename. After being pushed, said last position will
       be displayed as part of warning messages. This flag require [`TPP_HAVE_FILE_DUMMY`](config-core.md#tpp_have_file_dummy)
       to be enabled, otherwise it is ignored.
- `2`: Do the inverse of flag `1` and pop a dummy-file off the `#include`-stack. Like the
       `1` flag, this flag require [`TPP_HAVE_FILE_DUMMY`](config-core.md#tpp_have_file_dummy) to be enabled, otherwise it is
       ignored.
- `3`: Set [`TPP_FILE_FLAGS_SYSHDR`](../src/tpp-amalgamation.h#L15775) for the current text-file. When this flag is not
       supplied, [`TPP_FILE_FLAGS_SYSHDR`](../src/tpp-amalgamation.h#L15775) is instead cleared for the current text-file.
       This flag requires [`TPP_HAVE_FILE_SYSHDR`](config-core.md#tpp_have_file_syshdr) to be enabled, otherwise it is ignored.
- `4`: Same as flag `3`, except for the [`TPP_FILE_FLAGS_EXTERN_C`](../src/tpp-amalgamation.h#L15778) flag. Similarly, this
       flag requires [`TPP_HAVE_FILE_EXTERN_C`](config-core.md#tpp_have_file_extern_c) to be enabled, otherwise it is ignored.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_DIGIT_LINE "digit-directives"
```
</details>

## TPP_HAVE_CPP_LINE

Support for `#line 42 "foo.h"`-directives

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_LINE "line-directives"
```
</details>

## TPP_HAVE_CPP_INCLUDE

Support for `#include <stdio.h>`-directives

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_INCLUDE "include-directives"
```
</details>

## TPP_HAVE_CPP_INCLUDE_NEXT

Support for `#include_next <stdio.h>`-directives

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_INCLUDE_NEXT "include-next-directives"
```
</details>

## TPP_HAVE_CPP_IMPORT

Support for `#import <stdio.h>`-directives

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_IMPORT "import-directives"
```
</details>

## TPP_HAVE_CPP_IF_ELSE_ENDIF

Support for: `#if`, `#ifdef`, `#ifndef`, `#elif`, `#elifdef`, `#elifndef`, `#else`, `#endif`

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_DIRECTIVES) ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_IF_ELSE_ENDIF "if-directives"
```
</details>

## TPP_HAVE_CPP_DEFINE

Support for: `#define`, `#undef`

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_DIRECTIVES) ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_DEFINE "define-directives"
```
</details>

## TPP_HAVE_CPP_ASSERT

Support for: `#assert`, `#unassert`

see: https://gcc.gnu.org/onlinedocs/cpp/Obsolete-Features.html

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_ASSERT "assertions"
```
</details>

## TPP_HAVE_CPP_ERROR

Support for: `#error`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_ERROR "error-directives"
```
</details>

## TPP_HAVE_CPP_WARNING

Support for: `#warning`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_WARNING "warning-directives"
```
</details>

## TPP_HAVE_CPP_IDENT_SCCS

Support for: `#ident`, `#sccs`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_IDENT_SCCS "ident-directives"
```
</details>

## TPP_HAVE_CPP_PRAGMA

Support for: `#pragma`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_PRAGMA "pragma-directives"
```
</details>

## TPP_HAVE_CPP_EMBED

Support for: `#embed`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension name:

```c
#define TPP_EXTNAME_CPP_EMBED "embed-directives"
```
</details>

## TPP_HAVE_MACRO__Pragma

Support for: `_Pragma("foo")`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO__Pragma "_Pragma"
```

Detect:

```c
#ifdef _Pragma
...
#endif
```
</details>

## TPP_HAVE_MACRO___pragma

Support for: `__pragma(foo)`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___pragma "__pragma"
```

Detect:

```c
#ifdef __pragma
...
#endif
```
</details>

## TPP_HAVE_CLANG_MACRO___has_attribute

Support for clang `__has_attribute()`, which is conventionally
used to check support of `__attribute__((foo))` in C/C++ compilers.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_attribute "clang-__has_attribute"
```

Detect:

```c
#ifdef __has_attribute
...
#endif
```
</details>

## TPP_HAVE_CLANG_MACRO___has_builtin

Support for clang `__has_builtin()`, which is conventionally
used to check support of `__builtin_foo()` in C/C++ compilers.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_builtin "clang-__has_builtin"
```

Detect:

```c
#ifdef __has_builtin
...
#endif
```
</details>

## TPP_HAVE_CLANG_MACRO___has_cpp_attribute

Support for clang `__has_cpp_attribute()`, which is conventionally
used to check support of `[[foo]]` in C++ compilers.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_cpp_attribute "clang-__has_cpp_attribute"
```

Detect:

```c
#ifdef __has_cpp_attribute
...
#endif
```
</details>

## TPP_HAVE_CLANG_MACRO___has_declspec_attribute

Support for clang `__has_declspec_attribute()`, which is conventionally
used to check support of `__declspec(foo)` in C/C++ compilers.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_declspec_attribute "clang-__has_declspec_attribute"
```

Detect:

```c
#ifdef __has_declspec_attribute
...
#endif
```
</details>

## TPP_HAVE_CLANG_MACRO___has_extension

Support for clang `__has_extension()`, which is conventionally
used to check support of compiler extensions in C/C++ compilers.

NOTE: Another builtin macro of the same name exists and can be
      enabled via [`TPP_HAVE_MACRO___has_extension`](#tpp_have_macro___has_extension). The 2 macros
      don't conflict with each other though (both can safely be
      enabled at the same time), since TPP's `__has_extension()`
      takes a string, whilst this one takes a keyword/identifier.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_extension "clang-__has_extension"
```

Detect:

```c
#ifdef __has_extension
...
#endif
```
</details>

## TPP_HAVE_CLANG_MACRO___has_feature

Support for clang `__has_feature()`, which is conventionally
used to check support of compiler features in C/C++ compilers.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_feature "clang-__has_feature"
```

Detect:

```c
#ifdef __has_feature
...
#endif
```
</details>

## TPP_HAVE_CLANG_MACRO___has_c_attribute

Support for clang `__has_c_attribute()`, which is conventionally
used to check support of `[[foo]]` in C+ compilers.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_c_attribute "clang-__has_c_attribute"
```

Detect:

```c
#ifdef __has_c_attribute
...
#endif
```
</details>

## TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES

When enabled, clang's `__has_feature()` also
expands to `1` when `__has_extension()` would.

See also:

- [`TPP_HAVE_CLANG_MACRO___has_feature`](#tpp_have_clang_macro___has_feature)
- [`TPP_HAVE_CLANG_MACRO___has_extension`](#tpp_have_clang_macro___has_extension)

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CLANG_MACRO___has_extension && TPP_HAVE_CLANG_MACRO___has_feature) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_CLANG_EXTENSIONS_ARE_FEATURES "clang-extensions-are-features"
```
</details>

## TPP_HAVE_MACRO___is_identifier

Support for clang `__is_identifier()`, which can be used
to check if a given keyword is a builtin identifier:

```c
#if __is_identifier(tpp_exec)
// "#pragma tpp_exec()" is probably supported then...
#endif
```

A keyword is considered to be an "identifier" if [`TPP_TOK_ISBUILTINKEYWORD()`](../src/tpp-amalgamation.h#L12561)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___is_identifier "__is_identifier"
```

Detect:

```c
#ifdef __is_identifier
...
#endif
```
</details>

## TPP_HAVE_MACRO___is_deprecated

Support for TPP's `__is_deprecated()`, which can be used to
check if an identifier has been `#pragma deprecated("foo")`-ed

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___is_deprecated "__is_deprecated"
```

Detect:

```c
#ifdef __is_deprecated
...
#endif
```
</details>

## TPP_HAVE_MACRO___is_poisoned

Support for TPP's `__is_poisoned()`, which can be used to
check if an identifier has been `#pragma GCC poison foo`-ed

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___is_poisoned "__is_poisoned"
```

Detect:

```c
#ifdef __is_poisoned
...
#endif
```
</details>

## TPP_HAVE_MACRO___has_extension

Support for TPP's `__has_extension()`, which can be
used to check if a TPP extension is known + enabled:

```c
#if __has_extension("-ftrigraphs")
...
#endif
```

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_EXTENSIONS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___has_extension "__has_extension"
```

Detect:

```c
#ifdef __has_extension
...
#endif
```
</details>

## TPP_HAVE_MACRO___has_known_extension

Support for TPP's `__has_known_extension()`, which
can be used to check if a TPP extension is known:

```c
#if __has_known_extension("-ftrigraphs")
...
#endif
```

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_EXTENSIONS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___has_known_extension "__has_known_extension"
```

Detect:

```c
#ifdef __has_known_extension
...
#endif
```
</details>

## TPP_HAVE_MACRO___has_warning

Support for TPP's `__has_warning()`, which can be
used to check if a TPP warning is known + enabled:

```c
#if __has_warning("-Wmultiline-string")
...
#endif
```

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___has_warning "__has_warning"
```

Detect:

```c
#ifdef __has_warning
...
#endif
```
</details>

## TPP_HAVE_MACRO___has_known_warning

Support for TPP's `__has_known_warning()`, which
can be used to check if a TPP warning is known:

```c
#if __has_known_warning("-Wmultiline-string")
...
#endif
```

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___has_known_warning "__has_known_warning"
```

Detect:

```c
#ifdef __has_known_warning
...
#endif
```
</details>

## TPP_HAVE_MACRO___has_include

Support for clang `__has_include()`, which
can be used to check if `#include` exists:

```c
#if __has_include(<stdio.h>)
#include <stdio.h>
#endif
```

<details><summary>Details</summary>

Default:

```c
((TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_IMPORT) && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___has_include "__has_include"
```

Detect:

```c
#ifdef __has_include
...
#endif
```
</details>

## TPP_HAVE_MACRO___has_include_next

Support for clang `__has_include_next()`, which
can be used to check if `#include_next` exists:

```c
#if __has_include_next(<stdio.h>)
#include_next <stdio.h>
#endif
```

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_INCLUDE_NEXT && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___has_include_next "__has_include_next"
```

Detect:

```c
#ifdef __has_include_next
...
#endif
```
</details>

## TPP_HAVE_MACRO___has_embed

Support for clang `__has_embed()`, which can be
used to check if `#embed` exists and/or is empty:

```c
#if __has_embed("resource.dat" limit(10)) == __STDC_EMBED_FOUND__
#embed "resource.dat" limit(10) // Resource exists and is non-empty
#endif
```

The different values that `__has_embed` can expand to are configured by:

- `__STDC_EMBED_NOT_FOUND__` (usually `0`; [`TPP_CONFIG_VALUEOF_STDC_EMBED_NOT_FOUND`](config-value.md#tpp_config_valueof_stdc_embed_not_found))
- `__STDC_EMBED_FOUND__` (usually `1`; [`TPP_CONFIG_VALUEOF_STDC_EMBED_FOUND`](config-value.md#tpp_config_valueof_stdc_embed_found))
- `__STDC_EMBED_EMPTY__` (usually `2`; [`TPP_CONFIG_VALUEOF_STDC_EMBED_EMPTY`](config-value.md#tpp_config_valueof_stdc_embed_empty))

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_EMBED && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___has_embed "__has_embed"
```

Detect:

```c
#ifdef __has_embed
...
#endif
```
</details>

## TPP_HAVE_MACRO___FILE__

Support for the builtin macro `__FILE__`,
which expands to the current file's name

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___FILE__ "__FILE__"
```

Detect:

```c
#ifdef __FILE__
...
#endif
```
</details>

## TPP_HAVE_MACRO___LINE__

Support for the builtin macro `__LINE__`,
which expands to the current line number

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___LINE__ "__LINE__"
```

Detect:

```c
#ifdef __LINE__
...
#endif
```
</details>

## TPP_HAVE_MACRO___TIME__

Support for the builtin macro `__TIME__`,
which expands to something like `"12:37:58"`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TIME__ "__TIME__"
```

Detect:

```c
#ifdef __TIME__
...
#endif
```
</details>

## TPP_HAVE_MACRO___DATE__

Support for the builtin macro `__DATE__`,
which expands to something like `"Jul  6 2026"`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___DATE__ "__DATE__"
```

Detect:

```c
#ifdef __DATE__
...
#endif
```
</details>

## TPP_HAVE_MACRO___COLUMN__

Support for the builtin macro `__COLUMN__`,
which expands to current column number

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___COLUMN__ "column-macro"
```

Detect:

```c
#ifdef __COLUMN__
...
#endif
```
</details>

## TPP_HAVE_MACRO___BASE_FILE__

Support for the builtin macro `__BASE_FILE__`,
which expands to base file's name

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___BASE_FILE__ "basefile-macro"
```

Detect:

```c
#ifdef __BASE_FILE__
...
#endif
```
</details>

## TPP_HAVE_MACRO___FILE_NAME__

Support for the builtin macro `__FILE_NAME__`.
Similar to `__FILE__` (see [`TPP_HAVE_MACRO___FILE__`](#tpp_have_macro___file__)), but expand to the
current file's name, without its leading path components (i.e. its *basename*).

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___FILE_NAME__ "__FILE_NAME__"
```

Detect:

```c
#ifdef __FILE_NAME__
...
#endif
```
</details>

## TPP_HAVE_MACRO___INCLUDE_LEVEL__

Support for the builtin macro `__INCLUDE_LEVEL__`,
which expands to numerical representation of include depth.
Whilst inside the "base"-file, it expands to `0`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___INCLUDE_LEVEL__ "include-level-macro"
```

Detect:

```c
#ifdef __INCLUDE_LEVEL__
...
#endif
```
</details>

## TPP_HAVE_MACRO___INCLUDE_DEPTH__

Support for the builtin macro `__INCLUDE_DEPTH__`, which is an
alias for `__INCLUDE_LEVEL__` (see [`TPP_HAVE_MACRO___INCLUDE_LEVEL__`](#tpp_have_macro___include_level__))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___INCLUDE_DEPTH__ "include-depth-macro"
```

Detect:

```c
#ifdef __INCLUDE_DEPTH__
...
#endif
```
</details>

## TPP_HAVE_MACRO___COUNTER__

Support for the builtin macro `__COUNTER__`, which
expands to`1+` its previous expansion. The first time
this macro is used, it expands to `0`.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___COUNTER__ "counter-macro"
```

Detect:

```c
#ifdef __COUNTER__
...
#endif
```
</details>

## TPP_HAVE_MACRO___TIMESTAMP__

Support for the builtin macro `__TIMESTAMP__`, which
expands to something like `"Mon Jul  6 12:37:58 2026"`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TIMESTAMP__ "timestamp-macro"
```

Detect:

```c
#ifdef __TIMESTAMP__
...
#endif
```
</details>

## TPP_HAVE_NUMERIC_DATE_MACROS

Support for the builtin macros `__DATE_DAY__`, `__DATE_WDAY__`, `__DATE_YDAY__`, `__DATE_MONTH__`, `__DATE_YEAR__`.
These are precise date/time macros and behave like `__LINE__`, but expand to the components of `__DATE__`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_NUMERIC_DATE_MACROS "numeric-date-macros"
```

Detect:

```c
#ifdef __DATE_DAY__, ...
...
#endif
```
</details>

## TPP_HAVE_NUMERIC_TIME_MACROS

Support for the builtin macros `__TIME_SEC__`, `__TIME_MIN__`, `__TIME_HOUR__`.
These are precise date/time macros and behave like `__LINE__`, but expand to components of `__TIME__`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_NUMERIC_TIME_MACROS "numeric-time-macros"
```

Detect:

```c
#ifdef __TIME_SEC__, ...
...
#endif
```
</details>

## TPP_HAVE_MACRO___TPP_EVAL

Support for the builtin function-like macro `__TPP_EVAL()`, which can
be used to evaluate an expression like in `#if` and replace the macro
invocation with the expression's result in the form of 1-2 tokens:

- `[<int>]`
- `[-][<int>]`
- `[<string>]` (if [`TPP_HAVE_BUILTIN_EXPR_STRINGS`](#tpp_have_builtin_expr_strings) is enabled)

Example:

```c
__TPP_EVAL(10 + 20) // Expands to a single token: 30
```

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TPP_EVAL "tpp-eval-macro"
```

Detect:

```c
#ifdef __TPP_EVAL
...
#endif
```
</details>

## TPP_HAVE_MACRO___TPP_EXEC

Support for the builtin function-like macro `__TPP_EXEC()`.

Takes a string that is then re-interpreted as preprocessor input,
and expands to whatever that string expands to. Note that the string
may contain any kind of directive:

```c
#define RESOURCE_SIZE2(...) __VA_NARGS__
#define RESOURCE_SIZE1(x)   RESOURCE_SIZE2(x)
#define RESOURCE_SIZE(name) RESOURCE_SIZE1(__TPP_EXEC("#embed " #name))
RESOURCE_SIZE("resource.dat") // Expands to file size in bytes
```

NOTE: If you just want to execute code but discard whatever it expands
      to, use `#pragma tpp_exec()` (see [`TPP_HAVE_PRAGMA_TPP_EXEC`](#tpp_have_pragma_tpp_exec))
      instead, which has a lower overhead and lets you safely execute
      the code whilst discarding its result.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TPP_EXEC "tpp-exec-macro"
```

Detect:

```c
#ifdef __TPP_EXEC
...
#endif
```
</details>

## TPP_HAVE_MACRO___TPP_UNIQUE

Support for the builtin function-like macro `__TPP_UNIQUE()`.

When called with some keyword/identifier, this macro expands a unique INT-token
representative of the given keyword/identifier. The value of that INT-token
is distinct from any other keyword/identifier and remains the same for the
remaining of input (though it will differ if re-run with different input;
this is not a hash-function)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TPP_UNIQUE "tpp-unique-macro"
```

Detect:

```c
#ifdef __TPP_UNIQUE
...
#endif
```
</details>

## TPP_HAVE_MACRO___TPP_LOAD_FILE

Support for the builtin function-like macro `__TPP_LOAD_FILE()`.

This macro behaves similar to `#include`, but the contents of the included
file are not macro-expanded, but instead packaged into a string literal to
which this macro then expands.

Using some other features/extensions, this macro can be implemented as:

```c
#define __TPP_LOAD_FILE(filename) __TPP_STR_PACK(__TPP_EXEC("#embed " #filename))
```

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TPP_LOAD_FILE "tpp-load-file-macro"
```

Detect:

```c
#ifdef __TPP_LOAD_FILE
...
#endif
```
</details>

## TPP_HAVE_MACRO___TPP_COUNTER

Support for the builtin function-like macro `__TPP_COUNTER()`.

Called the same way as `__TPP_UNIQUE` (see [`TPP_HAVE_MACRO___TPP_UNIQUE`](#tpp_have_macro___tpp_unique)),
but returns an ever-increasing value starting at `0` (same as `__COUNTER__`),
but that counter is specific to the given keyword. i.e.: `__TPP_COUNTER(foo)`
and `__TPP_COUNTER(bar)` increment different counters

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TPP_COUNTER "tpp-counter-macro"
```

Detect:

```c
#ifdef __TPP_COUNTER
...
#endif
```
</details>

## TPP_HAVE_MACRO___TPP_RANDOM

Support for the builtin function-like macro `__TPP_RANDOM()`.

Overloaded macro taking 1 or 2 arguments, and expanding to a random number
that stays the same during repeated compilations, only changing if source
code is altered:

- The single-argument form `__TPP_RANDOM(hi)` expands to a pseudo-random
  integer token in the range `[0,hi)`
- The 2-argument form `__TPP_RANDOM(lo, hi)` expands to a pseudo-random
  integer token in the range `[lo,hi)`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TPP_RANDOM "tpp-random-macro"
```

Detect:

```c
#ifdef __TPP_RANDOM
...
#endif
```
</details>

## TPP_HAVE_MACRO___TPP_STR_DECOMPILE

Support for the builtin function-like macro `__TPP_STR_DECOMPILE()`.

This macro is very similar to `__TPP_EXEC()` (see [`TPP_HAVE_MACRO___TPP_EXEC`](#tpp_have_macro___tpp_exec)),
except that while `__TPP_EXEC()` will expand other macros and directives,
`__TPP_STR_DECOMPILE()` doesn't: it simply takes a string and expands to
its decoded form *without* expansion (however: expansion may still occur as
returned tokens are yielded, so this difference may not always be obvious).

In practice this usually only means:

- `__TPP_EXEC()` accepts preprocessor directives
- `__TPP_STR_DECOMPILE()` only does basic string-to-token conversion

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TPP_STR_DECOMPILE "tpp-str-decompile-macro"
```

Detect:

```c
#ifdef __TPP_STR_DECOMPILE
...
#endif
```
</details>

## TPP_HAVE_MACRO___TPP_STR_PACK

Support for the builtin function-like macro `__TPP_STR_PACK()`.

Expands to a string literal that is made up of the arguments taken by
this macro. There can be any number of arguments (and arguments don't
even have to be separated by `,`), but every argument must be:

- Another string (that is added to the result after being decoded and re-encoded)
- An INT token (that must evaluate to a value in range `[0,0xFF]`)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TPP_STR_PACK "tpp-str-pack-macro"
```

Detect:

```c
#ifdef __TPP_STR_PACK
...
#endif
```
</details>

## TPP_HAVE_MACRO___TPP_STR_SUBSTR

Support for the builtin function-like macro `__TPP_STR_SUBSTR()` and `__TPP_STR_AT()`.

Stand-alone macro that takes 3 arguments and (assuming [`TPP_HAVE_BUILTIN_EXPR_STRINGS`](#tpp_have_builtin_expr_strings)
and [`TPP_HAVE_MACRO___TPP_EVAL`](#tpp_have_macro___tpp_eval) are enabled) can be implemented as follows:

```c
#define __TPP_STR_SUBSTR_3(str, i, n) __TPP_EVAL((str)[(i):(i)+(n)])
#define __TPP_STR_SUBSTR_2(str, i)    __TPP_EVAL((str)[(i):(i)+1])
#define __TPP_STR_SUBSTR(...)         __TPP_STR_SUBSTR_##__VA_NARGS__(__VA_ARGS__)
```

The macro `__TPP_STR_AT()` behaves the same, but quotes its result using `'` instead of `"`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TPP_STR_SUBSTR "tpp-str-substr-macro"
```

Detect:

```c
#if defined(__TPP_STR_SUBSTR) || defined(__TPP_STR_AT)
...
#endif
```
</details>

## TPP_HAVE_MACRO___TPP_STR_SIZE

Support for the builtin function-like macro `__TPP_STR_SIZE()`.

Stand-alone macro that takes 1 arguments and (assuming [`TPP_HAVE_BUILTIN_EXPR_STRINGS`](#tpp_have_builtin_expr_strings)
and [`TPP_HAVE_MACRO___TPP_EVAL`](#tpp_have_macro___tpp_eval) are enabled) can be implemented as follows:

```c
#define __TPP_STR_SIZE(str) __TPP_EVAL(#(str))
```

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TPP_STR_SIZE "tpp-str-size-macro"
```

Detect:

```c
#ifdef __TPP_STR_SIZE
...
#endif
```
</details>

## TPP_HAVE_MACRO___TPP_COUNT_TOKENS

Support for the builtin function-like macro `__TPP_COUNT_TOKENS()`.

A token-feature probing macro that lets you count the number of
tokens that are contained within a given string. For this
purpose, no macro expansion or directive processing is done:

```c
__TPP_COUNT_TOKENS("++")         // 1 if "TPP_HAVE_TOK_PLUS_PLUS" is enabled, else 2
#define FOO 10 20 30
__TPP_COUNT_TOKENS("FOO")        // 1 because macros aren't expanded
__TPP_COUNT_TOKENS("#undef FOO") // 3 (or 4 if TPP_HAVE_TOK_SPACE) because directives aren't parsed
```

Based on the numbers returned by this macro, it becomes possible
to detect the state of pretty much all configuration options that
affect the behavior of [`tpp_lexer_yieldraw()`](../src/tpp-amalgamation.h#L20411)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TPP_COUNT_TOKENS "tpp-count-tokens-macro"
```

Detect:

```c
#ifdef __TPP_COUNT_TOKENS
...
#endif
```
</details>

## TPP_HAVE_MACRO___TPP_IDENTIFIER

Support for the builtin function-like macro `__TPP_IDENTIFIER()`.

This builtin macro can be used to construct arbitrary keyword-like
tokens, including ones that aren't actually keywords. This macro
takes a single argument (in the form of a string), and always expands
to a single keyword/identifier-type token:

```c
__TPP_IDENTIFIER("foo")   // Same "foo"
__TPP_IDENTIFIER("1foo")  // Still a keyword; namely: "1foo" -- there's no way
                          // to write this, since it'd normally be a TPP_TOK_C_INT
__TPP_IDENTIFIER("a\0b")  // Compilers probably won't like this: NUL-character in
                          // keyword name, meaning strlen() returns "1", but keyword
                          // is legally distinct from "a" (though lots of places
                          // will probably print the keyword as "a")
```

Similar functionality can also be achieved using [`TPP_HAVE_IDENTIFIER_ESCAPE_UNI`](#tpp_have_identifier_escape_uni)

Note that unlike [`TPP_HAVE_IDENTIFIER_ESCAPE_UNI`](#tpp_have_identifier_escape_uni), `__TPP_IDENTIFIER`
can only be used in places where macros are expanded:

```c
#__TPP_IDENTIFIER("define") foo 42  // Won't work
#\u0064efine foo 42                 // This will work
```

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO___TPP_IDENTIFIER "tpp-identifier-macro"
```

Detect:

```c
#ifdef __TPP_IDENTIFIER
...
#endif
```
</details>

## TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS

Support for alternative parenthesis pairs in user-defined macros:

```c
#define normal_macro(x) you_should("know this", x)
#define array[index]    get_ident(ARRAY, index)
#define block{...}      do { __VA_ARGS__ } while (should_continue())
#define point<T>        struct { T x; T y; }

normal_macro(foo) // you_should("know this", foo)
array[42]         // get_ident(ARRAY, 42)
block { body }    // do { body } while (should_continue())
point<int>        // struct { int x; int y; }
```

These alternate parenthesis pairs work in a hierarchy, where
higher-order parenthesis pairs respect unmatched parentheses
of lower orders, but (to not break standard compatibility),
not the opposite. iow: `point<foo(x > 10)>` works
as expected (the `T` parameter is `foo(x > 10)` rather
than `foo(x `), but `(normal_macro(foo < 10) > 20)` does not
look out of `< >`-paris (the `x` parameter is `foo < 10`
rather than `foo < 10) > 20`). See the following hierarchy:

| order | l-paren | r-paren |
| ----- | ------- | ------- |
| 1     | `(`     | `)`     |
| 2     | `[`     | `]`     |
| 3     | `{`     | `}`     |
| 4     | `<`     | `>`     |

Some more examples of the results produced by this hirarchy:

```c
point<{ int x = v > 20; }>   // T is `{ int x = v > 20; }`
point<arr[y > 10 ? 0 : 2]>   // T is `arr[y > 10 ? 0 : 2]`
point<foo(x > 10)>           // T is `foo(x > 10)`
point<point<int>>            // T is `point<int>`
point<x > 10>                // !!! T is `x` and expansion is followed by ` 10>`

block { int y = < 20; } > }; // __VA_ARGS__ is `int y = v < 20;` and expansion is followed by ` > };`
block { int y = { 20; } };   // __VA_ARGS__ is `int y = { 20; }`
block { int y = [ 20; } ] }; // __VA_ARGS__ is `int y = [20; } ]`
block { int y = ( 20; } ) }; // __VA_ARGS__ is `int y = (20; } )`

array[y < 20] > ]; // index is `y < 20` and expansion is followed by ` > ];`
array[y { 20] } ]; // index is `y < 20` and expansion is followed by ` } ];`
array[y [ 20] ];   // index is `y [ 20]`
array[y ( 20] ) ]; // index is `y ( 20] )`

normal_macro(y < 20) > ); // x is `y < 20` and expansion is followed by ` > );`
normal_macro(y { 20) } ); // x is `y { 20` and expansion is followed by ` } );`
normal_macro(y [ 20) ] ); // x is `y [ 20` and expansion is followed by ` ] );`
normal_macro(y ( 20) );   // x is `y ( 20)`
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*:

```c
#pragma extension("-falternative-macro-parenthesis")
#define point1<T> struct { T x; T y; }
#pragma extension("-fno-alternative-macro-parenthesis")
#define point2<T> struct { T x; T y; }

point1<int>  // struct { int x; int y; }
point2<int>  // <T> struct { T x; T y; }<int>
```

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS ? TPP_CONF_EXT1 : 0
```

Extension name:

```c
#define TPP_EXTNAME_ALTERNATIVE_MACRO_PARENTHESIS "alternative-macro-parenthesis"
```

Detect:

```c
#define foo<bar> +1
#define bar 10
#if (1 foo<1> +1) == 3
...
#endif
```
</details>

## TPP_HAVE_MACRO_ARGUMENT_WHITESPACE

Retain whitespace around macro arguments:

```c
#define STR1(x) #x
#pragma extension(push, "-fmacro-argument-whitespace")
#define STR2(x) #x
#pragma extension(pop)
STR1(  foo  ) // "foo"
STR2(  foo  ) // "  foo  "
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT0 : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO_ARGUMENT_WHITESPACE "macro-argument-whitespace"
```
</details>

## TPP_HAVE_MACRO_RECURSION

Allow self-recursion for certain macros:

```c
#define REPEAT_0(n, x)
#pragma extension(push, "-fmacro-recursion")
#define REPEAT_1(n, x) #!x REPEAT(n, #!x)
#define REPEAT__(v, n, x) REPEAT_##v(n, #!x)
#define REPEAT_(v, n, x) REPEAT__(v, n, #!x)
#define REPEAT(n, x) REPEAT_(__TPP_EVAL((n) > 0), __TPP_EVAL((n)-1), #!x)
#pragma extension(pop)
REPEAT(42, HELLO_WORLD) // HELLO_WORLD HELLO_WORLD HELLO_WORLD [...] (42 times)
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? TPP_CONF_EXT0 : 0
```

Extension name:

```c
#define TPP_EXTNAME_MACRO_RECURSION "macro-recursion"
```
</details>

## TPP_HAVE_TRADITIONAL_MACROS

Support for traditional macro expansion rules:

```c
#define CAT(a, b) a##b
#define STR(x)    #x
#pragma extension(push, "-ftraditional-macro")
#define T_CAT(a, b) a/**/b
#define T_STR(x)    "x"
#pragma extension(pop)
CAT(10, 20)   // 1020
STR(10)       // "10"
T_CAT(10, 20) // 1020
T_STR(10)     // "10"
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION* ")

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? TPP_CONF_EXT0 : 0
```

Extension name:

```c
#define TPP_EXTNAME_TRADITIONAL_MACROS "traditional-macro"
```
</details>

## TPP_HAVE_NAMED_VARARGS_IN_MACROS

Support for variable-argument macros with named varargs:

```c
#define printf(format, args...) fprintf(stdout, format, args)
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_NAMED_VARARGS_IN_MACROS "named-varargs-in-macros"
```
</details>

## TPP_HAVE_VA_ARGS_IN_MACROS

Support for variable-argument macros:

```c
#define printf(...) fprintf(stdout, __VA_ARGS__)
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_VA_ARGS_IN_MACROS "va-args-in-macros"
```
</details>

## TPP_HAVE_VA_COMMA_IN_MACROS

Support for special keyword `__VA_COMMA__` inside of variable-argument macros.
When varargs are non-empty, replaced with `,`. Otherwise, replaced with nothing:

```c
#define printf(format, ...) fprintf(stdout, format __VA_COMMA__ __VA_ARGS__)
printf("foo\n");         // fprintf(stdout, "foo\n");
printf("i = %d\n", 10);  // fprintf(stdout, "i = %d\n", 10);
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_VA_COMMA_IN_MACROS "va-comma-in-macros"
```

Detect:

```c
#define test1(a, b, ...) __VA_ARGS__+0
#define test2(...) test1(__VA_COMMA__ 0, 1)
#if test2(~)
...
#endif
```
</details>

## TPP_HAVE_VA_OPT_IN_MACROS

Support for special function `__VA_OPT__()` inside of variable-argument macros.
When varargs are non-empty, the content within parenthesis is kept. Otherwise,
said content is removed alongside the `__VA_OPT__()` call itself:

```c
#define printf(format, ...) fprintf(stdout, format __VA_OPT__(,) __VA_ARGS__)
printf("foo\n");         // fprintf(stdout, "foo\n");
printf("i = %d\n", 10);  // fprintf(stdout, "i = %d\n", 10);
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_VA_OPT_IN_MACROS "va-opt-in-macros"
```

Detect:

```c
#define test1(a, b, ...) __VA_ARGS__+0
#define test2(...) test1(__VA_OPT__(,) 0, 1)
#if test2(~)
...
#endif
```
</details>

## TPP_HAVE_VA_NARGS_IN_MACROS

Support for special keyword `__VA_NARGS__` inside of variable-argument macros.
Expands to the number of varargs, allowing for easy overloading of macros based
on the number of supplied arguments:

```c
#define min_0()        0
#define min_1(a)       a
#define min_2(a, b)    ((a) < (b) ? (a) : (b))
#define min_3(a, b, c) min_2(min_2(a, b), c)
#define min(...)       min_##__VA_NARGS__(__VA_ARGS__)
min()        // Expands to: "0"
min(10)      // Expands to: "10"
min(10, 20)  // Expands to: "((10) < (20) ? (10) : (20))"
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_VA_NARGS_IN_MACROS "va-nargs-in-macros"
```

Detect:

```c
#define test___VA_NARGS__ 0
#define test_1            1
#define test(...) test_##__VA_NARGS__
#if test2(~)
...
#endif
```
</details>

## TPP_HAVE_VA_GLUE_COMMA_IN_MACROS

Support for GCC-style va-comma: when a variable-argument macro
contains a construct like `, ## __VA_ARGS__` (or the name of the
variable-length argument when [`TPP_HAVE_NAMED_VARARGS_IN_MACROS`](#tpp_have_named_varargs_in_macros)
is enabled), then the `,` is deleted during expansion whenever
the there are no variable arguments:

```c
#define printf(format, ...) fprintf(stdout, format, ## __VA_ARGS__)
printf("foo\n");         // fprintf(stdout, "foo\n");
printf("i = %d\n", 10);  // fprintf(stdout, "i = %d\n", 10);
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_VA_GLUE_COMMA_IN_MACROS "glue-comma-in-macros"
```

Detect:

```c
#define test1(a, b, ...) __VA_ARGS__+0
#define test2(...) test1(,##__VA_ARGS__, 1)
#if test2() == 0
...
#endif
```
</details>

## TPP_HAVE_STRINGIZE_MACRO_ARGUMENT

Support for the so-called "stringize" operator in macros:

```c
#define str(x) #x
str(foo)    // Expands to: "foo"
str("foo")  // Expands to: "\"foo\""
```

Note that this operator only takes effect during modern macro
compilation. When [`TPP_HAVE_TRADITIONAL_MACROS`](#tpp_have_traditional_macros) is enabled,
then stringization works differently and must be done like so:

```c
#define str(x) "x"
str(foo)    // Expands to: "foo"
str("foo")  // Expands to: ""foo"" -- oops; traditional macros can't do this
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRINGIZE_MACRO_ARGUMENT "stringize-macro-argument"
```

Detect:

```c
#define str(x) #x
#if __TPP_COUNT_TOKENS(str(a b)) == 1
...
#endif
```
</details>

## TPP_HAVE_CHARIZE_MACRO_ARGUMENT

Support for the so-called "charize" operator in macros:

```c
#define str(x) #@x
str(foo)    // Expands to: 'foo'
str('foo')  // Expands to: '\'foo\''
```

Note that this operator only takes effect during modern macro
compilation. When [`TPP_HAVE_TRADITIONAL_MACROS`](#tpp_have_traditional_macros) is enabled,
then charization works differently and must be done like so:

```c
#define chr(x) 'x'
chr(foo)    // Expands to: 'foo'
chr('foo')  // Expands to: ''foo'' -- oops; traditional macros can't do this
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_CHARIZE_MACRO_ARGUMENT "charize-macro-argument"
```

Detect:

```c
#define str(x) #@x
#if __TPP_COUNT_TOKENS(str(a b)) == 1
...
#endif
```
</details>

## TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT

Support for `#define noexpand(x) #!x`, which allows
you to insert macro arguments without expanding them:

```c
#define STR1(x) #x
#define STR2(x) STR1(x)
#define STR3(x) STR1(#!x)
#define FOO 42
STR1(FOO) // "FOO"
STR2(FOO) // "42"
STR3(FOO) // "FOO"
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_DONT_EXPAND_MACRO_ARGUMENT "dont-expand-macro-argument"
```

Detect:

```c
#define test1(x) #x
#define test2(x) test1(#!x)
#define test3    42
#if __TPP_STR_SIZE(__TPP_COUNT_TOKENS(test3)) == 5
...
#endif
```
</details>

## TPP_HAVE_GLUE_MACRO_ARGUMENT

Support for `##` in macros to concatenate different tokens
in order to form a new token:

```c
#define cat(a, b) a##b
cat(+, +)  // Expands to a single token "++" (assuming that TPP_HAVE_TOK_PLUS_PLUS is enabled)
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_GLUE_MACRO_ARGUMENT "glue-macro-argument"
```

Detect:

```c
#define test(a, b) a##b
#define str2(x) #x
#define str(x) str2(x)
#if __TPP_COUNT_TOKENS(str(test(10, 20))) == 1
...
#endif
```
</details>

## TPP_HAVE_PRAGMA_PUSH_MACRO

Support for: `#pragma push_macro()` / `#pragma pop_macro()`

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_PUSH_MACRO "pragma-push-macro"
```

Detect:

```c
#define TEST 42
#pragma push_macro("TEST")
#undef TEST
#pragma pop_macro("TEST")
#ifdef TEST
...
#endif
```
</details>

## TPP_HAVE_PRAGMA_ONCE

Support for: `#pragma once`

<details><summary>Details</summary>

Default:

```c
((TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT) && TPP_HAVE_PRAGMA) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_ONCE "pragma-once"
```
</details>

## TPP_HAVE_PRAGMA_DEPRECATED

Support for: `#pragma deprecated("foo")`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_DEPRECATED "pragma-deprecated"
```
</details>

## TPP_HAVE_PRAGMA_EXTENSION

Support for: `#pragma extension(...)`

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_PRAGMA && TPP_HAVE_EXTENSIONS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_EXTENSION "pragma-extension"
```
</details>

## TPP_HAVE_PRAGMA_WARNING

Support for: `#pragma warning(...)`

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_WARNING "pragma-warning"
```
</details>

## TPP_HAVE_PRAGMA_MESSAGE

Support for: `#pragma message("...")`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_MESSAGE "pragma-message"
```
</details>

## TPP_HAVE_PRAGMA_ERROR

Support for: `#pragma error("...")`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_ERROR "pragma-error"
```
</details>

## TPP_HAVE_PRAGMA_REGION

Support for: `#pragma region` + `#pragma endregion`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_REGION "pragma-region"
```
</details>

## TPP_HAVE_PRAGMA_TPP_EXEC

Support for: `#pragma tpp_exec("...")`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_TPP_EXEC "pragma-tpp-exec"
```
</details>

## TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS

Support for: `#pragma tpp_set_keyword_flags("foo", 0x7f)`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_TPP_SET_KEYWORD_FLAGS "pragma-tpp-set-keyword-flags"
```
</details>

## TPP_HAVE_PRAGMA_GCC_POISON

Support for: `#pragma GCC poison foo`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_GCC_POISON "pragma-gcc-poison"
```
</details>

## TPP_HAVE_PRAGMA_GCC_WARNING

Support for: `#pragma GCC warning "message"`

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_GCC_WARNING "pragma-gcc-warning"
```
</details>

## TPP_HAVE_PRAGMA_GCC_ERROR

Support for: `#pragma GCC error "message"`

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_GCC_ERROR "pragma-gcc-error"
```
</details>

## TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER

Support for: `#pragma GCC system_header`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_GCC_SYSTEM_HEADER "pragma-gcc-system-header"
```
</details>

## TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC

Support for: `#pragma GCC diagnostic`

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_GCC_DIAGNOSTIC "pragma-gcc-diagnostic"
```
</details>

## TPP_HAVE_PRAGMA_GCC_DEPENDENCY

Support for: `#pragma GCC dependency`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_GCC_DEPENDENCY "pragma-gcc-dependency"
```
</details>

## TPP_HAVE_TRIGRAPHS

Support for trigraph character aliases:

| Trigraph | Replacement |
| -------- | ----------- |
| `??=`    | `#`         |
| `??(`    | `[`         |
| `??/`    | `\`         |
| `??)`    | `]`         |
| `??'`    | `^`         |
| `??<`    | `{`         |
| `??!`    | `\|`        |
| `??>`    | `}`         |
| `??-`    | `~`         |

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)
```

Extension name:

```c
#define TPP_EXTNAME_TRIGRAPHS "trigraphs"
```
</details>

## TPP_HAVE_DIGRAPHS

Support for digraph token aliases:
| Digraph | Replacement |
| ------- | ----------- |
| `<%`    | `{`         |
| `<:`    | `[`         |
| `%>`    | `}`         |
| `:>`    | `]`         |
| `%:`    | `#`         |
| `%:%:`  | `##` (requires [`TPP_HAVE_TOK_POUND_POUND`](config-token.md#tpp_have_tok_pound_pound)) |
| `<::`   | `<`, `::` (requires [`TPP_HAVE_TOK_COLON_COLON`](config-token.md#tpp_have_tok_colon_colon)) |

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)
```

Extension name:

```c
#define TPP_EXTNAME_DIGRAPHS "digraphs"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("%:") == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_LF

Configures if line-feed tokens should be forwarded, or filtered by [`tpp_lexer_yieldpp()`](../src/tpp-amalgamation.h#L20608)

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_SPACE
```

Extension name:

```c
#define TPP_EXTNAME_TOK_LF "tok-lf"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("\n") != 0
...
#endif
```
</details>

## TPP_HAVE_TOK_SPACE

Configures if whitespace tokens should be forwarded, or filtered by [`tpp_lexer_yieldpp()`](../src/tpp-amalgamation.h#L20608)

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_SPACE
```

Extension name:

```c
#define TPP_EXTNAME_TOK_SPACE "tok-space"
```

Detect:

```c
#if __TPP_COUNT_TOKENS(" ") != 0
...
#endif
```
</details>

## TPP_HAVE_TOK_COMMENT

Configures if comment tokens should be forwarded, or filtered by [`tpp_lexer_yieldpp()`](../src/tpp-amalgamation.h#L20608)

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_SPACE
```

Extension name:

```c
#define TPP_EXTNAME_TOK_COMMENT "tok-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("// a b c") == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_CXX_COMMENT

Enable support for recognizing c++-like comments: `// like this one!`

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_DEFAULT || (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)) ? 1 : (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_ENABLE(TPP_COMMON_HAVE_TPP_TOK_COMMENT) : TPP_COMMON_HAVE_TPP_TOK_COMMENT)
```

Extension name:

```c
#define TPP_EXTNAME_TOK_CXX_COMMENT "tok-cxx-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("// a b c") <= 1
...
#endif
```
</details>

## TPP_HAVE_TOK_C_COMMENT

Enable support for recognizing c-like comments: `/* like this one! */`

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_DEFAULT || (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)) ? 1 : (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_ENABLE(TPP_COMMON_HAVE_TPP_TOK_COMMENT) : TPP_COMMON_HAVE_TPP_TOK_COMMENT)
```

Extension name:

```c
#define TPP_EXTNAME_TOK_C_COMMENT "tok-c-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("/* a b c */") <= 1
...
#endif
```
</details>

## TPP_HAVE_TOK_PASCAL_COMMENT

Enable support for recognizing pascal-like comments: `(* like this one! *)`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension name:

```c
#define TPP_EXTNAME_TOK_PASCAL_COMMENT "tok-pascal-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("(* a b c *)") <= 1
...
#endif
```
</details>

## TPP_HAVE_TOK_PASCAL_BRACE_COMMENT

Enable support for recognizing pascal brace-like comments: `{ like this one! }`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension name:

```c
#define TPP_EXTNAME_TOK_PASCAL_BRACE_COMMENT "tok-pascal-brace-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("{ a b c }") <= 1
...
#endif
```
</details>

## TPP_HAVE_TOK_HTML_COMMENT

Enable support for recognizing html-like comments: `<!-- like this one -->`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension name:

```c
#define TPP_EXTNAME_TOK_HTML_COMMENT "tok-html-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("<!-- a b c -->") <= 1
...
#endif
```
</details>

## TPP_HAVE_TOK_SQL_COMMENT

Enable support for recognizing SQL-like comments: `-- like this one!`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension name:

```c
#define TPP_EXTNAME_TOK_SQL_COMMENT "tok-sql-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("-- a b c") <= 1
...
#endif
```
</details>

## TPP_HAVE_TOK_AT_AT_COMMENT

Enable support for recognizing deemon-doc-like comments: `@@ like this one!`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension name:

```c
#define TPP_EXTNAME_TOK_AT_AT_COMMENT "tok-at-at-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("@@ a b c") <= 1
...
#endif
```
</details>

## TPP_HAVE_TOK_SHELL_COMMENT

Enable support for recognizing shell-like comments: `# like this one!`

This still works in conjunction with [`TPP_HAVE_CPP_DIRECTIVES`](#tpp_have_cpp_directives), in
that unknown directives will simply be re-emit as shell comments,
and shell comments that don't appear at the start of lines are not
even processed as CPP directives.

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension name:

```c
#define TPP_EXTNAME_TOK_SHELL_COMMENT "tok-shell-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("foo# a b c") <= 2
...
#endif
```
</details>

## TPP_HAVE_TOK_SLASH_COMMENT

Enable support for recognizing `/`-like comments anywhere: `/ like this one!`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension name:

```c
#define TPP_EXTNAME_TOK_SLASH_COMMENT "tok-slash-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("foo/ a b c") <= 2
...
#endif
```
</details>

## TPP_HAVE_TOK_AT_COMMENT

Enable support for recognizing `@`-like comments anywhere: `@ like this one!`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension name:

```c
#define TPP_EXTNAME_TOK_AT_COMMENT "tok-at-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("foo@ a b c") <= 2
...
#endif
```
</details>

## TPP_HAVE_TOK_SOL_SHELL_COMMENT

Same as [`TPP_HAVE_TOK_SHELL_COMMENT`](#tpp_have_tok_shell_comment), but only recognized when the `#`
appears as the first character of the relevant line, or is preceded by
nothing but whitespace.

Due to limitations related to when/how the contents of a [`tpp_file`](../src/tpp-amalgamation.h#L15870) can
be unloaded, said preceding whitespace will be considered part of the
[`TPP_TOK_SOL_SHELL_COMMENT`](../src/tpp-amalgamation.h#L11311) token)

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension name:

```c
#define TPP_EXTNAME_TOK_SOL_SHELL_COMMENT "tok-sol-shell-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("  # a b c") == __TPP_COUNT_TOKENS("# a b c")
...
#endif
```
</details>

## TPP_HAVE_TOK_SOL_SLASH_COMMENT

Same as [`TPP_HAVE_TOK_SLASH_COMMENT`](#tpp_have_tok_slash_comment), but only recognized when the `/`
appears as the first character of the relevant line, or is preceded by
nothing but whitespace (any preceding whitespace will be part of the
resulting [`TPP_TOK_SOL_SLASH_COMMENT`](../src/tpp-amalgamation.h#L11317) token; see [`TPP_HAVE_TOK_SOL_SHELL_COMMENT`](#tpp_have_tok_sol_shell_comment))

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension name:

```c
#define TPP_EXTNAME_TOK_SOL_SLASH_COMMENT "tok-sol-slash-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("  / a b c") == __TPP_COUNT_TOKENS("/ a b c")
...
#endif
```
</details>

## TPP_HAVE_TOK_SOL_AT_COMMENT

Same as [`TPP_HAVE_TOK_SLASH_COMMENT`](#tpp_have_tok_slash_comment), but only recognized when the `@`
appears as the first character of the relevant line, or is preceded by
nothing but whitespace (any preceding whitespace will be part of the
resulting [`TPP_TOK_SOL_AT_COMMENT`](../src/tpp-amalgamation.h#L11323) token; see [`TPP_HAVE_TOK_SOL_SHELL_COMMENT`](#tpp_have_tok_sol_shell_comment))

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension name:

```c
#define TPP_EXTNAME_TOK_SOL_AT_COMMENT "tok-sol-at-comment"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("  @ a b c") == __TPP_COUNT_TOKENS("@ a b c")
...
#endif
```
</details>

## TPP_HAVE_TOK_DOLLAR

When enabled, `$` is treated as its own token, rather than as part of identifiers/keywords.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) ? TPP_CONF_EXT0 : 0
```

Extension name:

```c
#define TPP_EXTNAME_TOK_DOLLAR "tok-dollar"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("a$b") == 3
...
#endif
```
</details>

## TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE

Allow use of `_` as a thousands separator `123_456_789`
in [`TPP_HAVE_TOK_C_INT`](#tpp_have_tok_c_int) and [`TPP_HAVE_TOK_C_FLOAT`](#tpp_have_tok_c_float)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TOK_C_INT ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_THOUSANDS_SEPARATOR_UNDERSCORE "thousands-separator-underscore"
```
</details>

## TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK

Allow use of `'` as a thousands separator `123'456'789`
in [`TPP_HAVE_TOK_C_INT`](#tpp_have_tok_c_int) and [`TPP_HAVE_TOK_C_FLOAT`](#tpp_have_tok_c_float)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_TOK_C_INT ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_THOUSANDS_SEPARATOR_SINGLETICK "thousands-separator-singletick"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("123'456'789") == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_C_INT

C-style integer token `123`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_GENERIC
```

Extension name:

```c
#define TPP_EXTNAME_TOK_C_INT "tok-c-int"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("123") == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_PASCAL_HEX

Pascal-style hex token `$DEADBEEF`. When disabled, such constructs would
instead be treated as identifiers (depending on [`TPP_HAVE_TOK_DOLLAR`](#tpp_have_tok_dollar))

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
```

Extension name:

```c
#define TPP_EXTNAME_TOK_PASCAL_HEX "tok-pascal-hex"
```
</details>

## TPP_HAVE_TOK_C_FLOAT

C-style float token `123.0`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_GENERIC
```

Extension name:

```c
#define TPP_EXTNAME_TOK_C_FLOAT "tok-c-float"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("123.0") == 1
...
#endif
```
</details>

## TPP_HAVE_SMART_FLOAT_TOKENS

(Try to) be smarter regarding how float tokens are detected.
This tries to detect some syntax-error floating point tokens
and terminates them in those places where you must have wanted
them to terminate (even though standards say that they should
keep going):

| Input       | Standard parsing                | Smart parsing                         | Comment  |
| ----------- | ------------------------------- | ------------------------------------- | -------- |
| `1E2`       | `[FLOAT:1E2]`                   | `[FLOAT:1E2]`                         | - |
| `1P2`       | `[INT:1P2]`                     | `[INT:1P2]`                           | - |
| `0x1E2`     | `[INT:0x1E2]`                   | `[INT:0x1E2]`                         | - |
| `0x1P2`     | `[FLOAT:0x1P2]`                 | `[FLOAT:0x1P2]`                       | - |
| `0xE12`     | `[INT:0xE12]`                   | `[INT:0xE12]`                         | - |
| `0xE+12`    | `[FLOAT:0xE+12]`                | `[INT:0xE][PLUS:+][INT:12]`           | Exponent after `x` / `X` must use `p` or `P` |
| `0x1E+12`   | `[FLOAT:0x1E+12]`               | `[INT:0x1E][PLUS:+][INT:12]`          | Exponent after `x` / `X` must use `p` or `P` |
| `0E+12`     | `[FLOAT:0E+12]`                 | `[FLOAT:0E+12]`                       | - |
| `0xP12`     | `[FLOAT:0xP12]`                 | `[INT:0xP12]`                         | Exponent cannot directly appear after `x` / `X` |
| `0xP+12`    | `[FLOAT:0xP+12]`                | `[INT:0xP][PLUS:+][INT:12]`           | Exponent cannot directly appear after `x` / `X` |
| `0x1P12`    | `[FLOAT:0x1P12]`                | `[FLOAT:0x1P12]`                      | - |
| `0x1P+12`   | `[FLOAT:0x1P+12]`               | `[FLOAT:0x1P+12]`                     | - |
| `0P+12`     | `[FLOAT:0P+12]`                 | `[INT:0P][PLUS:+][INT:12]`            | Without `x` / `X`, must use `e` or `E` for exponents |
| `0xA12`     | `[INT:0xA12]`                   | `[INT:0xA12]`                         | - |
| `0xA+12`    | `[INT:0xA][PLUS:+][INT:12]`     | `[INT:0xA][PLUS:+][INT:12]`           | - |
| `0x1A+12`   | `[INT:0x1A][PLUS:+][INT:12]`    | `[INT:0x1A][PLUS:+][INT:12]`          | - |
| `0A+12`     | `[INT:0A][PLUS:+][INT:12]`      | `[INT:0A][PLUS:+][INT:12]`            | - |
| `0x0.P12`   | `[FLOAT:0x0.P12]`               | `[FLOAT:0x0.P12]`                     | - |
| `0x0.P+12`  | `[FLOAT:0x0.P+12]`              | `[FLOAT:0x0.P+12]`                    | - |
| `0x0.1P+12` | `[FLOAT:0x0.1P+12]`             | `[FLOAT:0x0.1P+12]`                   | - |
| `0x.P12`    | `[FLOAT:0x.P12]`                | `[FLOAT:0x.P12]`                      | - |
| `0x.P+12`   | `[FLOAT:0x.P+12]`               | `[FLOAT:0x.P+12]`                     | - |
| `0x.1P+12`  | `[FLOAT:0x.1P+12]`              | `[FLOAT:0x.1P+12]`                    | - |
| `0x.E12`    | `[FLOAT:0x.E12]`                | `[FLOAT:0x.E12]`                      | - |
| `0x.E+12`   | `[FLOAT:0x.E+12]`               | `[FLOAT:0x.E][PLUS:+][INT:12]`        | With `x` / `X`, must use `p` or `P` for exponents |
| `0x.1E+12`  | `[FLOAT:0x.1E+12]`              | `[FLOAT:0x.1E][PLUS:+][INT:12]`       | With `x` / `X`, must use `p` or `P` for exponents |
| `0.P+12`    | `[FLOAT:0.P+12]`                | `[INT:0][DOT:.][P:P][PLUS:+][INT:12]` | Without `x` / `X`, must use `e` or `E` for exponents |
| `0.E+12`    | `[FLOAT:0.E+12]`                | `[FLOAT:0.E+12]`                      | - |
| `0x.A12`    | `[FLOAT:0x.A12]`                | `[FLOAT:0x.A12]`                      | - |
| `0x.A+12`   | `[FLOAT:0x.A][PLUS:+][INT:12]`  | `[FLOAT:0x.A][PLUS:+][INT:12]`        | - |
| `0x.1A+12`  | `[FLOAT:0x.1A][PLUS:+][INT:12]` | `[FLOAT:0x.1A][PLUS:+][INT:12]`       | - |
| `0.A+12`    | `[FLOAT:0.A][PLUS:+][INT:12]`   | `[INT:0][DOT:.][A:A][PLUS:+][INT:12]` | Without `x` / `X`, first character after `.` must be 0-9 |
| `012`       | `[INT:012]`                     | `[INT:012]`                           | - |
| `0.12`      | `[FLOAT:0.12]`                  | `[FLOAT:0.12]`                        | - |
| `0..12`     | `[FLOAT:0..12]`                 | `[INT:0][DOT_DOT:..][INT:12]`         | There can be at most 1 decimal-`.` |
| `0foo`      | `[INT:0foo]`                    | `[INT:0foo]`                          | - |
| `0.foo`     | `[FLOAT:0.foo]`                 | `[INT:0][DOT:.][foo:foo]`             | Hex-character `f` after `.` requires `x` / `X` |
| `0..foo`    | `[FLOAT:0..foo]`                | `[INT:0][DOT_DOT:..][foo:foo]`        | There can be at most 1 decimal-`.` |
| `0xfoo`     | `[INT:0xfoo]`                   | `[INT:0xfoo]`                         | - |
| `0x.foo`    | `[FLOAT:0x.foo]`                | `[FLOAT:0x.foo]`                      | - |
| `0x..foo`   | `[FLOAT:0x..foo]`               | `[INT:0x][DOT_DOT:..][foo:foo]`       | There can be at most 1 decimal-`.` |
| `0x1foo`    | `[INT:0x1foo]`                  | `[INT:0x1foo]`                        | - |
| `0x1.foo`   | `[FLOAT:0x1.foo]`               | `[FLOAT:0x1.foo]`                     | - |
| `0x1..foo`  | `[FLOAT:0x1..foo]`              | `[INT:0x1][DOT_DOT:..][foo:foo]`      | There can be at most 1 decimal-`.` |
| `0luz`      | `[INT:0luz]`                    | `[INT:0luz]`                          | - |
| `0.luz`     | `[FLOAT:0.luz]`                 | `[INT:0][DOT:.][luz:luz]`             | Character after `.` must be decimal |
| `0..luz`    | `[FLOAT:0..luz]`                | `[INT:0][DOT_DOT:..][luz:luz]`        | There can be at most 1 decimal-`.` |
| `0xluz`     | `[INT:0xluz]`                   | `[INT:0xluz]`                         | - |
| `0x.luz`    | `[FLOAT:0x.luz]`                | `[INT:0x][DOT:.][luz:luz]`            | Character after `.` must be hexadecimal |
| `0x..luz`   | `[FLOAT:0x..luz]`               | `[INT:0x][DOT_DOT:..][luz:luz]`       | There can be at most 1 decimal-`.` |
| `0x1luz`    | `[INT:0x1luz]`                  | `[INT:0x1luz]`                        | - |
| `0x1.luz`   | `[FLOAT:0x1.luz]`               | `[INT:0x1][DOT:.][luz:luz]`           | Character after `.` must be hexadecimal |
| `0x1..luz`  | `[FLOAT:0x1..luz]`              | `[INT:0x1][DOT_DOT:..][luz:luz]`      | There can be at most 1 decimal-`.` |
| `12`        | `[INT:12]`                      | `[INT:12]`                            | - |
| `.12`       | `[FLOAT:.12]`                   | `[FLOAT:.12]`                         | - |
| `..12`      | `[DOT_DOT:..][INT:12]`          | `[DOT_DOT:..][INT:12]`                | There can be at most 1 decimal-`.` |

<details><summary>Details</summary>

Default:

```c
((TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) && TPP_HAVE_TOK_C_FLOAT) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : TPP_CONF_FEAT1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_SMART_FLOAT_TOKENS "smart-float-tokens"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("0x1P+12") == 1 && __TPP_COUNT_TOKENS("0xE+12") == 3
...
#endif
```
</details>

## TPP_HAVE_TOK_C_CHAR

Support for C character literals: `'foo'`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_GENERIC
```

Extension name:

```c
#define TPP_EXTNAME_TOK_C_CHAR "tok-char"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("'foo'") == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_C_STRING

Support for C string literals: `"foo"`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_GENERIC
```

Extension name:

```c
#define TPP_EXTNAME_TOK_C_STRING "tok-string"
```

Detect:

```c
#if __TPP_COUNT_TOKENS('"foo"') == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL

Support for string literals: `R"AB(foo)AB"`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_CXX_RAW_STRING_LITERAL "tok-cxx-raw-string-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS('R"AB(foo)AB"') == 1 && __TPP_STR_SIZE(R"AB(foo)AB") == 3
...
#endif
```
</details>

## TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL

Support for string literals: `L"foo"`
When [`TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL`](#tpp_have_tok_cxx_raw_string_literal) is also enabled, also support `LR"AB(foo)AB")`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_CXX_WIDE_STRING_LITERAL "tok-cxx-wide-string-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS('L"foo"') == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL

Support for string literals: `u8"foo"`
When [`TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL`](#tpp_have_tok_cxx_raw_string_literal) is also enabled, also support `u8R"AB(foo)AB"`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_CXX_UTF8_STRING_LITERAL "tok-cxx-utf8-string-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS('u8"foo"') == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL

Support for string literals: `u"foo"`
When [`TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL`](#tpp_have_tok_cxx_raw_string_literal) is also enabled, also support `uR"AB(foo)AB"`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_CXX_UTF16_STRING_LITERAL "tok-cxx-utf16-string-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS('u"foo"') == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL

Support for string literals: `U"foo"`
When [`TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL`](#tpp_have_tok_cxx_raw_string_literal) is also enabled, also support `UR"AB(foo)AB"`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_CXX_UTF32_STRING_LITERAL "tok-cxx-utf32-string-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS('U"foo"') == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL

Support for string literals: `R'AB(f)AB'`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_CXX_RAW_CHAR_LITERAL "tok-cxx-raw-char-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("R'AB(foo)AB'") == 1 && __TPP_STR_SIZE(R'AB(foo)AB') == 3
...
#endif
```
</details>

## TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL

Support for string literals: `L'f'`
When [`TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL`](#tpp_have_tok_cxx_raw_char_literal) is also enabled, also support `LR'AB(f)AB'`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_CXX_WIDE_CHAR_LITERAL "tok-cxx-wide-char-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("L'f'") == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL

Support for string literals: `u8'f'`
When [`TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL`](#tpp_have_tok_cxx_raw_char_literal) is also enabled, also support `u8R'AB(f)AB'`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_CXX_UTF8_CHAR_LITERAL "tok-cxx-utf8-char-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("u8'f'") == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL

Support for string literals: `u'f'`
When [`TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL`](#tpp_have_tok_cxx_raw_char_literal) is also enabled, also support `uR'AB(f)AB'`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_CXX_UTF16_CHAR_LITERAL "tok-cxx-utf16-char-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("u'f'") == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL

Support for string literals: `U'f'`
When [`TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL`](#tpp_have_tok_cxx_raw_char_literal) is also enabled, also support `UR'AB(f)AB'`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_CXX_UTF32_CHAR_LITERAL "tok-cxx-utf32-char-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("U'f'") == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_RAW_STRING_LITERAL

Support for deemon/python-style raw string literals: `r"foo"`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_RAW_STRING_LITERAL "tok-raw-string-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS('R"foo"') == 1 && __TPP_STR_SIZE(R"AB(foo)AB") == 9
...
#endif
```
</details>

## TPP_HAVE_TOK_RAW_CHAR_LITERAL

Support for deemon/python-style raw string literals: `r'bar'`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_RAW_CHAR_LITERAL "tok-raw-char-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS('R"foo"') == 1 && __TPP_STR_SIZE(R'AB(foo)AB') == 9
...
#endif
```
</details>

## TPP_HAVE_TOK_BLOCK_STRING_LITERAL

Support for java-style block string literals: `"""foo"""`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_BLOCK_STRING_LITERAL "tok-block-string-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS('"""\n a\n b"""') == 1
...
#endif
```
</details>

## TPP_HAVE_TOK_BLOCK_CHAR_LITERAL

Support for java-style block string literals (but with single-ticks): `'''foo'''`

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
```

Extension name:

```c
#define TPP_EXTNAME_TOK_BLOCK_CHAR_LITERAL "tok-block-char-literal"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("'''\n a\n b'''") == 1
...
#endif
```
</details>

## TPP_HAVE_STRING_ESCAPE_E

Support for `\e` (for `U+001B`) escape sequences:

```c
printf("Error: \e[31m%d\e[0m", errno);
```

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_E "string-escape-e"
```
</details>

## TPP_HAVE_STRING_ESCAPE_S

Support for `\s` (for `U+0020`) escape sequences:

```java
System.out.println("""
    This line has visible trailing whitespace:    \s
    """);
```

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT0 : 0) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_S "string-escape-s"
```
</details>

## TPP_HAVE_STRING_ESCAPE_XML

Support for [D](https://en.wikipedia.org/wiki/D_(programming_language))-like
escape sequences in strings:

```c
char const *tpp1 = "Tiny\&nbsp;PreProcessor";
char const *tpp2 = "Tiny\u0080PreProcessor"; // Same as this
```

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_XML "string-escape-xml"
```
</details>

## TPP_HAVE_STRING_ESCAPE_OCT

Support for `\123` octal sequences (with `1`-`3` characters in range `0-7` following the `\`)

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_OCT "string-escape-oct"
```
</details>

## TPP_HAVE_STRING_ESCAPE_OCT_BRACE

Support for `\o{377}` as alias for `\377`

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_OCT_BRACE "string-escape-oct-brace"
```
</details>

## TPP_HAVE_STRING_ESCAPE_OCT_BRACE_MANY

Support for `\o{ 0 037 , 377 }` as alias for `\0\037\377`.
This is an extension to [`TPP_HAVE_STRING_ESCAPE_OCT_BRACE`](#tpp_have_string_escape_oct_brace),
meaning it also requires that extension to be enabled to work.

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE_OCT_BRACE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 0) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_OCT_BRACE_MANY "string-escape-oct-brace-many"
```
</details>

## TPP_HAVE_STRING_ESCAPE_HEX

Support for `\xAB` hex sequences (with `1`-`2` characters in range `0-9`, `a-f`, `A-F` following the `\`)
When [`TPP_HAVE_STRING_ESCAPE_HEX_BIG`](#tpp_have_string_escape_hex_big) is also enabled, the limit of `2` characters is lifted.

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_HEX "string-escape-hex"
```
</details>

## TPP_HAVE_STRING_ESCAPE_HEX_BIG

Support for `\xABCDEF` hex sequences. Extension to [`TPP_HAVE_STRING_ESCAPE_HEX`](#tpp_have_string_escape_hex) that allows more than `2`
hex nibbles to be specified. A warning [`TPP_W_CHARACTER_TOO_LARGE`](../src/tpp-amalgamation.h#L3989) is emitted if the hex-sequence is too
large to fit into [`tpp_uintmax`](../src/tpp-amalgamation.h#L4455), or the output string format.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRING_ESCAPE_HEX ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT0 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_HEX_BIG "string-escape-hex-big"
```
</details>

## TPP_HAVE_STRING_ESCAPE_HEX_BRACE

Support for `\o{377}` as alias for `\377`

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE_HEX && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_HEX_BRACE "string-escape-hex-brace"
```
</details>

## TPP_HAVE_STRING_ESCAPE_HEX_BRACE_MANY

Support for `\o{ 0 037 , 377 }` as alias for `\0\037\377`.
This is an extension to [`TPP_HAVE_STRING_ESCAPE_HEX_BRACE`](#tpp_have_string_escape_hex_brace),
meaning it also requires that extension to be enabled to work.

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE_HEX_BRACE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 0) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_HEX_BRACE_MANY "string-escape-hex-brace-many"
```
</details>

## TPP_HAVE_STRING_ESCAPE_UNI

Support for `\u1234` and `\U12345678` unicode ordinal escape sequences.

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_UNI "string-escape-uni"
```
</details>

## TPP_HAVE_STRING_ESCAPE_UNI_BRACE

Support for `\u{12345678}` unicode ordinal escape sequences.

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_UNI_BRACE "string-escape-uni-brace"
```
</details>

## TPP_HAVE_STRING_ESCAPE_UNI_BRACE_MANY

Support for `\u{ 1234 5678 , ABCDEF }` as alias for `\u1234\u5678\U00ABCDEF`.
This is an extension to [`TPP_HAVE_STRING_ESCAPE_UNI_BRACE`](#tpp_have_string_escape_uni_brace),
meaning it also requires that extension to be enabled to work.

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE_UNI_BRACE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 0) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_UNI_BRACE_MANY "string-escape-uni-brace-many"
```
</details>

## TPP_HAVE_STRING_ESCAPE_NAMED

Support for `\N{...}` unicode ordinal escape sequences in strings.
What actually is allowed within `\N{...}` is controlled by:

- [`TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES`](#tpp_have_escape_named_unicode_names)
- [`TPP_HAVE_ESCAPE_NAMED_UNICODE_ORD`](#tpp_have_escape_named_unicode_ord)
- [`TPP_HAVE_ESCAPE_NAMED_XML`](#tpp_have_escape_named_xml)

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_STRING_ESCAPE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_NAMED "string-escape-named"
```
</details>

## TPP_HAVE_STRING_ESCAPE_NAMED_MANY

Support for `\N{FOO, BAR}` as alias for `\N{FOO}\N{BAR}`.
This is an extension to [`TPP_HAVE_STRING_ESCAPE_NAMED`](#tpp_have_string_escape_named) and [`TPP_HAVE_STRING_ESCAPE_NAMED`](#tpp_have_string_escape_named),
meaning it takes no effect if neither of those extensions is enabled.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRING_ESCAPE_NAMED ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ESCAPE_NAMED_MANY "string-escape-named-many"
```
</details>

## TPP_HAVE_STRING_ALLOW_MULTILINE

Feature-flag: treat line-feeds like any regular character in string tokens:

- [`TPP_HAVE_TOK_C_STRING`](#tpp_have_tok_c_string)
- [`TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL`](#tpp_have_tok_cxx_wide_string_literal)
- [`TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL`](#tpp_have_tok_cxx_utf8_string_literal)
- [`TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL`](#tpp_have_tok_cxx_utf16_string_literal)
- [`TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL`](#tpp_have_tok_cxx_utf32_string_literal)
- [`TPP_HAVE_TOK_RAW_STRING_LITERAL`](#tpp_have_tok_raw_string_literal)
- [`TPP_HAVE_TOK_C_CHAR`](#tpp_have_tok_c_char)
- [`TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL`](#tpp_have_tok_cxx_wide_char_literal)
- [`TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL`](#tpp_have_tok_cxx_utf8_char_literal)
- [`TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL`](#tpp_have_tok_cxx_utf16_char_literal)
- [`TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL`](#tpp_have_tok_cxx_utf32_char_literal)
- [`TPP_HAVE_TOK_RAW_CHAR_LITERAL`](#tpp_have_tok_raw_char_literal)

When this flag is disabled, line-feeds in such string tokens will instead
terminate the string, and cause a [`TPP_W_STRING_TERMINATED_BY_LINEFEED`](../src/tpp-amalgamation.h#L3139)
warning to be emitted.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) ? TPP_CONF_FEAT0 : 0
```

Extension name:

```c
#define TPP_EXTNAME_STRING_ALLOW_MULTILINE "string-allow-multiline"
```

Detect:

```c
#if __TPP_COUNT_TOKENS("\"\n\"") == 1
...
#endif
```
</details>

## TPP_HAVE_STRING_AUTO_CONCAT

Enable support for automatic concatenation of adjacent string tokens.
This affects the behavior of [`tpp_lexer_parsestring_ex()`](../src/tpp-amalgamation.h#L21221) and its
companion [`tpp_lexer_parsestring_cb()`](../src/tpp-amalgamation.h#L21261), such that they will only yield
to the next token, but not check if that next token might be another
string.

This feature should always be enabled for C/C++ (since their standards
mandate that adjacent string tokens be joined into a single literal),
but other languages might not want such behavior.

WARNING: TPP will automatically join any type of string-like-token with any
         other when this feature is enabled (e.g. `"FOO" 'BAR' R"AB(BAZ)AB"`
         becomes a single string literal `"FOOBARBAZ"`)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```

Extension name:

```c
#define TPP_EXTNAME_STRING_AUTO_CONCAT "string-auto-concat"
```
</details>

## TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES

Support for unicode names in `\N` like `\N{LATIN SMALL LETTER B}`.
This feature affects the behavior of:

- [`TPP_HAVE_STRING_ESCAPE_NAMED`](#tpp_have_string_escape_named)
- [`TPP_HAVE_IDENTIFIER_ESCAPE_NAMED`](#tpp_have_identifier_escape_named)

With this enabled, you can write stuff like this:

```c
"Happy\N{SPACE, CANDLE, BIRTHDAY CAKE, CANDLE, SPACE}to\N{SPACE}you\N{EXCLAMATION MARK}"
```

For more information, see [`TPP_HAVE_UNICODE_BYNAME_LOOKUP`](config-core.md#tpp_have_unicode_byname_lookup), which
is enabled based on this config, and controls the availability of
the internal API required for this feature.

Because of the amount of data added by this feature, it isn't
automatically enabled under [`TPP_PROFILE_DEFAULT`](../src/tpp-amalgamation.h#L4992), so if you
want it, you have to enable is manually.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_DECODE_NAMED_ESCAPE ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_ESCAPE_NAMED_UNICODE_NAMES "named-escape-unicode"
```
</details>

## TPP_HAVE_ESCAPE_NAMED_UNICODE_ORD

Support for unicode ordinals in `\N` (all of which are the same as `\u0100`):

- `\N{U+0100}`
- `\N{U+100}`
- `\N{U+000100}`
- `\N{0x0100}`
- `\N{256}`

This feature affects the behavior of:

- [`TPP_HAVE_STRING_ESCAPE_NAMED`](#tpp_have_string_escape_named)
- [`TPP_HAVE_IDENTIFIER_ESCAPE_NAMED`](#tpp_have_identifier_escape_named)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_DECODE_NAMED_ESCAPE ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_ESCAPE_NAMED_UNICODE_ORD "named-escape-ord"
```
</details>

## TPP_HAVE_ESCAPE_NAMED_XML

Support for XML/HTML5 names in `\N`: `\N{&tab;}`

This feature affects the behavior of:

- [`TPP_HAVE_STRING_ESCAPE_NAMED`](#tpp_have_string_escape_named)
- [`TPP_HAVE_IDENTIFIER_ESCAPE_NAMED`](#tpp_have_identifier_escape_named)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_DECODE_NAMED_ESCAPE ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : 0) : 0
```

Extension name:

```c
#define TPP_EXTNAME_ESCAPE_NAMED_XML "named-escape-xml"
```
</details>

## TPP_HAVE_UNICODE_BYNAME_LOOKUP_ICASE

Ignore casing (which is normally all-uppercase) inside of [`TPP_HAVE_UNICODE_BYNAME_LOOKUP`](config-core.md#tpp_have_unicode_byname_lookup)

When this is enabled, the following 2 are identical:

```c
foo\N{LATIN SMALL LETTER B}ar
foo\N{latin small letter b}ar
```

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_UNICODE_BYNAME_LOOKUP ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_UNICODE_BYNAME_LOOKUP_ICASE "named-escape-unicode-icase"
```
</details>

## TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE

Whitespace (and `_`) are ignored by [`TPP_HAVE_UNICODE_BYNAME_LOOKUP`](config-core.md#tpp_have_unicode_byname_lookup) whenever they
aren't required to distinguish distinct tokens. When this extension is combined with
[`TPP_HAVE_UNICODE_BYNAME_LOOKUP_ICASE`](#tpp_have_unicode_byname_lookup_icase), TPP's unicode name parser pretty much satisfies
[UAX44-LM2](https://www.unicode.org/reports/tr44/tr44-24.html#UAX44-LM2)

When this is enabled, the following 2 are identical:

```c
foo\N{LATIN SMALL LETTER B}ar
foo\N{LATINSMALLLETTERB}ar
```

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_UNICODE_BYNAME_LOOKUP ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT0 : 0) : 0
```

Extension name:

```c
#define TPP_EXTNAME_UNICODE_BYNAME_LOOKUP_ISPACE "named-escape-unicode-icase"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_DEFINED

Enable support for `defined(MACRO)` in builtin lexer expressions

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_DEFINED "defined-in-expressions"
```
</details>

## TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR

Enable special handling in `#define foo(x) defined(x)` such that `x` is not expanded.
Irregardless of this feature being enabled or not, a warning [`TPP_W_EXPANSION_TO_DEFINED`](../src/tpp-amalgamation.h#L3709)
is emitted whenever a construct `defined(<param>)` or `defined <param>` is encountered
within the body of a function-style macro definition, where `<param>` is the name of one
of the macro's parameters (see [`TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED`](config-warn.md#tpp_have_tpp_w_expansion_to_defined)).

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_EXPR_DEFINED && TPP_HAVE_CPP_MACROS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? TPP_CONF_EXT0 : 0
```

Extension name:

```c
#define TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR "dont-expand-defined"
```

Detect:

```c
#define test(x) defined(x)
#define test2   NOT_DEFINED
#if test(test2)
...
#endif
```
</details>

## TPP_HAVE_BUILTIN_EXPR_STRINGS

Enable support for strings in builtin lexer expressions, as well as some new operators:

```c
#if "FOO" != "BAR"
...
#endif

#if "FOO"[0] == 'F'
...
#endif

#if "FOO"[1:] == "OO"
...
#endif

#if #"FOO" == 3
...
#endif
```

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_TOK_STRINGLIKE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_STRINGS "strings-in-expressions"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_FLOATS

Enable support for floats in builtin lexer expressions

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_TOK_C_FLOAT && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_FLOATS "floats-in-expressions"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT

Enable support for `foo ?: bar` in builtin lexer expressions (same as `foo ? foo : bar`)

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT "if-else-optional-true"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS

Enable support for `if (foo) bar else baz` in builtin
lexer expressions, as alias for `foo ? bar : baz`.

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 0
```

Extension name:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS "ifelse-in-expressions"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR

Enable support for `^^` in builtin lexer expressions

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_LOGICAL_XOR "logical-xor-in-expressions"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS

Treat `'a'` in expressions as an integer, rather than as a string (in C/C++, this is always the case)

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_TOK_STRINGLIKE_SQUOTE) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_FEAT1 : ((TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX) ? 1 : 0)) : 0
```

Extension name:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_CHARACTER_LITERALS "character-literals"
```
</details>

## TPP_HAVE_EXTERN_C_FOR_SYSHDR

When [`TPP_FILE_FLAGS_SYSHDR`](../src/tpp-amalgamation.h#L15775) is set during `#include` (i.e. *NOT* via `#pragma GCC system_header`),
then the [`TPP_FILE_FLAGS_EXTERN_C`](../src/tpp-amalgamation.h#L15778) flag should be set alongside [`TPP_FILE_FLAGS_SYSHDR`](../src/tpp-amalgamation.h#L15775).

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_FILE_SYSHDR && TPP_HAVE_FILE_EXTERN_C ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT0 : TPP_CONF_FEAT0) : 0
```

Extension name:

```c
#define TPP_EXTNAME_EXTERN_C_FOR_SYSHDR "extern-c-for-syshdr"
```
</details>

## TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH

Config option to skip searching [`TPP_CONFIG_SYSTEM_INCLUDE_PATH`](config-value.md#tpp_config_system_include_path).
Can (and should) be used to implement `-nostdinc`

<details><summary>Details</summary>

Default:

```c
TPP_TUPLE_NONEMPTY(TPP_CONFIG_SYSTEM_INCLUDE_PATH) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_SEARCH_SYSTEM_INCLUDE_PATH "stdinc"
```
</details>

## TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE

`"`-quoted `#include`-strings are searched relative to *every* I/O-file found on the
`#include`-stack; not just the most-recent one. Doing this for all files is what TPP2
always- and unconditionally did, but turns out that isn't actually something normally
done by preprocessors. As such, TPP3 turns this behavior into an extension, but has
it turned off by default.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL && TPP_HAVE_INCLUDE_STACK) ? TPP_CONF_EXT0 : 0
```

Extension name:

```c
#define TPP_EXTNAME_INCLUDE_RELATIVE_TO_EVERY_FILE "include-relative-to-every-file"
```
</details>

## TPP_HAVE_LEXER_DECODEINT_HEX_LITERALS

Enable support for `0x` literals in [`tpp_lexer_decodeint()`](../src/tpp-amalgamation.h#L21045) when parsing [`TPP_TOK_C_INT`](../src/tpp-amalgamation.h#L11186) (see [`TPP_HAVE_TOK_C_INT`](#tpp_have_tok_c_int))

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_LEXER_DECODEINT && (TPP_PROFILE != TPP_PROFILE_MINIMAL) && TPP_HAVE_TOK_C_INT) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_LEXER_DECODEINT_HEX_LITERALS "hex-literals"
```
</details>

## TPP_HAVE_LEXER_DECODEINT_BINARY_LITERALS

Enable support for `0b` literals in [`tpp_lexer_decodeint()`](../src/tpp-amalgamation.h#L21045) when parsing [`TPP_TOK_C_INT`](../src/tpp-amalgamation.h#L11186) (see [`TPP_HAVE_TOK_C_INT`](#tpp_have_tok_c_int))

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_LEXER_DECODEINT && (TPP_PROFILE != TPP_PROFILE_MINIMAL) && TPP_HAVE_TOK_C_INT) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_LEXER_DECODEINT_BINARY_LITERALS "binary-literals"
```
</details>

## TPP_HAVE_LEXER_DECODEINT_OCTAL_LITERALS

Enable support for `0o` literals in [`tpp_lexer_decodeint()`](../src/tpp-amalgamation.h#L21045) when parsing [`TPP_TOK_C_INT`](../src/tpp-amalgamation.h#L11186) (see [`TPP_HAVE_TOK_C_INT`](#tpp_have_tok_c_int))

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_LEXER_DECODEINT && (TPP_PROFILE != TPP_PROFILE_MINIMAL) && TPP_HAVE_TOK_C_INT) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension name:

```c
#define TPP_EXTNAME_LEXER_DECODEINT_OCTAL_LITERALS "octal-literals"
```
</details>

## TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION

Extra configuration for `#pragma message`: print a leading
[`TPP_CONFIG_FILE_AND_LINE_FORMAT`](config-value.md#tpp_config_file_and_line_format) using the values
that would also be printed by `__FILE__`, `__LINE__`, `__COLUMN__`

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) ? TPP_CONF_EXT0 : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_MESSAGE_PRINTS_LOCATION "pragma-message-prints-location"
```
</details>

## TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED

Extra configuration for `#pragma message`: print a trailing `\n`

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) ? TPP_CONF_EXT0 : 0
```

Extension name:

```c
#define TPP_EXTNAME_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED "pragma-message-omits-trailing-linefeed"
```
</details>

<!--END:conf-->
