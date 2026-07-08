# Feature configs

Every one of these config macros can be defined as one of the following values:

- `TPP_CONF_1`: Compile-time enabled  (always on; no `#pragma extension("-f...")` / `TPP_FEAT_*` available)
- `TPP_CONF_0`: Compile-time disabled
- `TPP_CONF_EXT1`: Runtime-configurable (via `#pragma extension("-f...")` / `TPP_EXT_*`, default = `true`)
- `TPP_CONF_EXT0`: Runtime-configurable (via `#pragma extension("-f...")` / `TPP_EXT_*`, default = `false`)
- `TPP_CONF_FEAT1`: Runtime-configurable (via `TPP_FEAT_*`, default = `true`)
- `TPP_CONF_FEAT0`: Runtime-configurable (via `TPP_FEAT_*`, default = `false`)


<!--BEGIN:conf-->
## TPP_HAVE_BSE

Support for `\`-escaped line continuation: when a line ends with a `\` character
that is immediately (but see [`TPP_HAVE_BSE_WHITESPACE`](config-conf.md#tpp_have_bse_whitespace)) followed by a linefeed,
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
@detect: `#if __TPP_COUNT_TOKENS("a\\\nb") == 1`

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)
```

Extension:

```c
#define TPP_EXTNAME_BSE "bse"
```
</details>

## TPP_HAVE_BSE_WHITESPACE

Extension to [`TPP_HAVE_BSE`](config-conf.md#tpp_have_bse): the `\` character is allowed to be followed by extra
whitespace preceding the actual linefeed:

```c
#define multi_line line1 \␣␣␣
                   line2
```

@detect: `#if __TPP_COUNT_TOKENS("a\\ \nb") == 1`

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_BSE ? TPP_CONF_EXT1 : 0
```

Extension:

```c
#define TPP_EXTNAME_BSE_WHITESPACE "bse-whitespace"
```
</details>

## TPP_HAVE_ESCAPE_IN_IDENTIFIERS

Support for `\uABCD` and `\U01234567` in identifier names (will be
replaced with effective UTF-8 encodings when translated to keywords)
```c
int identifier\u0020with\u0020whitespace = 42;
// Same as:
int __TPP_IDENTIFIER("identifier with whitespace") = 42;
```

@detect: `#if __TPP_COUNT_TOKENS("a\\u1234b") == 1`

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)
```

Extension:

```c
#define TPP_EXTNAME_ESCAPE_IN_IDENTIFIERS "extended-identifiers"
```
</details>

## TPP_HAVE_ESCAPE_E_IN_STRINGS

Support for `\e` (for `U+001B`) escape sequences:

```c
printf("Error: \e[31m%d\e[0m");
```

@detect: N/A

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRING_ESCAPE ? TPP_CONF_EXT1: 0
```

Extension:

```c
#define TPP_EXTNAME_ESCAPE_E_IN_STRINGS "escape-e-in-strings"
```
</details>

## TPP_HAVE_ESCAPE_S_IN_STRINGS

Support for `\s` (for `U+0020`) escape sequences:

```java
System.out.println("""
    This line has visible trailing whitespace:    \s
    """);
```
@detect: N/A

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_STRING_ESCAPE ? TPP_CONF_EXT1 : 0
```

Extension:

```c
#define TPP_EXTNAME_ESCAPE_S_IN_STRINGS "escape-s-in-strings"
```
</details>

## TPP_HAVE_CPP_DIRECTIVES

Specifies if *any* CPP directives are supported
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : ((TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL))
```

Extension:

```c
#define TPP_EXTNAME_CPP_DIRECTIVES "cpp-directives"
```
</details>

## TPP_HAVE_CPP_MACROS

Support for C-style macros
@detect: N/A

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_MACROS "cpp-macros"
```
</details>

## TPP_HAVE_CPP_BUILTIN_MACROS

Support for builtin C-style macros (require [`TPP_HAVE_CPP_MACROS`](config-conf.md#tpp_have_cpp_macros) to be enabled, too)
@detect: N/A

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_BUILTIN_MACROS "cpp-builtin-macros"
```
</details>

## TPP_HAVE_CPP_EXCLAIM

Support for `#!foobar`-directives (which are treated as comments)
@detect: #if __has_known_extension("-fshebang-directives")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_EXCLAIM "shebang-directives"
```
</details>

## TPP_HAVE_CPP_BLANK

Support for `#`-directives (blank directives), which are ignored
@detect: N/A

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension:

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
- `3`: Set `TPP_FILE_FLAGS_SYSHDR` for the current text-file. When this flag is not
       supplied, `TPP_FILE_FLAGS_SYSHDR` is instead cleared for the current text-file.
       This flag requires [`TPP_HAVE_FILE_SYSHDR`](config-core.md#tpp_have_file_syshdr) to be enabled, otherwise it is ignored.
- `4`: Same as flag `3`, except for the `TPP_FILE_FLAGS_EXTERN_C` flag. Similarly, this
       flag requires [`TPP_HAVE_FILE_EXTERN_C`](config-core.md#tpp_have_file_extern_c) to be enabled, otherwise it is ignored.

@detect: N/A

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_DIGIT_LINE "digit-directives"
```
</details>

## TPP_HAVE_CPP_LINE

Support for `#line 42 "foo.h"`-directives
@detect: N/A

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_LINE "line-directives"
```
</details>

## TPP_HAVE_CPP_INCLUDE

Support for `#include <stdio.h>`-directives
@detect: #if __has_known_extension("-finclude-directives")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_INCLUDE "include-directives"
```
</details>

## TPP_HAVE_CPP_INCLUDE_NEXT

Support for `#include_next <stdio.h>`-directives
@detect: #if __has_known_extension("-finclude-next-directives")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_INCLUDE_NEXT "include-next-directives"
```
</details>

## TPP_HAVE_CPP_IMPORT

Support for `#import <stdio.h>`-directives
@detect: #if __has_known_extension("-fimport-directives")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_IMPORT "import-directives"
```
</details>

## TPP_HAVE_CPP_IF_ELSE_ENDIF

Support for: `#if`, `#ifdef`, `#ifndef`, `#elif`, `#elifdef`, `#elifndef`, `#else`, `#endif`
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_DIRECTIVES) ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_IF_ELSE_ENDIF "if-directives"
```
</details>

## TPP_HAVE_CPP_DEFINE

Support for: `#define`, `#undef`
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_DIRECTIVES) ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_DEFINE "define-directives"
```
</details>

## TPP_HAVE_CPP_ASSERT

Support for: `#assert`, `#unassert`
@see: https://gcc.gnu.org/onlinedocs/cpp/Obsolete-Features.html
@detect: #if __has_known_extension("-fassertions")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_ASSERT "assertions"
```
</details>

## TPP_HAVE_CPP_ERROR

Support for: `#error`
@detect: #if __has_known_extension("-ferror-directives")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_ERROR "error-directives"
```
</details>

## TPP_HAVE_CPP_WARNING

Support for: `#warning`
@detect: #if __has_known_extension("-fwarning-directives")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_WARNING "warning-directives"
```
</details>

## TPP_HAVE_CPP_IDENT_SCCS

Support for: `#ident`, `#sccs`
@detect: #if __has_known_extension("-fident-directives")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_IDENT_SCCS "ident-directives"
```
</details>

## TPP_HAVE_CPP_PRAGMA

Support for: `#pragma`
@detect: N/A

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_PRAGMA "pragma-directives"
```
</details>

## TPP_HAVE_CPP_EMBED

Support for: `#embed`
@detect: N/A

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension:

```c
#define TPP_EXTNAME_CPP_EMBED "embed-directives"
```
</details>

## TPP_HAVE_MACRO__Pragma

Support for: `_Pragma("foo")`
@detect: #ifdef _Pragma

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO__Pragma "_Pragma"
```
</details>

## TPP_HAVE_MACRO___pragma

Support for: `__pragma(foo)`
@detect: #ifdef __pragma

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___pragma "__pragma"
```
</details>

## TPP_HAVE_CLANG_MACRO___has_attribute

Support for clang `__has_attribute()`
Check support of `__attribute__((foo))`

@detect: #ifdef __has_attribute

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_attribute "clang-__has_attribute"
```
</details>

## TPP_HAVE_CLANG_MACRO___has_builtin

Support for clang `__has_builtin()`
Check support of `__builtin_foo()`

@detect: #ifdef __has_builtin

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_builtin "clang-__has_builtin"
```
</details>

## TPP_HAVE_CLANG_MACRO___has_cpp_attribute

Support for clang `__has_cpp_attribute()`
Check support of `[[foo]]`

@detect: #ifdef __has_cpp_attribute

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_cpp_attribute "clang-__has_cpp_attribute"
```
</details>

## TPP_HAVE_CLANG_MACRO___has_declspec_attribute

Support for clang `__has_declspec_attribute()`
Check support of `__declspec(foo)`

@detect: #ifdef __has_declspec_attribute

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_declspec_attribute "clang-__has_declspec_attribute"
```
</details>

## TPP_HAVE_CLANG_MACRO___has_extension

Support for clang `__has_extension()`
Check support of compiler extensions

NOTE: Another builtin macro of the same name exists and can be
      enabled via [`TPP_HAVE_MACRO___has_extension`](config-conf.md#tpp_have_macro___has_extension). The 2 macros
      don't conflict with each other though (both can safely be
      enabled at the same time), since TPP's `__has_extension()`
      takes a string, whilst this one takes a keyword/identifier.

@detect: #ifdef __has_extension

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_extension "clang-__has_extension"
```
</details>

## TPP_HAVE_CLANG_MACRO___has_feature

Support for clang `__has_feature()`
Check support of compiler features

@detect: #ifdef __has_feature

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_feature "clang-__has_feature"
```
</details>

## TPP_HAVE_CLANG_MACRO___has_c_attribute

Support for clang `__has_c_attribute()`
Check support of `[[foo]]`

@detect: #ifdef __has_c_attribute

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_CLANG_MACRO___has_c_attribute "clang-__has_c_attribute"
```
</details>

## TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES

When enabled, clang's `__has_feature()` also
expands to `1` when `__has_extension()` would.

See also:

- [`TPP_HAVE_CLANG_MACRO___has_feature`](config-conf.md#tpp_have_clang_macro___has_feature)
- [`TPP_HAVE_CLANG_MACRO___has_extension`](config-conf.md#tpp_have_clang_macro___has_extension)

@detect: #if __has_known_extension("-fclang-extensions-are-features")

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CLANG_MACRO___has_extension && TPP_HAVE_CLANG_MACRO___has_feature) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0
```

Extension:

```c
#define TPP_EXTNAME_CLANG_EXTENSIONS_ARE_FEATURES "clang-extensions-are-features"
```
</details>

## TPP_HAVE_MACRO___is_identifier

Support for clang `__is_identifier()`
Check if something is a builtin identifier:

```c
#if __is_identifier(tpp_exec)
// "#pragma tpp_exec()" is probably supported then...
#endif
```

A keyword is considered to be an "identifier" if `TPP_TOK_ISBUILTINKEYWORD()`

@detect: #ifdef __is_identifier

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___is_identifier "__is_identifier"
```
</details>

## TPP_HAVE_MACRO___is_deprecated

Support for TPP's `__is_deprecated()`
Check if an identifier has been `#pragma deprecated("foo")`-ed

@detect: #ifdef __is_deprecated

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___is_deprecated "__is_deprecated"
```
</details>

## TPP_HAVE_MACRO___is_poisoned

Support for TPP's `__is_poisoned()`
Check if an identifier has been `#pragma GCC poison foo`-ed

@detect: #ifdef __is_poisoned

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___is_poisoned "__is_poisoned"
```
</details>

## TPP_HAVE_MACRO___has_extension

Support for TPP's `__has_extension()`
Check if a TPP extension is known + enabled: `#if __has_extension("-ftrigraphs")`

@detect: #ifdef __has_extension

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_EXTENSIONS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___has_extension "__has_extension"
```
</details>

## TPP_HAVE_MACRO___has_known_extension

Support for TPP's `__has_known_extension()`
Check if a TPP extension is known: `#if __has_known_extension("-ftrigraphs")`

@detect: #ifdef __has_known_extension

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_EXTENSIONS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___has_known_extension "__has_known_extension"
```
</details>

## TPP_HAVE_MACRO___has_warning

Support for TPP's `__has_warning()`
Check if a TPP warning is known + enabled: `#if __has_warning("-Wmultiline-string")`

@detect: #ifdef __has_warning

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___has_warning "__has_warning"
```
</details>

## TPP_HAVE_MACRO___has_known_warning

Support for TPP's `__has_known_warning()`
Check if a TPP warning is known: `#if __has_known_extension("-Wmultiline-string")`

@detect: #ifdef __has_known_warning

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___has_known_warning "__has_known_warning"
```
</details>

## TPP_HAVE_MACRO___has_include

Support for clang `__has_include()`
Check if `#include` exists: `#if __has_include(<stdio.h>)`

@detect: #ifdef __has_include

<details><summary>Details</summary>

Default:

```c
((TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_IMPORT) && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___has_include "__has_include"
```
</details>

## TPP_HAVE_MACRO___has_include_next

Support for clang `__has_include_next()`
Check if `#include_next` exists: `#if __has_include_next(<stdio.h>)`

@detect: #ifdef __has_include_next

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_INCLUDE_NEXT && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___has_include_next "__has_include_next"
```
</details>

## TPP_HAVE_MACRO___has_embed

Support for clang `__has_embed()`
Check if `#embed` exists: `#if __has_embed("resource.dat" limit(10))`

@detect: #ifdef __has_embed

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_EMBED && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___has_embed "__has_embed"
```
</details>

## TPP_HAVE_MACRO___FILE__

Support for the builtin macro `__FILE__`
Expand to current file's name

@detect: #ifdef __FILE__

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___FILE__ "__FILE__"
```
</details>

## TPP_HAVE_MACRO___LINE__

Support for the builtin macro `__LINE__`
Expand to current line number

@detect: #ifdef __LINE__

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___LINE__ "__LINE__"
```
</details>

## TPP_HAVE_MACRO___TIME__

Support for the builtin macro `__TIME__`
Expand to something like `"12:37:58"`

@detect: #ifdef __TIME__

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TIME__ "__TIME__"
```
</details>

## TPP_HAVE_MACRO___DATE__

Support for the builtin macro `__DATE__`
Expand to something like `"Jul  6 2026"`

@detect: #ifdef __DATE__

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___DATE__ "__DATE__"
```
</details>

## TPP_HAVE_MACRO___COLUMN__

Support for the builtin macro `__COLUMN__`
Expand to current column number

@detect: #ifdef __COLUMN__

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___COLUMN__ "column-macro"
```
</details>

## TPP_HAVE_MACRO___BASE_FILE__

Support for the builtin macro `__BASE_FILE__`
Expand to base file's name

@detect: #ifdef __BASE_FILE__

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___BASE_FILE__ "basefile-macro"
```
</details>

## TPP_HAVE_MACRO___FILE_NAME__

Support for the builtin macro `__FILE_NAME__`
Expand to current file's name, without its leading path components.

@see: TPP_HAVE_MACRO___FILE__
@detect: #ifdef __FILE_NAME__

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___FILE_NAME__ "__FILE_NAME__"
```
</details>

## TPP_HAVE_MACRO___INCLUDE_LEVEL__

Support for the builtin macro `__INCLUDE_LEVEL__`
Expand to numerical representation of include depth.
Whilst inside the "base"-file, it expands to `0`

@detect: #ifdef __INCLUDE_LEVEL__

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___INCLUDE_LEVEL__ "include-level-macro"
```
</details>

## TPP_HAVE_MACRO___INCLUDE_DEPTH__

Support for the builtin macro `__INCLUDE_DEPTH__`
Alias for `__INCLUDE_LEVEL__` (see [`TPP_HAVE_MACRO___INCLUDE_LEVEL__`](config-conf.md#tpp_have_macro___include_level__))

@detect: #ifdef __INCLUDE_DEPTH__

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___INCLUDE_DEPTH__ "include-depth-macro"
```
</details>

## TPP_HAVE_MACRO___COUNTER__

Support for the builtin macro `__COUNTER__`
Expand to `1+` its previous expansion. The first time
this macro is used, it expands to `0`

@detect: #ifdef __COUNTER__

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___COUNTER__ "counter-macro"
```
</details>

## TPP_HAVE_MACRO___TIMESTAMP__

Support for the builtin macro `__TIMESTAMP__`
Expand to something like `"Mon Jul  6 12:37:58 2026"`
@detect: #ifdef __TIMESTAMP__

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TIMESTAMP__ "timestamp-macro"
```
</details>

## TPP_HAVE_NUMERIC_DATE_MACROS

Support for the builtin macros `__DATE_DAY__`, `__DATE_WDAY__`, `__DATE_YDAY__`, `__DATE_MONTH__`, `__DATE_YEAR__`
Precise date/time macros (behave like `__LINE__`, but expand to components of `__DATE__`)
@detect: #ifdef __DATE_DAY__, ...

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_NUMERIC_DATE_MACROS "numeric-date-macros"
```
</details>

## TPP_HAVE_NUMERIC_TIME_MACROS

Support for the builtin macros `__TIME_SEC__`, `__TIME_MIN__`, `__TIME_HOUR__`
Precise date/time macros (behave like `__LINE__`, but expand to components of `__TIME__`)
@detect: #ifdef __TIME_SEC__, ...

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_NUMERIC_TIME_MACROS "numeric-time-macros"
```
</details>

## TPP_HAVE_MACRO___TPP_EVAL

Support for the builtin macro `__TPP_EVAL()`

Evaluate an expression like in `#if`, then expand to its result
in the form of 1-2 tokens: `[opt:MINUS][INT]` (or a `"string"`
if [`TPP_HAVE_BUILTIN_EXPR_STRINGS`](config-conf.md#tpp_have_builtin_expr_strings) is enabled):

```c
__TPP_EVAL(10 + 20) // Expands to a single token: 30
```

@detect: #ifdef __TPP_EVAL

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TPP_EVAL "tpp-eval-macro"
```
</details>

## TPP_HAVE_MACRO___TPP_EXEC

Support for the builtin macro `__TPP_EXEC()`

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
      to, use `#pragma tpp_exec()` instead, which has a lower overhead
      and lets you safely execute the code whilst discarding its result

@detect: #ifdef __TPP_EXEC

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TPP_EXEC "tpp-exec-macro"
```
</details>

## TPP_HAVE_MACRO___TPP_UNIQUE

Support for the builtin macro `__TPP_UNIQUE()`
Called with some keyword/identifier, this macro expands a unique INT-token
representative of the given keyword/identifier. The value of that INT-token
is distinct from any other keyword/identifier and remains the same for the
remaining of input (though it will differ if re-run with different input;
this is not a hash-function)

@detect: #ifdef __TPP_UNIQUE

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TPP_UNIQUE "tpp-unique-macro"
```
</details>

## TPP_HAVE_MACRO___TPP_LOAD_FILE

Support for the builtin macro `__TPP_LOAD_FILE()`
Same as `#include`, but package the file's entire contents into a string.
Could be implemented as:

```c
#define __TPP_LOAD_FILE(filename) __TPP_STR_PACK(__TPP_EXEC("#embed " #filename))
```

@detect: #ifdef __TPP_LOAD_FILE

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TPP_LOAD_FILE "tpp-load-file-macro"
```
</details>

## TPP_HAVE_MACRO___TPP_COUNTER

Support for the builtin macro `__TPP_COUNTER()`
Called the same way as `__TPP_UNIQUE` (see [`TPP_HAVE_MACRO___TPP_UNIQUE`](config-conf.md#tpp_have_macro___tpp_unique)),
but returns an ever-increasing value starting at `0` (same as `__COUNTER__`),
but that counter is specific to the given keyword. i.e.: `__TPP_COUNTER(foo)`
and `__TPP_COUNTER(bar)` increment different counters

@detect: #ifdef __TPP_COUNTER

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TPP_COUNTER "tpp-counter-macro"
```
</details>

## TPP_HAVE_MACRO___TPP_RANDOM

Support for the builtin macro `__TPP_RANDOM()`
Overloaded macro taking 1 or 2 arguments, and expanding to a random number
that stays the same during repeated compilations, only changing if source
code is altered:

- The single-argument form `__TPP_RANDOM(hi)` expands to a pseudo-random
  integer token in the range `[0,hi)`
- The 2-argument form `__TPP_RANDOM(lo, hi)` expands to a pseudo-random
  integer token in the range `[lo,hi)`

@detect: #ifdef __TPP_RANDOM

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TPP_RANDOM "tpp-random-macro"
```
</details>

## TPP_HAVE_MACRO___TPP_STR_DECOMPILE

Support for the builtin macro `__TPP_STR_DECOMPILE()`
Very similar to `__TPP_EXEC()` (see [`TPP_HAVE_MACRO___TPP_EXEC`](config-conf.md#tpp_have_macro___tpp_exec)), except
that `__TPP_EXEC()` will expand other macros and directives, while
`__TPP_STR_DECOMPILE()` doesn't: it simply takes a string and expands to
its decoded form *without* expansion (however: expansion may still
occur as returned tokens are read).
In practice this usually only means:

- `__TPP_EXEC()` accepts preprocessor directives
- `__TPP_STR_DECOMPILE()` only does basic string-to-token conversion

@detect: #ifdef __TPP_STR_DECOMPILE

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TPP_STR_DECOMPILE "tpp-str-decompile-macro"
```
</details>

## TPP_HAVE_MACRO___TPP_STR_PACK

Support for the builtin macro `__TPP_STR_PACK()`
Expands to a string literal that is made up of the arguments taken by
this macro. There can be any number of arguments (and arguments don't
even have to be separated by `,`), but every argument must be:

- Another string (that is added to the result after being decoded and re-encoded)
- An INT token (that must evaluate to a value in range `[0,0xFF]`)

@detect: #ifdef __TPP_STR_PACK

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TPP_STR_PACK "tpp-str-pack-macro"
```
</details>

## TPP_HAVE_MACRO___TPP_STR_SUBSTR

Support for the builtin macro `__TPP_STR_SUBSTR()`
Stand-alone macro that takes 3 arguments and (assuming [`TPP_HAVE_BUILTIN_EXPR_STRINGS`](config-conf.md#tpp_have_builtin_expr_strings)
and [`TPP_HAVE_MACRO___TPP_EVAL`](config-conf.md#tpp_have_macro___tpp_eval) are enabled) can be implemented as follows:

```c
#define __TPP_STR_SUBSTR(str, lo, hi) __TPP_EVAL((str)[(lo):(hi)])
```

@detect: #ifdef __TPP_STR_SUBSTR

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TPP_STR_SUBSTR "tpp-str-substr-macro"
```
</details>

## TPP_HAVE_MACRO___TPP_STR_SIZE

Support for the builtin macro `__TPP_STR_SIZE()`
Stand-alone macro that takes 1 arguments and (assuming [`TPP_HAVE_BUILTIN_EXPR_STRINGS`](config-conf.md#tpp_have_builtin_expr_strings)
and [`TPP_HAVE_MACRO___TPP_EVAL`](config-conf.md#tpp_have_macro___tpp_eval) are enabled) can be implemented as follows:

```c
#define __TPP_STR_SIZE(str) __TPP_EVAL(#(str))
```

@detect: #ifdef __TPP_STR_SIZE

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TPP_STR_SIZE "tpp-str-size-macro"
```
</details>

## TPP_HAVE_MACRO___TPP_COUNT_TOKENS

Support for the builtin macro `__TPP_COUNT_TOKENS()`
A token-feature probing macro that lets you count the # of
tokens that are contained within a given string. For this
purpose, no macro expansion or directive processing is done:

```
__TPP_COUNT_TOKENS("++")  // 1 if "TPP_HAVE_TPP_TOK_PLUS_PLUS" is enabled, else 2
#define FOO 10 20 30
__TPP_COUNT_TOKENS("FOO")        // 1 because macros aren't expanded
__TPP_COUNT_TOKENS("#undef FOO") // 3 (or 4 if TPP_HAVE_TPP_TOK_SPACE) because directives aren't parsed
```

Based on the numbers returned by this macro, it becomes possible
to detect the state of pretty much all configuration options that
affect the behavior of `tpp_lexer_yieldraw()`

@detect: #ifdef __TPP_COUNT_TOKENS

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TPP_COUNT_TOKENS "tpp-count-tokens-macro"
```
</details>

## TPP_HAVE_MACRO___TPP_IDENTIFIER

Support for the builtin macro `__TPP_IDENTIFIER()`
This builtin macro can be used to construct arbitrary keyword-like
tokens, including ones that aren't actually keywords. This macro
takes a single argument (in the form of a string), and always expands
to a single keyword/identifier-type token:

```c
__TPP_IDENTIFIER("foo")   // Same "foo"
__TPP_IDENTIFIER("1foo")  // Still a keyword; namely: "1foo" -- there's no way
                          // to write this, since it'd normally be a TPP_TOK_INT
__TPP_IDENTIFIER("a\0b")  // Compilers probably won't like this: NUL-character in
                          // keyword name, meaning strlen() returns "1", but keyword
                          // is legally distinct from "a" (though lots of places
                          // will probably print the keyword as "a")
```

@detect: #ifdef __TPP_IDENTIFIER

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_MACRO___TPP_IDENTIFIER "tpp-identifier-macro"
```
</details>

## TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS

Support for alternative parenthesis pairs in macros:

```c
#define normal_macro(x) you_should("know this", x)
#define array[index]    get_ident(ARRAY, index)
#define block{...}      do { __VA_ARGS__ } while (should_continue())
#define point<T>        struct { T x; T y; }
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*
@detect: #if __has_known_extension("-falternative-macro-parenthesis")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS ? TPP_CONF_EXT1 : 0
```

Extension:

```c
#define TPP_EXTNAME_ALTERNATIVE_MACRO_PARENTHESIS "alternative-macro-parenthesis"
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
@detect: #if __has_known_extension("-fmacro-argument-whitespace")

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT0 : 0
```

Extension:

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
REPEAT(42, HELLO_WORLD)
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*
@detect: #if __has_known_extension("-fmacro-recursion")

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? TPP_CONF_EXT0 : 0
```

Extension:

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

NOTE: affects behavior of macros at the *TIME OF DEFINITION*
@detect: #if __has_known_extension("-ftraditional-macro")

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? TPP_CONF_EXT0 : 0
```

Extension:

```c
#define TPP_EXTNAME_TRADITIONAL_MACROS "traditional-macro"
```
</details>

## TPP_HAVE_NAMED_VARARGS_IN_MACROS

Support for variable-argument macros with named varargs:

```c
#define printf(format, args...) args
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*
@detect: #if __has_known_extension("-fnamed-varargs-in-macros")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_NAMED_VARARGS_IN_MACROS "named-varargs-in-macros"
```
</details>

## TPP_HAVE_VA_ARGS_IN_MACROS

Support for variable-argument macros:

```c
#define printf(format, ...) __VA_ARGS__
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*
@detect: #if __has_known_extension("-fva-args-in-macros")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CPP_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_VA_ARGS_IN_MACROS "va-args-in-macros"
```
</details>

## TPP_HAVE_VA_COMMA_IN_MACROS

Support for special keyword `__VA_COMMA__` inside of variable-argument macros.
When varargs are non-empty, replaced with `,`. Otherwise, replaced with nothing:

```c
#define printf(format, ...) fprintf(stderr, format __VA_COMMA__ __VA_ARGS__)
printf("foo\n");         // fprintf(stderr, "foo\n");
printf("i = %d\n", 10);  // fprintf(stderr, "i = %d\n", 10);
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*
@detect: #define test1(a, b, ...) __VA_ARGS__+0
         #define test2(...) test1(__VA_COMMA__ 0, 1)
         #if test2(~)

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_VA_COMMA_IN_MACROS "va-comma-in-macros"
```
</details>

## TPP_HAVE_VA_OPT_IN_MACROS

Support for special function `__VA_OPT__()` inside of variable-argument macros.
When varargs are non-empty, the content within parenthesis is kept. Otherwise,
replaced said content is removed alongside the `__VA_OPT__()` call itself:

```c
#define printf(format, ...) fprintf(stderr, format __VA_OPT__(,) __VA_ARGS__)
printf("foo\n");         // fprintf(stderr, "foo\n");
printf("i = %d\n", 10);  // fprintf(stderr, "i = %d\n", 10);
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*
@detect: #define test1(a, b, ...) __VA_ARGS__+0
         #define test2(...) test1(__VA_OPT__(,) 0, 1)
         #if test2(~)

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_VA_OPT_IN_MACROS "va-opt-in-macros"
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
@detect: #define test___VA_NARGS__ 0
         #define test_1            1
         #define test(...) test_##__VA_NARGS__
         #if test2(~)

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_VA_NARGS_IN_MACROS "va-nargs-in-macros"
```
</details>

## TPP_HAVE_VA_GLUE_COMMA_IN_MACROS

Support for GCC-style va-comma: when a variable-argument macro
contains a construct like `, ## __VA_ARGS__` (or the name of the
variable-length argument when [`TPP_HAVE_NAMED_VARARGS_IN_MACROS`](config-conf.md#tpp_have_named_varargs_in_macros)
is enabled), then the `,` is deleted during expansion whenever
the there are no variable arguments:

```c
#define printf(format, ...) fprintf(stderr, format, ##__VA_ARGS__)
printf("foo\n");         // fprintf(stderr, "foo\n");
printf("i = %d\n", 10);  // fprintf(stderr, "i = %d\n", 10);
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*
@detect: #define test1(a, b, ...) __VA_ARGS__+0
         #define test2(...) test1(,##__VA_ARGS__, 1)
         #if test2() == 0

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_VA_GLUE_COMMA_IN_MACROS "glue-comma-in-macros"
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
compilation. When [`TPP_HAVE_TRADITIONAL_MACROS`](config-conf.md#tpp_have_traditional_macros) is enabled,
then stringization works differently and must be done like so:

```c
#define str(x) "x"
str(foo)    // Expands to: "foo"
str("foo")  // Expands to: ""foo"" -- oops; traditional macros can't do this
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*
@detect: #define str(x) #x
         #if __TPP_COUNT_TOKENS(str(a b)) == 1

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_STRINGIZE_MACRO_ARGUMENT "stringize-macro-argument"
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
compilation. When [`TPP_HAVE_TRADITIONAL_MACROS`](config-conf.md#tpp_have_traditional_macros) is enabled,
then charization works differently and must be done like so:

```c
#define chr(x) 'x'
chr(foo)    // Expands to: 'foo'
chr('foo')  // Expands to: ''foo'' -- oops; traditional macros can't do this
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*
@detect: #define str(x) #@x
         #if __TPP_COUNT_TOKENS(str(a b)) == 1

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_CHARIZE_MACRO_ARGUMENT "charize-macro-argument"
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
@detect: #define test1(x) #x
         #define test2(x) test1(#!x)
         #define test3    42
         #if __TPP_STR_SIZE(__TPP_COUNT_TOKENS(test3)) == 5

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_DONT_EXPAND_MACRO_ARGUMENT "dont-expand-macro-argument"
```
</details>

## TPP_HAVE_GLUE_MACRO_ARGUMENT

Support for `##` in macros to concatenate different tokens
in order to form a new token:

```c
#define cat(a, b) a##b
cat(+, +)  // Expands to a single token "++" (assuming that TPP_HAVE_TPP_TOK_PLUS_PLUS is enabled)
```

NOTE: affects behavior of macros at the *TIME OF DEFINITION*
@detect: #define test(a, b) a##b
         #define str2(x) #x
         #define str(x) str2(x)
         #if __TPP_COUNT_TOKENS(str(test(10, 20))) == 1

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_GLUE_MACRO_ARGUMENT "glue-macro-argument"
```
</details>

## TPP_HAVE_PRAGMA_PUSH_MACRO

Support for: #pragma push_macro() / #pragma pop_macro()
@detect: #define TEST 42
         #pragma push_macro("TEST")
         #undef TEST
         #pragma pop_macro("TEST")
         #ifdef TEST

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_PUSH_MACRO "pragma-push-macro"
```
</details>

## TPP_HAVE_PRAGMA_ONCE

Support for: #pragma once
@detect: #if __has_known_extension("-fpragma-once")

<details><summary>Details</summary>

Default:

```c
((TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT) && TPP_HAVE_PRAGMA) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_ONCE "pragma-once"
```
</details>

## TPP_HAVE_PRAGMA_DEPRECATED

Support for: #pragma deprecated("foo")
@detect: #if __has_known_extension("-fpragma-deprecated")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_DEPRECATED "pragma-deprecated"
```
</details>

## TPP_HAVE_PRAGMA_EXTENSION

Support for: #pragma extension(...)
@detect: #if __has_known_extension("-fpragma-extension")

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_PRAGMA && TPP_HAVE_EXTENSIONS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_EXTENSION "pragma-extension"
```
</details>

## TPP_HAVE_PRAGMA_WARNING

Support for: #pragma warning(...)
@detect: #if __has_known_extension("-fpragma-warning")

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_WARNING "pragma-warning"
```
</details>

## TPP_HAVE_PRAGMA_MESSAGE

Support for: #pragma message("...")
@detect: #if __has_known_extension("-fpragma-message")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_MESSAGE "pragma-message"
```
</details>

## TPP_HAVE_PRAGMA_ERROR

Support for: #pragma error("...")
@detect: #if __has_known_extension("-fpragma-error")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_ERROR "pragma-error"
```
</details>

## TPP_HAVE_PRAGMA_REGION

Support for: #pragma region,  #pragma endregion
@detect: #if __has_known_extension("-fpragma-region")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_REGION "pragma-region"
```
</details>

## TPP_HAVE_PRAGMA_TPP_EXEC

Support for: #pragma tpp_exec("...")
@detect: #if __has_known_extension("-fpragma-tpp-exec")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_TPP_EXEC "pragma-tpp-exec"
```
</details>

## TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS

Support for: #pragma tpp_set_keyword_flags("foo", 0x7f)
@detect: #if __has_known_extension("-fpragma-tpp-set-keyword-flags")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_TPP_SET_KEYWORD_FLAGS "pragma-tpp-set-keyword-flags"
```
</details>

## TPP_HAVE_PRAGMA_GCC_POISON

Support for: #pragma GCC poison foo
@detect: #if __has_known_extension("-fpragma-gcc-poison")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_GCC_POISON "pragma-gcc-poison"
```
</details>

## TPP_HAVE_PRAGMA_GCC_WARNING

Support for: #pragma GCC warning "message"
@detect: #if __has_known_extension("-fpragma-gcc-warning")

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_GCC_WARNING "pragma-gcc-warning"
```
</details>

## TPP_HAVE_PRAGMA_GCC_ERROR

Support for: #pragma GCC error "message"
@detect: #if __has_known_extension("-fpragma-gcc-error")

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_GCC_ERROR "pragma-gcc-error"
```
</details>

## TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER

Support for: #pragma GCC system_header
@detect: #if __has_known_extension("-fpragma-gcc-system-header")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_GCC_SYSTEM_HEADER "pragma-gcc-system-header"
```
</details>

## TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC

Support for: #pragma GCC diagnostic
@detect: #if __has_known_extension("-fpragma-gcc-diagnostic")

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

```c
#define TPP_EXTNAME_PRAGMA_GCC_DIAGNOSTIC "pragma-gcc-diagnostic"
```
</details>

## TPP_HAVE_PRAGMA_GCC_DEPENDENCY

Support for: #pragma GCC dependency
@detect: #if __has_known_extension("-fpragma-gcc-dependency")

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)) : 0
```

Extension:

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
| `??!`    | `|`         |
| `??>`    | `}`         |
| `??-`    | `~`         |
| `???`    | `?`         |

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)
```

Extension:

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
| `%:%:`  | `##` (requires [`TPP_HAVE_TPP_TOK_POUND_POUND`](config-token.md#tpp_have_tpp_tok_pound_pound)) |
| `<::`   | `<`, `::` (requires [`TPP_HAVE_TPP_TOK_COLON_COLON`](config-token.md#tpp_have_tpp_tok_colon_colon)) |

@detect: #if __TPP_COUNT_TOKENS("%:") == 1

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)
```

Extension:

```c
#define TPP_EXTNAME_DIGRAPHS "digraphs"
```
</details>

## TPP_HAVE_TPP_TOK_LF

Configures if line-feed tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()`
@detect: #if __TPP_COUNT_TOKENS("\n") != 0

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_SPACE
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_LF "tok-lf"
```
</details>

## TPP_HAVE_TPP_TOK_SPACE

Configures if whitespace tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()`
@detect: #if __TPP_COUNT_TOKENS(" ") != 0

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_SPACE
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_SPACE "tok-space"
```
</details>

## TPP_HAVE_TPP_TOK_COMMENT

Configures if comment tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()`
@detect: #if __TPP_COUNT_TOKENS("// a b c") == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_SPACE
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_COMMENT "tok-comment"
```
</details>

## TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT

Enable support for recognizing c++-like comments: `// like this one!`
@detect: #if __TPP_COUNT_TOKENS("// a b c") <= 1

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_DEFAULT || (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)) ? 1 : TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_SLASH_SLASH_COMMENT "tok-slash-slash-comment"
```
</details>

## TPP_HAVE_TPP_TOK_SLASH_STAR_COMMENT_STAR_SLASH

Enable support for recognizing c-like comments: `/* like this one! */`
@detect: #if __TPP_COUNT_TOKENS("/* a b c */") <= 1

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_DEFAULT || (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)) ? 1 : TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_SLASH_STAR_COMMENT_STAR_SLASH "tok-slash-star-comment-star-slash"
```
</details>

## TPP_HAVE_TPP_TOK_LPAREN_STAR_COMMENT_STAR_RPAREN

Enable support for recognizing pascal-like comments: `(* like this one! *)`
@detect: #if __TPP_COUNT_TOKENS("(* a b c *)") <= 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_LPAREN_STAR_COMMENT_STAR_RPAREN "tok-lparen-star-comment-star-rparen"
```
</details>

## TPP_HAVE_TPP_TOK_LANGLE_EXCLAIM_MINUS_MINUS_COMMENT_MINUS_MINUS_RANGLE

Enable support for recognizing html-like comments: `<!-- like this one -->`
@detect: #if __TPP_COUNT_TOKENS("<!-- a b c -->") <= 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_LANGLE_EXCLAIM_MINUS_MINUS_COMMENT_MINUS_MINUS_RANGLE "tok-langle-exclaim-minus-minus-comment-minus-minus-rangle"
```
</details>

## TPP_HAVE_TPP_TOK_POUND_COMMENT

Enable support for recognizing shell-like comments: `# like this one!`

This still works in conjunction with "TPP_HAVE_CPP_DIRECTIVES", in
that unknown directives will simply be re-emit as shell comments,
and shell comments that don't appear at the start of lines are not
even processed as CPP directives.
@detect: #if __TPP_COUNT_TOKENS("# a b c") <= 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_POUND_COMMENT "tok-pound-comment"
```
</details>

## TPP_HAVE_TPP_TOK_SLASH_COMMENT

Enable support for recognizing ASM-like comments: `/ like this one!`
@detect: #if __TPP_COUNT_TOKENS("/ a b c") <= 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_SLASH_COMMENT "tok-slash-comment"
```
</details>

## TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT

Enable support for recognizing SQL-like comments: `-- like this one!`
@detect: #if __TPP_COUNT_TOKENS("-- a b c") <= 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_MINUS_MINUS_COMMENT "tok-minus-minus-comment"
```
</details>

## TPP_HAVE_TPP_TOK_AT_AT_COMMENT

Enable support for recognizing deemon-doc-like comments: `@@ like this one!`
@detect: #if __TPP_COUNT_TOKENS("@@ a b c") <= 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_COMMENT
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_AT_AT_COMMENT "tok-at-at-comment"
```
</details>

## TPP_HAVE_TPP_TOK_DOLLAR

"$" (treat as its own token, rather than as part of identifiers/keywords)
@detect: #if __TPP_COUNT_TOKENS("a$b") == 3

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_1CHAR
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_DOLLAR "tok-dollar"
```
</details>

## TPP_HAVE_TPP_TOK_INT

123
@detect: #if __TPP_COUNT_TOKENS("123") == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_GENERIC
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_INT "tok-int"
```
</details>

## TPP_HAVE_TPP_TOK_FLOAT

123.0
@detect: #if __TPP_COUNT_TOKENS("123.0") == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_GENERIC
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_FLOAT "tok-float"
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

@detect: #if __TPP_COUNT_TOKENS("0x1P+12") == 1 && __TPP_COUNT_TOKENS("0xE+12") == 3

<details><summary>Details</summary>

Default:

```c
((TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) && TPP_HAVE_TPP_TOK_FLOAT) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : TPP_CONF_FEAT1) : 0
```

Extension:

```c
#define TPP_EXTNAME_SMART_FLOAT_TOKENS "smart-float-tokens"
```
</details>

## TPP_HAVE_TPP_TOK_C_CHAR

Support for C character literals: `'foo'`
@detect: #if __TPP_COUNT_TOKENS("'foo'") == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_GENERIC
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_C_CHAR "tok-char"
```
</details>

## TPP_HAVE_TPP_TOK_C_STRING

Support for C string literals: `"foo"`
@detect: #if __TPP_COUNT_TOKENS('"foo"') == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_GENERIC
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_C_STRING "tok-string"
```
</details>

## TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL

Support for string literals: `R"AB(foo)AB"`
@detect: #if __TPP_COUNT_TOKENS('R"AB(foo)AB"') == 1 && __TPP_STR_SIZE(R"AB(foo)AB") == 3

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_CXX_RAW_STRING_LITERAL "tok-cxx-raw-string-literal"
```
</details>

## TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL

Support for string literals: `L"foo"`
When [`TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_raw_string_literal) is also enabled, also support `LR"AB(foo)AB")`
@detect: #if __TPP_COUNT_TOKENS('L"foo"') == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_CXX_WIDE_STRING_LITERAL "tok-cxx-wide-string-literal"
```
</details>

## TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL

Support for string literals: `u8"foo"`
When [`TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_raw_string_literal) is also enabled, also support `u8R"AB(foo)AB"`
@detect: #if __TPP_COUNT_TOKENS('u8"foo"') == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_CXX_UTF8_STRING_LITERAL "tok-cxx-utf8-string-literal"
```
</details>

## TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL

Support for string literals: `u"foo"`
When [`TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_raw_string_literal) is also enabled, also support `uR"AB(foo)AB"`
@detect: #if __TPP_COUNT_TOKENS('u"foo"') == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_CXX_UTF16_STRING_LITERAL "tok-cxx-utf16-string-literal"
```
</details>

## TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL

Support for string literals: `U"foo"`
When [`TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_raw_string_literal) is also enabled, also support `UR"AB(foo)AB"`
@detect: #if __TPP_COUNT_TOKENS('U"foo"') == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_CXX_UTF32_STRING_LITERAL "tok-cxx-utf32-string-literal"
```
</details>

## TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL

Support for string literals: `R'AB(f)AB'`
@detect: #if __TPP_COUNT_TOKENS("R'AB(foo)AB'") == 1 && __TPP_STR_SIZE(R'AB(foo)AB') == 3

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_CXX_RAW_CHAR_LITERAL "tok-cxx-raw-char-literal"
```
</details>

## TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL

Support for string literals: `L'f'`
When [`TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_raw_char_literal) is also enabled, also support `LR'AB(f)AB'`
@detect: #if __TPP_COUNT_TOKENS("L'f'") == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_CXX_WIDE_CHAR_LITERAL "tok-cxx-wide-char-literal"
```
</details>

## TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL

Support for string literals: `u8'f'`
When [`TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_raw_char_literal) is also enabled, also support `u8R'AB(f)AB'`
@detect: #if __TPP_COUNT_TOKENS("u8'f'") == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_CXX_UTF8_CHAR_LITERAL "tok-cxx-utf8-char-literal"
```
</details>

## TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL

Support for string literals: `u'f'`
When [`TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_raw_char_literal) is also enabled, also support `uR'AB(f)AB'`
@detect: #if __TPP_COUNT_TOKENS("u'f'") == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_CXX_UTF16_CHAR_LITERAL "tok-cxx-utf16-char-literal"
```
</details>

## TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL

Support for string literals: `U'f'`
When [`TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_raw_char_literal) is also enabled, also support `UR'AB(f)AB'`
@detect: #if __TPP_COUNT_TOKENS("U'f'") == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_C_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_CXX_UTF32_CHAR_LITERAL "tok-cxx-utf32-char-literal"
```
</details>

## TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL

Support for string literals: `R"foo"` and `r"foo"`
@detect: #if __TPP_COUNT_TOKENS('R"foo"') == 1 && __TPP_STR_SIZE(R"AB(foo)AB") == 9

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_RAW_STRING_LITERAL "tok-raw-string-literal"
```
</details>

## TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL

Support for string literals: `R'bar'` and `r'bar'`
@detect: #if __TPP_COUNT_TOKENS('R"foo"') == 1 && __TPP_STR_SIZE(R'AB(foo)AB') == 9

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_RAW_CHAR_LITERAL "tok-raw-char-literal"
```
</details>

## TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL

Support for java-style block string literals: `"""foo"""`
@detect: #if __TPP_COUNT_TOKENS('"""\n a\n b"""') == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_BLOCK_STRING_LITERAL "tok-block-string-literal"
```
</details>

## TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL

Support for java-style block string literals (but with single-ticks): `'''foo'''`
@detect: #if __TPP_COUNT_TOKENS("'''\n a\n b'''") == 1

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
```

Extension:

```c
#define TPP_EXTNAME_TPP_TOK_BLOCK_CHAR_LITERAL "tok-block-char-literal"
```
</details>

## TPP_HAVE_STRING_ALLOW_MULTILINE

Feature-flag: treat line-feeds like any regular character in string tokens:

- [`TPP_HAVE_TPP_TOK_C_STRING`](config-conf.md#tpp_have_tpp_tok_c_string)
- [`TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_wide_string_literal)
- [`TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_utf8_string_literal)
- [`TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_utf16_string_literal)
- [`TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_utf32_string_literal)
- [`TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL`](config-conf.md#tpp_have_tpp_tok_raw_string_literal)
- [`TPP_HAVE_TPP_TOK_C_CHAR`](config-conf.md#tpp_have_tpp_tok_c_char)
- [`TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_wide_char_literal)
- [`TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_utf8_char_literal)
- [`TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_utf16_char_literal)
- [`TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL`](config-conf.md#tpp_have_tpp_tok_cxx_utf32_char_literal)
- [`TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL`](config-conf.md#tpp_have_tpp_tok_raw_char_literal)

When this flag is disabled, line-feeds in such string tokens will instead
terminate the string, and cause a `TPP_W_STRING_TERMINATED_BY_LINEFEED`
warning to be emitted.

@detect: #if __TPP_COUNT_TOKENS("\"\n\"") == 1

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) ? TPP_CONF_FEAT0 : 0
```

Extension:

```c
#define TPP_EXTNAME_STRING_ALLOW_MULTILINE "string-allow-multiline"
```
</details>

## TPP_HAVE_STRING_AUTO_CONCAT

Enable support for automatic concatenation of adjacent string tokens.
This affects the behavior of `tpp_lexer_parsestring_ex()` and its
companion `tpp_lexer_parsestring_cb()`, such that they will only yield
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

Extension:

```c
#define TPP_EXTNAME_STRING_AUTO_CONCAT "string-auto-concat"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_DEFINED

Enable support for `defined(MACRO)` in builtin lexer expressions
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_DEFINED "defined-in-expressions"
```
</details>

## TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR

Enable special handling in `#define foo(x) defined(x)` such that `x` is not expanded
@detect: #define test(x) defined(x)
         #define test2   test1
         #if test(test2)

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_EXPR_DEFINED && TPP_HAVE_CPP_MACROS && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? TPP_CONF_EXT0 : 0
```

Extension:

```c
#define TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR "dont-expand-defined"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_STRINGS

Enable support for string operations in builtin lexer expressions
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_TPP_TOK_C_STRINGLIKE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_STRINGS "strings-in-expressions"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_FLOATS

Enable support for floats in builtin lexer expressions
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_TPP_TOK_FLOAT && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_FLOATS "floats-in-expressions"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT

Enable support for `foo ?: bar` in builtin lexer expressions (same as `foo ? foo : bar`)
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT "if-else-optional-true"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS

Enable support for `if (foo) bar else baz` in builtin lexer expressions
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS "ifelse-in-expressions"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR

Enable support for `^^` in builtin lexer expressions
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_LOGICAL_XOR "logical-xor-in-expressions"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS

Enable support for `0b` literals in builtin lexer expressions
@detect: N/A

<details><summary>Details</summary>

Default:

```c
((TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) && TPP_HAVE_TPP_TOK_INT) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_BINARY_LITERALS "binary-literals"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS

Enable support for `0o` literals in builtin lexer expressions
@detect: N/A

<details><summary>Details</summary>

Default:

```c
((TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) && TPP_HAVE_TPP_TOK_INT) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0
```

Extension:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_OCTAL_LITERALS "octal-literals"
```
</details>

## TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX

Enable support for `u`, `l`, `ul`, `ll`, `ull` integer suffixes
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_TPP_TOK_INT && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)) : 0
```

Extension:

```c
#define TPP_EXTNAME_LEXER_DECODEINT_FIXED_TYPE_SUFFIX "fixed-type-integrals"
```
</details>

## TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX

Enable support for `z`, `uz` integer suffixes
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_TPP_TOK_INT && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)) : 0
```

Extension:

```c
#define TPP_EXTNAME_LEXER_DECODEINT_SIZE_TYPE_SUFFIX "size-type-integrals"
```
</details>

## TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX

Enable support for `i8`, `i16`, `i32`, `i64`, `ui8`, `ui16`, `ui32`, `ui64` integer suffixes
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_TPP_TOK_INT && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)) : 0
```

Extension:

```c
#define TPP_EXTNAME_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX "fixed-length-integrals"
```
</details>

## TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX

Enable support for `f`, `F`, `l`, `L` float suffixes
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_TPP_TOK_FLOAT && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)) : 0
```

Extension:

```c
#define TPP_EXTNAME_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX "fixed-type-float"
```
</details>

## TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX

Enable support for `d`, `D` float suffixes
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_TPP_TOK_FLOAT && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 0) : 0
```

Extension:

```c
#define TPP_EXTNAME_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX "double-type-float"
```
</details>

## TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX

Enable support for `df`, `DF`, `dd`, `DD`, `dl`, `DL` float suffixes
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_TPP_TOK_FLOAT && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)) : 0
```

Extension:

```c
#define TPP_EXTNAME_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX "decimal-type-float"
```
</details>

## TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS

Treat `'a'` in expressions as an integer, rather than as a string (in C, this is always the case)
@detect: N/A

<details><summary>Details</summary>

Default:

```c
(TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_TPP_TOK_C_STRINGLIKE_SQUOTE && (TPP_PROFILE != TPP_PROFILE_MINIMAL)) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_FEAT1 : 1) : 0
```

Extension:

```c
#define TPP_EXTNAME_BUILTIN_EXPR_CHARACTER_LITERALS "character-literals"
```
</details>

## TPP_HAVE_EXTERN_C_FOR_SYSHDR

When `TPP_FILE_FLAGS_SYSHDR` is set during `#include` (i.e. *NOT* via `#pragma GCC system_header`),
then the `TPP_FILE_FLAGS_EXTERN_C` flag should be set alongside `TPP_FILE_FLAGS_SYSHDR`.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_FILE_SYSHDR && TPP_HAVE_FILE_EXTERN_C ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT0 : TPP_CONF_FEAT0) : 0
```

Extension:

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

Extension:

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

Extension:

```c
#define TPP_EXTNAME_INCLUDE_RELATIVE_TO_EVERY_FILE "include-relative-to-every-file"
```
</details>

## TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION

Extra configuration for `#pragma message`: print a leading
[`TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT`](config-value.md#tpp_config_warning_file_and_line_format) using the values
that would also be printed by `__FILE__`, `__LINE__`, `__COLUMN__`

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE != TPP_PROFILE_MINIMAL) ? TPP_CONF_EXT0 : 0
```

Extension:

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

Extension:

```c
#define TPP_EXTNAME_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED "pragma-message-omits-trailing-linefeed"
```
</details>

<!--END:conf-->
