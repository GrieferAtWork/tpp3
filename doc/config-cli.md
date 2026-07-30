# Builtin CLI parser configuration

In order to help you more easily create CLI frontends for TPP, there also exists a builtin CLI parser when `TPP_HAVE_CLI_LOADER` is enabled. To allow you to configure this parser, the following configurations are provided.

<!--BEGIN:cli-->
## TPP_HAVE_CLI

Provide an API surrounding [`tpp_cli_loader`](../src/tpp-amalgamation.h#L24889), which can be used to configure a lexer
using GCC-style commandline arguments like `-Dfoo=bar`, `-I/usr/include`, etc.

This API is entirely optional: there's nothing it can do that can't already
be done using some other C API; it's only there as a convenience to you.

The CLI loader must be used on a lexer that has already been initialized
itself (as per [`tpp_lexer_init()`](../src/tpp-amalgamation.h#L23063)), though whether or not the its initial
file has already been initialized doesn't matter (the CLI loader will never
make persistent modifications to a lexer's current file/token).

```c
int main(int argc, char **argv) {
    int result = 1;
    char *appname = argv[0];
    tpp_errno error;
    tpp_lexer lexer;
    tpp_cli_loader cli_loader;
    tpp_lexer_init(&lexer);
    tpp_cli_loader_init(&cli_loader, &lexer);
    if (argc)
        --argc, ++argv; // Skip "appname" argument
    error = tpp_cli_loader_parseargv(&cli_loader, &argc, &argv);
    if (TPP_ISERR(error)) {
        fprintf(stderr, "failed to parse arguments: %s\n", tpp_strerror(error));
        goto out_lexer;
    }
    // Normally, you'd be parsing your own input arguments at this point
    if (argc && strcmp(*argv, "--") == 0)
        --argc, ++argv;
    if (argc != 1) {
        fprintf(stderr, "bad arguments\nUSAGE: %s [ARGS...] INFILE\n", appname);
        goto out_lexer;
    }
    error = tpp_lexer_initfile_open(&lexer, argv[0], TPP_SIZE_MAX);
    if (TPP_ISERR(error)) {
        fprintf(stderr, "failed to open '%s': %s\n", argv[0], tpp_strerror(error));
        goto out_lexer;
    }
    error = tpp_cli_loader_flush(&cli_loader);
    for (;;) {
        tpp_token_id tok = tpp_lexer_yield(&lexer);
        if (tok == TPP_TOK_EOF)
            break;
        if (TPP_TOK_ISERR(tok)) {
            fprintf(stderr, "yield failed: %s\n", tpp_strerror(TPP_TOK_ASERR(tok)));
            goto out_lexer_file;
        }
        fwrite(tpp_lexer_gettokenstart(&lexer), 1,
               tpp_lexer_gettokenlen(&lexer), stdout);
    }
    result = 0;
out_lexer_file:
    tpp_lexer_finifile(&lexer);
out_lexer:
    tpp_lexer_fini(&lexer);
    return result;
}
```

<details><summary>Details</summary>

Default:

```c
TPP_PROFILE == TPP_PROFILE_ALL
```
</details>

## TPP_HAVE_CLI_DASH_DEFINE_MACRO

`-Dmacro[=def]`, `-D macro[=def]`,
`--define-macro=macro[=def]`, `--define-macro macro[=def]`:
Define an additional macro as `#define macro def` (or
`#define macro 1` when `def` isn't given)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_HAVE_LEXER_CLI_DEFINE
```
</details>

## TPP_HAVE_CLI_DASH_UNDEFINE_MACRO

`-Umacro`, `-U macro`, `--undefine-macro=macro`, `--undefine-macro macro`:
Delete a macro definition, the same way `#undef macro` would.

Implementation makes use of: [`tpp_lexer_define()`](../src/tpp-amalgamation.h#L23434) + [`tpp_lexer_undef()`](../src/tpp-amalgamation.h#L23442)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_HAVE_LEXER_CLI_DEFINE
```
</details>

## TPP_HAVE_CLI_DASH_ASSERT

`-Apredicate=answer`, `-A predicate=answer`, `--assert=predicate=answer`,
`--assert predicate=answer`, `-A-predicate[=answer]`, `-A -predicate[=answer]`,
`--assert=-predicate[=answer]`, `--assert -predicate[=answer]`:
Define or delete a preprocessor *"assertion"* (see [`TPP_HAVE_CPP_ASSERT`](config-conf.md#tpp_have_cpp_assert)).

Implementation makes use of: [`tpp_lexer_assert()`](../src/tpp-amalgamation.h#L23458) + [`tpp_lexer_unassert()`](../src/tpp-amalgamation.h#L23467) +
                             [`tpp_lexer_unassertall()`](../src/tpp-amalgamation.h#L23474)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_HAVE_LEXER_CLI_ASSERT
```
</details>

## TPP_HAVE_CLI_DASH_INCLUDE

`-include FILE`:
causes `FILE` to be injected as though it was `#include`-ed
at the start of the lexer's main input file.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_HAVE_INCLUDE_STACK && TPP_HAVE_LEXER_OPENFILE
```
</details>

## TPP_HAVE_CLI_DASH_IMACROS

`-imacros <file>`, `--imacros=<file>`, `--imacros <file>`:
Similar to [`TPP_HAVE_CLI_DASH_INCLUDE`](#tpp_have_cli_dash_include), but rather than including
the file at the start of the lexer's main input, it is instead
included right now, with all of its macros and pragma directives
processed as per usual (including expansion of macros within,
meaning that deeply nested pragmas also take effect), but any
tokens produced by it are discarded.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_HAVE_LEXER_OPENFILE && TPP_HAVE_CPP_MACROS
```
</details>

## TPP_HAVE_CLI_DASH_UNDEF

`-undef`:
Undef all *predefined* macros (done by disabling [`TPP_HAVE_CPP_PREDEFINED_MACROS`](config-conf.md#tpp_have_cpp_predefined_macros))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_CONF_IS_RT(TPP_HAVE_CPP_PREDEFINED_MACROS)
```
</details>

## TPP_HAVE_CLI_DASH_FEXTENSION

`-f...`, `-fno-...`:
Allow TPP extensions to be turned on/off via the commandline. Syntax here is the
same as in `#pragma TPP extension(...)`, such that `-fEXTENSION` turns `EXTENSION`
on, whilst `-fno-EXTENSION` turns it off.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_HAVE_EXTENSIONS
```
</details>

## TPP_HAVE_CLI_DASH_FPREPROCESSED

`-fpreprocessed`, `-fno-preprocessed`:
Turns the following lexer features off when enabled (or turns them on when disabled):

- [`TPP_HAVE_CPP_MACROS`](config-conf.md#tpp_have_cpp_macros)
- [`TPP_HAVE_TRIGRAPHS`](config-conf.md#tpp_have_trigraphs)
- [`TPP_HAVE_BSE`](config-conf.md#tpp_have_bse)
- [`TPP_HAVE_CPP_INCLUDE`](config-conf.md#tpp_have_cpp_include)
- [`TPP_HAVE_CPP_INCLUDE_NEXT`](config-conf.md#tpp_have_cpp_include_next)
- [`TPP_HAVE_CPP_IMPORT`](config-conf.md#tpp_have_cpp_import)
- [`TPP_HAVE_CPP_IF_ELSE_ENDIF`](config-conf.md#tpp_have_cpp_if_else_endif)
- [`TPP_HAVE_CPP_DEFINE`](config-conf.md#tpp_have_cpp_define)
- [`TPP_HAVE_CPP_ASSERT`](config-conf.md#tpp_have_cpp_assert)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && (TPP_CONF_IS_RT(TPP_HAVE_CPP_MACROS) || TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS) || TPP_CONF_IS_RT(TPP_HAVE_BSE) || TPP_CONF_IS_RT(TPP_HAVE_CPP_INCLUDE) || TPP_CONF_IS_RT(TPP_HAVE_CPP_INCLUDE_NEXT) || TPP_CONF_IS_RT(TPP_HAVE_CPP_IMPORT) || TPP_CONF_IS_RT(TPP_HAVE_CPP_IF_ELSE_ENDIF) || TPP_CONF_IS_RT(TPP_HAVE_CPP_DEFINE) || TPP_CONF_IS_RT(TPP_HAVE_CPP_ASSERT))
```
</details>

## TPP_HAVE_CLI_DASH_FDIRECTIVES_ONLY

`-fdirectives-only`, `-fno-directives-only`:
Turns off expansion of macros when enabled (but turns expansion back on when disabled)

s.a. [`TPP_HAVE_CPP_MACROS`](config-conf.md#tpp_have_cpp_macros)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_CONF_IS_RT(TPP_HAVE_CPP_MACROS)
```
</details>

## TPP_HAVE_CLI_DASH_FDOLLARS_IN_IDENTIFIERS

`-fdollars-in-identifiers`:
Turns off `$` being treated as a distinct token when enabled.
Essentially does the inverse of `-ftok-dollar` (s.a. [`TPP_HAVE_TOK_DOLLAR`](config-conf.md#tpp_have_tok_dollar))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_CONF_IS_RT(TPP_HAVE_TOK_DOLLAR)
```
</details>

## TPP_HAVE_CLI_DASH_FTABSTOP

`-ftabstop=<width>`:
Configure the number of columns to assign to `U+0008` (`\t`) characters.

WARNING: This is a *GLOBAL* config, meaning if you may run into problems
         if you have multiple threads with multiple lexers all running
         in parallel, with each of them also trying to set its own,
         different value for `-ftabstop`!

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && (TPP_TABSIZE < 0)
```
</details>

## TPP_HAVE_CLI_DASH_COMMENTS

`-C`, `-CC`, `--comments`, `--comments-in-macros`:
Enable emission of comment-like tokens in output. Without this, comments
are simply skipped the same way that preprocessor directives and macros
that expand to nothing are skipped.

NOTE: TPP doesn't differentiate between comments in-source and comments
      in macros, so both of these CLI switches are handled the same by
      turning on emission of comment tokens everywhere.

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_CONF_IS_RT(TPP_HAVE_TOK_COMMENT)
```
</details>

## TPP_HAVE_CLI_DASH_TRADITIONAL

`-traditional`, `--traditional`, `-traditional-cpp`, `--traditional-cpp`:
Enable traditional macro expansion rules (by default). Also turns off
a couple of other features (assuming those features can be turned off
at runtime):

- [`TPP_HAVE_TRIGRAPHS`](config-conf.md#tpp_have_trigraphs)
- [`TPP_HAVE_TOK_CXX_COMMENT`](config-conf.md#tpp_have_tok_cxx_comment)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_CONF_IS_RT(TPP_HAVE_TRADITIONAL_MACROS)
```
</details>

## TPP_HAVE_CLI_DASH_TRIGRAPHS

`-trigraphs`, `--trigraphs`:
Turns on processing of trigraphs (see [`TPP_HAVE_TRIGRAPHS`](config-conf.md#tpp_have_trigraphs))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS)
```
</details>

## TPP_HAVE_CLI_DASH_INCLUDE_BARRIER

`-I-`, `--include-barrier`:
Move `-I`-style include paths to `-iquote`, then turn off
[`TPP_HAVE_INCLUDE_RELATIVE_TO_CURRENT_FILE`](config-conf.md#tpp_have_include_relative_to_current_file).

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_HAVE_INCLUDE_PATH && TPP_CONF_IS_RT(TPP_HAVE_INCLUDE_RELATIVE_TO_CURRENT_FILE)
```
</details>

## TPP_HAVE_CLI_DASH_INCLUDE_DIRECTORY

`-IPATH`, `--include-directory=PATH`:
Add another include paths for `#include <file>`-style includes

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_HAVE_INCLUDE_PATH
```
</details>

## TPP_HAVE_CLI_DASH_NOSTDINC

`-nostdinc`, `--no-standard-includes`:
Disable searching for standard system include paths
(s.a. [`TPP_HAVE_INCLUDE_SYSTEM_INCLUDE_PATH`](config-conf.md#tpp_have_include_system_include_path))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_CONF_IS_RT(TPP_HAVE_INCLUDE_SYSTEM_INCLUDE_PATH)
```
</details>

## TPP_HAVE_CLI_DASH_WERROR

`-Werror`, `-Wno-error`:
Treat all warnings as errors (s.a. [`TPP_HAVE_WERROR`](config-conf.md#tpp_have_werror))

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_CONF_IS_RT(TPP_HAVE_WERROR)
```
</details>

## TPP_HAVE_CLI_DASH_WWARNING

`-W...`, `-Wno-...`:
Turn emission of a specific warning on/off (similar to `#pragma TPP warning("-W...")`).

When turned off, the warning state is set to [`TPP_WSTATE_DISABLED`](../src/tpp-amalgamation.h#L21091). When turned on,
the warning state is gradually increased from what it's previous state was:

- [`TPP_WSTATE_DISABLED`](../src/tpp-amalgamation.h#L21091) is changed to [`TPP_WSTATE_WARN`](../src/tpp-amalgamation.h#L21092)
- [`TPP_WSTATE_WARN`](../src/tpp-amalgamation.h#L21092) is changed to [`TPP_WSTATE_ERROR`](../src/tpp-amalgamation.h#L21094) (if [`TPP_HAVE_WARNING_ERROR`](config-core.md#tpp_have_warning_error)
  is available; else, changed to [`TPP_WSTATE_FATAL`](../src/tpp-amalgamation.h#L21099) instead)
- [`TPP_HAVE_WARNING_ERROR`](config-core.md#tpp_have_warning_error) is changed to [`TPP_WSTATE_FATAL`](../src/tpp-amalgamation.h#L21099)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_CLI && TPP_HAVE_WARNINGS
```
</details>

<!--END:cli-->
