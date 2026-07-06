# Common configuration groups

Similar to `TPP_PROFILE`, common configuration groups don't configure individual features, but rather specify the defaults of many features at once

<!--BEGIN:common-->
### TPP_COMMON_HAVE_TPP_TOK

Default configuration for tokens

Default: `(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : (TPP_PROFILE != TPP_PROFILE_MINIMAL)`


### TPP_COMMON_HAVE_TPP_TOK_1CHAR

Default configuration for 1-char tokens

Default: `(TPP_PROFILE != TPP_PROFILE_MINIMAL) ? TPP_CONF_EXT0 : 0`


### TPP_COMMON_HAVE_TPP_TOK_SPACE

Default configuration for `TPP_HAVE_TPP_TOK_LF`, `TPP_HAVE_TPP_TOK_SPACE`,
`TPP_HAVE_TPP_TOK_COMMENT`, indicating if tokens that are normally through
of as no-ops should be emitted by `tpp_lexer_yieldpp()`

Default: `(TPP_PROFILE != TPP_PROFILE_MINIMAL) ? TPP_CONF_FEAT1 : 1`


### TPP_COMMON_HAVE_TPP_TOK_COMMENT

Default configuration describing if otherwise unconfigured comment tokens should be supported

Default: `(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_COMMON_HAVE_TPP_TOK : 0`


### TPP_COMMON_HAVE_TPP_TOK_GENERIC

Default configuration for:

- `TPP_HAVE_TPP_TOK_INT`
- `TPP_HAVE_TPP_TOK_FLOAT`
- `TPP_HAVE_TPP_TOK_CHAR`
- `TPP_HAVE_TPP_TOK_STRING`

Default: `TPP_COMMON_HAVE_TPP_TOK`


### TPP_COMMON_HAVE_TPP_TOK_C_STRING

Default configuration for extended C/C++ string tokens:

- `TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL`
- `TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL`
- `TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL`
- `TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL`
- `TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL`
- `TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL`
- `TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL`
- `TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL`
- `TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL`
- `TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL`

Default: `(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_CONF_FEAT0 : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)`


### TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING

Default configuration for deemon-specific string tokens:

- `TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL`
- `TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL`
- `TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL`
- `TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL`

Default: `(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT0 : 0`


### TPP_COMMON_HAVE_TPP_TOK_C_TOKENS

Default configuration for tokens needed to implement a C- or C++-compiler

Default: `(TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) ? TPP_COMMON_HAVE_TPP_TOK : (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX)`


### TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS

Default configuration for tokens needed to implement a C++-compiler

Default: `(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT0 : (TPP_PROFILE == TPP_PROFILE_CXX)`


### TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS

Default configuration for tokens not covered by any
of the other `TPP_COMMON_HAVE_TPP_TOK_*` configs

Default: `(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT0 : 0`


### TPP_COMMON_HAVE_CPP_DIRECTIVES_STD

Default configuration for constructs defined by some C/C++ standard

Default: `(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : ((TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) || (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX))`


### TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT

Default configuration for constructs that should be considered extensions

Default: `(TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : ((TPP_PROFILE == TPP_PROFILE_ALL || TPP_PROFILE == TPP_PROFILE_DEFAULT) || (TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX))`


<!--END:common-->
