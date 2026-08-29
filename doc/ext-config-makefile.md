# Makefile

The **MAKEFILE** is a [*Source extension*](./config.md#source_extensions) for TPP that allows you to generate `Makefile`-compatible dependency descriptions for the input that is being preprocessed (listing dependencies triggered as a result of `#include` (or similar) directives).

Input:

```c
#include <stdio.h>

int main() {
	printf("Hello World\n");
	return 0;
}
```

Possible outputs (examples):

```Makefile
input.o: input.c /usr/include/stdio.h
```

```Makefile
input.o: input.c /usr/include/stdio.h
input.c:
/usr/include/stdio.h:
```

---

# Configuration

The following configuration options are available and can be used to customize the behavior of makefile generation.

<!--BEGIN:all-->
## TPP_MAKEFILE_HAVE_USER_DEPENDENCIES

When enabled, only include dependencies if
`#include`-stack doesn't contain any system
headers

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6629), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6628), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6633): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6632): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_FILE_SYSHDR ? TPP_CONF_FEAT0 : 0
```
</details>

## TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES

Enable API support for handling [`TPP_HOOK_INCLUDE_NOT_FOUND`](config-hook.md#tpp_have_include_not_found_hook) by emitting
the associated file as an additional dependency. Useful when wanting to
(somewhat accurately) determine dependencies, where some of them might
not exist, yet.

Used to implement `-MG` (aka. `--print-missing-file-dependencies`).

When enabled, the following APIs become available:

- [`tpp_makefile_enable_missing_file_dependencies()`](../src/tpp-makefile-amalgamation.h#L863)
- [`tpp_makefile_disable_missing_file_dependencies()`](../src/tpp-makefile-amalgamation.h#L865)
- [`tpp_makefile_get_missing_file_dependencies_enabled()`](../src/tpp-makefile-amalgamation.h#L864)
- [`tpp_makefile_set_missing_file_dependencies_enabled()`](../src/tpp-makefile-amalgamation.h#L882)

Note that even when this is enabled, printing of missing file
dependencies is disabled by default (enable it by making a
call to [`tpp_makefile_enable_missing_file_dependencies()`](../src/tpp-makefile-amalgamation.h#L863)).

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK)
```
</details>

## TPP_MAKEFILE_HAVE_PHONY

When enabled, the Makefile keeps track of all dependencies encountered
over time, and (once flushed) will print all of them a second time in
the form of empty (dummy) targets. This is necessary in case one of those
dependencies is deleted, since otherwise Makefile will complain about
deleted files when reading in dependencies again.

Used to implement the `-MP` CLI switch

Configure as one of:

- [`TPP_CONF_0`](../src/tpp-amalgamation.h#L6629), `0`: Disabled
- [`TPP_CONF_1`](../src/tpp-amalgamation.h#L6628), `1`: Enabled
- [`TPP_CONF_FEAT0`](../src/tpp-amalgamation.h#L6633): Available (disabled by default)
- [`TPP_CONF_FEAT1`](../src/tpp-amalgamation.h#L6632): Available (enabled by default)

<details><summary>Details</summary>

Default:

```c
TPP_CONF_FEAT0
```
</details>

## TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH

(soft-)limit on how long lines in the dependency output should get.
Whenever a line (tries to) become longer than this, the next dependency
isn't appended to the same line, and instead a `\`-escaped linefeed is
inserted and the dependency is emitted to the next line.

- When negative, the limit becomes configurable at runtime, with the
  absolute value of this configuration being used as default.
- When positive, the limit is hard-coded as that value
- When `0`, the limit is hard-coded as infinite (wrapping never happens)

<details><summary>Details</summary>

Default:

```c
-72
```
</details>

## TPP_MAKEFILE_HAVE_IO_HANDLE

Provide an API surrounding [`tpp_makefile_io_handle`](../src/tpp-makefile-amalgamation.h#L426) that can be used to
open/create files for writing, and then write to those files. This in
turn is necessary to implement stuff like the `-MF` CLI switch.

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO

Provide an API [`tpp_makefile_setoutput_io()`](../src/tpp-makefile-amalgamation.h#L716) that can be used to override
the makefile's output printer with one that prints data into a linked I/O
handle of type [`tpp_makefile_io_handle`](../src/tpp-makefile-amalgamation.h#L426).

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_MAKEFILE_HAVE_IO_HANDLE
```
</details>

## TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE

Provide an API [`tpp_makefile_setoutput_io_ex()`](../src/tpp-makefile-amalgamation.h#L748) that can also be used to
set a flag specifying that the specified I/O handle should *not* be closed
when the makefile is finalized (or a different output is assigned)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO
```
</details>

## TPP_MAKEFILE_HAVE_OUTPUT_FILE

Provide an API [`tpp_makefile_setoutput_file()`](../src/tpp-makefile-amalgamation.h#L780) that is a concenience
wrapper around [`tpp_makefile_setoutput_io()`](../src/tpp-makefile-amalgamation.h#L716) by automatically opening
the file in question and assigning its output.

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) && TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO
```
</details>

## TPP_MAKEFILE_HAVE_CLI

Enable support for [`tpp_makefile_cli_loader`](../src/tpp-makefile-amalgamation.h#L941)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI
```
</details>

## TPP_MAKEFILE_HAVE_CLI_HELP

Enable support for `tpp_makefile_cli_loader_help`, which exposes a small
database of supported commandline flags in a human-readable format that
can also be rendered (fairly) easily.

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI && TPP_HAVE_CLI_HELP
```
</details>

## TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS

Include extra spellings (i.e.: in addition to the primary spelling) of CLI options.

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI_HELP && (TPP_PROFILE != TPP_PROFILE_MINIMAL)
```
</details>

## TPP_MAKEFILE_HAVE_CLI_DASH_M

`-M`, `--dependencies`: Turn on makefile generator, and
set the CLI loader's `ONLYMAKEFILE` flag such that it will
consume all of the lexer's tokens during its flush phase.

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI
```
</details>

## TPP_MAKEFILE_HAVE_CLI_DASH_MM

`-MM`, `--user-dependencies`: Like `-M` but also turn on [`TPP_MAKEFILE_HAVE_USER_DEPENDENCIES`](#tpp_makefile_have_user_dependencies)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI && TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
```
</details>

## TPP_MAKEFILE_HAVE_CLI_DASH_MF

`-MF file`: Redirect makefile output to `file` (but doesn't clear the `ONLYMAKEFILE`
flag if used with `-M` or `-MM` -- for that, you must use `-MD` or `-MMD` instead,
which also (optionally) accept this flag)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI && TPP_MAKEFILE_HAVE_OUTPUT_FILE
```
</details>

## TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH

Extension to [`TPP_MAKEFILE_HAVE_CLI_DASH_MF`](#tpp_makefile_have_cli_dash_mf): when the
specified filename is `-`, output to `stdout` instead

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI_DASH_MF && TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE
```
</details>

## TPP_MAKEFILE_HAVE_CLI_DASH_MG

`-MG`, `--print-missing-file-dependencies`:
Turn on printing of missing file dependencies within the linked makefile/lexer.
Also disables regular preprocessor output like [`TPP_MAKEFILE_HAVE_CLI_DASH_M`](#tpp_makefile_have_cli_dash_m).

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI && TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES
```
</details>

## TPP_MAKEFILE_HAVE_CLI_DASH_MT

`-MT TARGET`: Specifies the exact text that to use as the makefile target name.

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI
```
</details>

## TPP_MAKEFILE_HAVE_CLI_DASH_MQ

`-MQ TARGET`: Same as [`TPP_MAKEFILE_HAVE_CLI_DASH_MT`](#tpp_makefile_have_cli_dash_mt), but perform makefile escaping on `TARGET`

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI
```
</details>

## TPP_MAKEFILE_HAVE_CLI_DASH_MD

`-MD`: Similar to `-M`, but don't consume all input and instead auto-determine
output filename (unless specified by `-MF FILE`) based on the `output_filename`
argument passed to [`tpp_makefile_cli_loader_flush()`](../src/tpp-makefile-amalgamation.h#L1146) or `__FILE__`:

- If `output_filename` is given, then the makefile output is
  `output_filename.rpartition(".").first + ".d"` (unless that
  file does not contain a `"."` in its last path component, in
  which case the `".d"` is simply appended as-is)
  NOTE: The `".d"` here can be configured by [`TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION`](#tpp_makefile_config_default_extension)
- If `output_filename` isn't given, `__FILE__` is used instead
  of it, but with the same transformation as described above.

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI && TPP_MAKEFILE_HAVE_OUTPUT_FILE
```
</details>

## TPP_MAKEFILE_HAVE_CLI_DASH_MMD

`-MMD`: Same as `-MD`, but also turn on [`TPP_MAKEFILE_HAVE_USER_DEPENDENCIES`](#tpp_makefile_have_user_dependencies)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI && TPP_MAKEFILE_HAVE_OUTPUT_FILE && TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
```
</details>

## TPP_MAKEFILE_HAVE_CLI_DASH_MP

`-MP`: Turn on [`TPP_MAKEFILE_HAVE_PHONY`](#tpp_makefile_have_phony)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI && TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_PHONY)
```
</details>

## TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE

Controls whether [`TPP_MAKEFILE_HAVE_CLI_DASH_M`](#tpp_makefile_have_cli_dash_m)
and [`TPP_MAKEFILE_HAVE_CLI_DASH_MM`](#tpp_makefile_have_cli_dash_mm) will cause the
lexer's entire input to be consumed during a call
to [`tpp_makefile_cli_loader_flush()`](../src/tpp-makefile-amalgamation.h#L1146).

s.a.:

- [`tpp_makefile_cli_loader_getonlymakefile()`](../src/tpp-makefile-amalgamation.h#L1048)
- [`tpp_makefile_cli_loader_setonlymakefile()`](../src/tpp-makefile-amalgamation.h#L1050)

Configure as one of:

- `0`: Disabled
- `1`: Enabled

<details><summary>Details</summary>

Default:

```c
TPP_MAKEFILE_HAVE_CLI_DASH_M || TPP_MAKEFILE_HAVE_CLI_DASH_MM || TPP_MAKEFILE_HAVE_CLI_DASH_MG
```
</details>

## TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION

Default file extensions appended by `-MD` and `-MMD`

<details><summary>Details</summary>

Default:

```c
".d"
```
</details>

## TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY



<details><summary>Details</summary>

Default:

```c
0
```
</details>

## TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY



<details><summary>Details</summary>

Default:

```c
0
```
</details>

<!--END:all-->


# Glossary

<!--BEGIN:glossary-->
[`TPP_MAKEFILE_HAVE_USER_DEPENDENCIES`](#tpp_makefile_have_user_dependencies) [`TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES`](#tpp_makefile_have_missing_file_dependencies) [`TPP_MAKEFILE_HAVE_PHONY`](#tpp_makefile_have_phony) [`TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH`](#tpp_makefile_config_max_line_length) [`TPP_MAKEFILE_HAVE_IO_HANDLE`](#tpp_makefile_have_io_handle) [`TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO`](#tpp_makefile_have_output_file_io) [`TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE`](#tpp_makefile_have_output_file_io_noclose) [`TPP_MAKEFILE_HAVE_OUTPUT_FILE`](#tpp_makefile_have_output_file) [`TPP_MAKEFILE_HAVE_CLI`](#tpp_makefile_have_cli) [`TPP_MAKEFILE_HAVE_CLI_HELP`](#tpp_makefile_have_cli_help) [`TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS`](#tpp_makefile_have_cli_help_all_spellings) [`TPP_MAKEFILE_HAVE_CLI_DASH_M`](#tpp_makefile_have_cli_dash_m) [`TPP_MAKEFILE_HAVE_CLI_DASH_MM`](#tpp_makefile_have_cli_dash_mm) [`TPP_MAKEFILE_HAVE_CLI_DASH_MF`](#tpp_makefile_have_cli_dash_mf) [`TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH`](#tpp_makefile_have_cli_dash_mf_dash) [`TPP_MAKEFILE_HAVE_CLI_DASH_MG`](#tpp_makefile_have_cli_dash_mg) [`TPP_MAKEFILE_HAVE_CLI_DASH_MT`](#tpp_makefile_have_cli_dash_mt) [`TPP_MAKEFILE_HAVE_CLI_DASH_MQ`](#tpp_makefile_have_cli_dash_mq) [`TPP_MAKEFILE_HAVE_CLI_DASH_MD`](#tpp_makefile_have_cli_dash_md) [`TPP_MAKEFILE_HAVE_CLI_DASH_MMD`](#tpp_makefile_have_cli_dash_mmd) [`TPP_MAKEFILE_HAVE_CLI_DASH_MP`](#tpp_makefile_have_cli_dash_mp) [`TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE`](#tpp_makefile_have_cli_onlymakefile) [`TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION`](#tpp_makefile_config_default_extension) [`TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY`](#tpp_makefile_default_target_path_prefix_is_empty) [`TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY`](#tpp_makefile_default_target_filename_prefix_is_empty)
<!--END:glossary-->
