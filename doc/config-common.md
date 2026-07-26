# Common configuration groups

Similar to `TPP_PROFILE`, common configuration groups don't configure individual features, but rather specify the defaults of many features at once

<!--BEGIN:common-->
## TPP_COMMON_HAVE_EXTENSIONS

Common configuration describing if extensions configuration should be used in defaults.
When disabled, default configurations use...
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L5302) instead of [`TPP_CONF_EXT0`](../src/tpp-amalgamation.h#L5300)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L5301) instead of [`TPP_CONF_EXT1`](../src/tpp-amalgamation.h#L5299)

s.a.: [`TPP_COMMON_HAVE_FEATURES`](#tpp_common_have_features)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_EXTENSIONS
```
</details>

## TPP_COMMON_HAVE_FEATURES

Common configuration describing if feature configuration should be used in defaults.
When disabled, default configurations use...
- ... [`TPP_CONF_EXT0`](../src/tpp-amalgamation.h#L5300) instead of [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L5302)
- ... [`TPP_CONF_EXT1`](../src/tpp-amalgamation.h#L5299) instead of [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L5301)

s.a.: [`TPP_COMMON_HAVE_EXTENSIONS`](#tpp_common_have_extensions)

<details><summary>Details</summary>

Default:

```c
1
```
</details>

## TPP_COMMON_HAVE_TPP_TOK

Default configuration for tokens

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_COMMON_HAVE_TPP_TOK_SPACE

Default configuration for [`TPP_HAVE_TOK_LF`](config-conf.md#tpp_have_tok_lf), [`TPP_HAVE_TOK_SPACE`](config-conf.md#tpp_have_tok_space),
[`TPP_HAVE_TOK_COMMENT`](config-conf.md#tpp_have_tok_comment), indicating if tokens that are normally through
of as no-ops should be emitted by [`tpp_lexer_yieldpp()`](../src/tpp-amalgamation.h#L21037)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : ((TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_FEAT0 : 0)
```
</details>

## TPP_COMMON_HAVE_TPP_TOK_COMMENT

Default configuration describing if otherwise unconfigured comment tokens should be supported

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT0 : 0
```
</details>

## TPP_COMMON_HAVE_TPP_TOK_GENERIC

Default configuration for:

- [`TPP_HAVE_TOK_C_INT`](config-conf.md#tpp_have_tok_c_int)
- [`TPP_HAVE_TOK_C_FLOAT`](config-conf.md#tpp_have_tok_c_float)
- [`TPP_HAVE_TOK_C_CHAR`](config-conf.md#tpp_have_tok_c_char)
- [`TPP_HAVE_TOK_C_STRING`](config-conf.md#tpp_have_tok_c_string)

<details><summary>Details</summary>

Default:

```c
TPP_COMMON_HAVE_TPP_TOK
```
</details>

## TPP_COMMON_HAVE_TPP_TOK_C_STRING

Default configuration for extended C/C++ string tokens:

- [`TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL`](config-conf.md#tpp_have_tok_cxx_raw_string_literal)
- [`TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL`](config-conf.md#tpp_have_tok_cxx_wide_string_literal)
- [`TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL`](config-conf.md#tpp_have_tok_cxx_utf8_string_literal)
- [`TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL`](config-conf.md#tpp_have_tok_cxx_utf16_string_literal)
- [`TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL`](config-conf.md#tpp_have_tok_cxx_utf32_string_literal)
- [`TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL`](config-conf.md#tpp_have_tok_cxx_raw_char_literal)
- [`TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL`](config-conf.md#tpp_have_tok_cxx_wide_char_literal)
- [`TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL`](config-conf.md#tpp_have_tok_cxx_utf8_char_literal)
- [`TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL`](config-conf.md#tpp_have_tok_cxx_utf16_char_literal)
- [`TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL`](config-conf.md#tpp_have_tok_cxx_utf32_char_literal)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_FEAT0 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)
```
</details>

## TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING

Default configuration for deemon-specific string tokens:

- [`TPP_HAVE_TOK_RAW_STRING_LITERAL`](config-conf.md#tpp_have_tok_raw_string_literal)
- [`TPP_HAVE_TOK_RAW_CHAR_LITERAL`](config-conf.md#tpp_have_tok_raw_char_literal)
- [`TPP_HAVE_TOK_BLOCK_STRING_LITERAL`](config-conf.md#tpp_have_tok_block_string_literal)
- [`TPP_HAVE_TOK_BLOCK_CHAR_LITERAL`](config-conf.md#tpp_have_tok_block_char_literal)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT0 : 0
```
</details>

## TPP_COMMON_HAVE_TPP_TOK_C_TOKENS

Default configuration for tokens needed to implement a C- or C++-compiler

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_COMMON_HAVE_TPP_TOK : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)
```
</details>

## TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS

Default configuration for tokens needed to implement a C++-compiler

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT0 : (TPP_PROFILE == TPP_PROFILE_CXX)
```
</details>

## TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS

Default configuration for tokens not covered by any
of the other `TPP_COMMON_HAVE_TPP_TOK_*` configs

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT0 : 0
```
</details>

## TPP_COMMON_HAVE_CPP_DIRECTIVES_STD

Default configuration for constructs defined by some C/C++ standard

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : ((TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) || (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX))
```
</details>

## TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT

Default configuration for constructs that should be considered extensions

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : ((TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) || (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX))
```
</details>

<!--END:common-->
