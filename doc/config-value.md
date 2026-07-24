# Value Configurations

These configurations all define something other than an enabled/disabled state.

<!--BEGIN:value-->
## TPP_CONFIG_SYSTEM_INCLUDE_PATH

A preprocessor tuple describing the built-in, hard-coded, system-include path.
- The paths specified here are searched in order of specification.
- For information on the full `#include`-path resolution order, see [`tpp_include_paths`](../src/tpp-amalgamation.h#L18369)
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

Filename of definitions file used by [`tpp_lexer_define()`](../src/tpp-amalgamation.h#L20210)

<details><summary>Details</summary>

Default:

```c
"<commandline>"
```
</details>

## TPP_CONFIG_VALUEOF_STDC_EMBED_NOT_FOUND

String representations of what `__has_embed()`
should expand to when the file wasn't found

Also represents the expansion of the pre-defined macro `__STDC_EMBED_NOT_FOUND__`

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

Also represents the expansion of the pre-defined macro `__STDC_EMBED_FOUND__`

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

Also represents the expansion of the pre-defined macro `__STDC_EMBED_EMPTY__`

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

## TPP_CONFIG_FILE_AND_LINE_FORMAT

Format to use for file+line+column log messages.
When [`TPP_HAVE_RT_FILE_AND_LINE_FORMAT`](config-core.md#tpp_have_rt_file_and_line_format) is enabled, this is
only the *default*-format, with the actual format being overwritable
at runtime.

<details><summary>Details</summary>

Default:

```c
defined(_MSC_VER) ? "%Pf(%Pl, %Pc): " : "%Pf:%Pl:%Pc: "
```
</details>

<!--END:value-->
