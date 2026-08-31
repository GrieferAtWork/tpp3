# API Limits / Numeric configs

These are numeric config options, meaning that when overwritten, they must expand to an integer constant expression.

- If that expression has a negative value, there will be some runtime facility to get/set the config, and the config macro's absolute value will be used as default
- If that expression has a positive value, that value will be hard-coded into TPP, and there will be no way to set a different value at runtime
- If configured to `0` (certain configs only), the meaning becomes configuration-specific, meaning "infinite" or "disabled"

<!--BEGIN:limit-->
## TPP_TABSIZE

TPP tab size: number of columns to assign to `U+0008` (`\t`) characters.
Used when calculating line/column information for messages, as well as
the builtin `__COLUMN__` macro (see [`TPP_HAVE_MACRO___COLUMN__`](config-conf.md#tpp_have_macro___column__))

- when positive: compile-time hard-code
- when negative: runtime-configurable, with absolute value being used as default

**Getter**: [`tpp_gettabsize()`](../src/tpp-amalgamation.h#L22296)<br/>
**Setter**: [`tpp_settabsize(v)`](../src/tpp-amalgamation.h#L22302)

<details><summary>Details</summary>

Default:

```c
(TPP_PROFILE == TPP_PROFILE_ALL) ? -4 : 4
```
</details>

## TPP_ERROR_LIMIT

Max # of [`TPP_WSTATE_ERROR`](../src/tpp-amalgamation.h#L23818) (continue compilation and fail later) warnings that
can be emitted before the next one will be treated as [`TPP_WSTATE_FATAL`](../src/tpp-amalgamation.h#L23823) (fail
immediately)

- `0`:  Set limit to `0` (when not also overwritten, causes `TPP_HAVE_WARNING_ERROR=0`)
- `N`:  Limit is hard-coded to `N` and cannot be overwritten at runtime
- `-N`: Limit can be overwritten at runtime, with `N` being used as the default

**Getter**: [`tpp_lexer_geterrorlimit(lexer)`](../src/tpp-amalgamation.h#L27024)<br/>
**Setter**: [`tpp_lexer_seterrorlimit(lexer, v)`](../src/tpp-amalgamation.h#L27025)

<details><summary>Details</summary>

Default:

```c
-16
```
</details>

## TPP_MAX_INCLUDE_DEPTH

Similar to GCC's `-fmax-include-depth`: Max # of times that the same file is allowed to
appear on the `#include`-stack before a warning [`TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED`](../src/tpp-amalgamation.h#L4968)
(which is default-configured to be an error) is emitted.

- `0`:  Disable include depth checks entirely (limit becomes *infinite*)
- `N`:  Limit is hard-coded to `N` and cannot be overwritten at runtime
- `-N`: Limit can be overwritten at runtime, with `N` being used as the default

**Getter**: [`tpp_lexer_getinclusionlimit(lexer)`](../src/tpp-amalgamation.h#L27055)<br/>
**Setter**: [`tpp_lexer_setinclusionlimit(lexer, v)`](../src/tpp-amalgamation.h#L27056)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_INCLUDE_STACK ? ((TPP_PROFILE != TPP_PROFILE_MINIMAL) ? -64 : 64) : 0
```
</details>

## TPP_MAX_RECURSIVE_MACRO_DEPTH

Max # of times that the same (self-recursive; see [`TPP_HAVE_MACRO_RECURSION`](config-conf.md#tpp_have_macro_recursion))
macro may appear on the `#include`-stack (with each instance's body having a
different expansion) before the macro will be treated as though its body
didn't change, causing it to not be expanded (anymore). At the same time
that this limit is reached, a warning [`TPP_W_MACRO_RECURSION_LIMIT_EXCEEDED`](../src/tpp-amalgamation.h#L4974)
will be emitted.

NOTE: When [`TPP_HAVE_MACRO_RECURSION`](config-conf.md#tpp_have_macro_recursion) can never be enabled, this limit is
      entirely pointless, since the C standard (which we follow whenever
      [`TPP_HAVE_MACRO_RECURSION`](config-conf.md#tpp_have_macro_recursion) isn't enabled) essentially requires that
      this limit be `1`.

- `0`:  Disable depth checks entirely (limit becomes *infinite*)
- `N`:  Limit is hard-coded to `N` and cannot be overwritten at runtime
- `-N`: Limit can be overwritten at runtime, with `N` being used as the default

**Getter**: [`tpp_lexer_getrecursivemacrolimit(lexer)`](../src/tpp-amalgamation.h#L27075)<br/>
**Setter**: [`tpp_lexer_setrecursivemacrolimit(lexer, v)`](../src/tpp-amalgamation.h#L27076)

<details><summary>Details</summary>

Default:

```c
TPP_HAVE_MACRO_RECURSION ? ((TPP_PROFILE != TPP_PROFILE_MINIMAL) ? -4096 : 4096) : 0
```
</details>

<!--END:limit-->
