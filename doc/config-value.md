# Value Configurations

These configurations all define something other than an enabled/disabled state.

<!--BEGIN:value-->
## TPP_CONFIG_INCLUDE_REMAP_FILENAME

Name of the magic file searched-for by [`TPP_HAVE_INCLUDE_REMAP`](config-conf.md#tpp_have_include_remap). Unless you're
doing something *really* custom, you probably shouldn't change this since this
is already a rather niche function of GCC, and changing the filename probably
won't earn you any browny points.

<details><summary>Details</summary>

Default:

```c
"header.gcc"
```
</details>

## TPP_CONFIG_FILE_AND_LINE_FORMAT

Format to use for file+line+column log messages.

When [`TPP_HAVE_RT_FILE_AND_LINE_FORMAT`](config-core.md#tpp_have_rt_file_and_line_format) is enabled, this is only
the *default*-format, with the actual format being overwritable
at runtime.

For documentation on available format-codes, see `tpp_lexer_printf_warning()`

<details><summary>Details</summary>

Default:

```c
defined(_MSC_VER) ? "%Pf%?P{(%Pl, %Pc)%}: " : "%Pf:%?P{%Pl:%Pc:%} "
```
</details>

## TPP_CONFIG_TIME_ENVIRON

Name(s) of environment variables scanned for an override of the current time,
to use in place of the ~actual~ current time when expanding `__TIME__` and
related macros.

The first non-empty environment variable with a name listed here will be used
as an override of the current time. The variable's value must be a decimal
integer describing the *unix epoch second* (that is: the number of seconds
since `1970-01-01 00:00:00`). If that is not the case, no warning will be
emitted, and the effective value used by the lexer as the current time will
be weak undefined.

<details><summary>Details</summary>

Default:

```c
1("SOURCE_DATE_EPOCH")
```
</details>

## TPP_CONFIG_SYSTEM_INCLUDE_PATH

A preprocessor tuple describing the built-in, hard-coded, system-include path.
- The paths specified here are searched in order of specification.
- For information on the full `#include`-path resolution order, see [`tpp_include_paths`](../src/tpp-amalgamation.h#L23354)
- Try not to include trailing slashes in paths hard-coded using this (if TPP3 needs
  trailing slashes in these strings, it will add those itself)

Example:

```c
#define TPP_CONFIG_SYSTEM_INCLUDE_PATH  2("/usr/local/include", "/usr/include")
```

<details><summary>Details</summary>

Default:

```c
0()
```
</details>

## TPP_CONFIG_CLI_FILENAME

Filename of definitions file used by [`tpp_lexer_define()`](../src/tpp-amalgamation.h#L27415)

<details><summary>Details</summary>

Default:

```c
"<commandline>"
```
</details>

## TPP_CONFIG_VALUEOF_STDC_EMBED_NOT_FOUND

String representations of what `__has_embed()`
should expand to when the file wasn't found

Also represents the expansion of the predefined macro `__STDC_EMBED_NOT_FOUND__`

<details><summary>Details</summary>

Default:

```c
"0"
```

Detect:

```c
#ifdef __STDC_EMBED_NOT_FOUND__
...
#endif
```
</details>

## TPP_CONFIG_VALUEOF_STDC_EMBED_FOUND

String representations of what `__has_embed()` should
expand to when the file was found and is non-empty

Also represents the expansion of the predefined macro `__STDC_EMBED_FOUND__`

<details><summary>Details</summary>

Default:

```c
"1"
```

Detect:

```c
#ifdef __STDC_EMBED_FOUND__
...
#endif
```
</details>

## TPP_CONFIG_VALUEOF_STDC_EMBED_EMPTY

String representations of what `__has_embed()` should
expand to when the file was found, but is empty

Also represents the expansion of the predefined macro `__STDC_EMBED_EMPTY__`

<details><summary>Details</summary>

Default:

```c
"2"
```

Detect:

```c
#ifdef __STDC_EMBED_EMPTY__
...
#endif
```
</details>

## TPP_CONFIG_CPP_CONSTANT_SUFFIX

Suffix added to version numbers in `__cpp_*` predefined macros/keyword-features.

This config only takes effect when [`TPP_HAVE_CPP_FEATURE_MACROS`](config-conf.md#tpp_have_cpp_feature_macros) is enabled.

e.g.: When [`TPP_HAVE_CPP_EMBED`](config-conf.md#tpp_have_cpp_embed) is enabled, `__cpp_pp_embed` is defined
      to expand to something like `202502`, followed by a suffix defined
      by this config.

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_C || TPP_PROFILE == TPP_PROFILE_CXX) ? "L" : ""
```
</details>

## TPP_CONFIG_CLI_DEFAULT_SYSROOT

The default value for `-isysroot path` (see [`TPP_HAVE_CLI_DASH_ISYSROOT`](config-cli.md#tpp_have_cli_dash_isysroot)) in [`tpp_cli_loader`](../src/tpp-amalgamation.h#L28885)

<details><summary>Details</summary>

Default:

```c
(char const *)NULL
```
</details>

<!--END:value-->
