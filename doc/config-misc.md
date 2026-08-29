# Misc configurations

In addition to configuration options defined in the `parts/config.h` portion of TPP, there are a number of additional configs / sets-of-identifiers that you can (pre-)define to override / customize a number of TPP's APIs.

The most notable of these are macros from the `parts/api.h` portion of TPP, but there are even more of them, all of which are documented here.

To override any of these, simply `#define` a macro of the same name alongside any other configuration macro definitions you're making to configure TPP.


<!-- XXX: Rename this file to `config-misc.md.in` -->
<!-- XXX: The config-generator script should parse `config-misc.md.in` and output
          `config-misc.md`, whilst doing the usual injection of line-references to
          `tpp-amalgamation.h`, as well as the names of other configs.
          It should also treat `## FOO` headers in this file as the names of more
          configs that:
          - should be linked if referenced by-name in other config files
          - should be included in the big config glossary in `config.md`
-->


## TPP_CONFIG_BUILTINS_FILENAME

Name of a file relative to `tpp-amalgamation.c`. This file should be the output of `/src/makecache.py`, and can be used to speed up TPP initialization by pre-generating definitions for builtin keywords (rather than initializing those definitions lazily at runtime).

For an example of a (simple) project that uses `TPP_CONFIG_BUILTINS_FILENAME`, see [`/samples/simple-with-cache`](../samples/simple-with-cache).

<details><summary>Details</summary>

Example:

```c
#define TPP_CONFIG_BUILTINS_FILENAME "../../path/to/build/tpp/builtins-cache.h"
```
</details>



## TPP_AMALGAMATION_H

Define to the relative `#include "filename"` with which `tpp-amalgamation.c` can include `tpp-amalgamation.h`.

By default, this is simply defined as `"tpp-amalgamation.h"`, which assumes that you've put `tpp-amalgamation.h` and `tpp-amalgamation.c` into the same directory. If you've decided to separate the 2, you must re-define this config to be whatever `#include`-string is needed for `tpp-amalgamation.c` to `#include TPP_AMALGAMATION_H` the associated `tpp-amalgamation.h`.

<details><summary>Details</summary>

Example:

```c
#define TPP_AMALGAMATION_H "../../path/to/tpp/tpp-amalgamation.h"
```
</details>



## TPP_OS_WINDOWS

Define to `1` if the host OS is windows/dos-like. Otherwise, defined to `0`

<details><summary>Details</summary>

Example:

```c
#define TPP_OS_WINDOWS 1
```
</details>



## TPP_OS_UNIX

Define to `1` if the host OS is unix/linux-like. Otherwise, defined to `0`

<details><summary>Details</summary>

Example:

```c
#define TPP_OS_UNIX 1
```
</details>



## TPP_HOST_NO_SYSTEM_INCLUDES

Define to `1` to prevent TPP from ever doing `#include <some/header.h>`. This allows you to use TPP in an environment where you're supplying your own definitions for system functions needed by TPP (see below), and don't want to bother emulating various headers like `<stdlib.h>` or `<unistd.h>`

<details><summary>Details</summary>

Example:

```c
#define TPP_HOST_NO_SYSTEM_INCLUDES 1
```
</details>



## TPP_HOST_HAS_ATTRIBUTE

Wrapper around `__has_attribute` for host compiler.

<details><summary>Details</summary>

Example:

```c
#define TPP_HOST_HAS_ATTRIBUTE(x) __has_attribute(x)
```
</details>



## TPP_HOST_HAS_DECLSPEC_ATTRIBUTE

Wrapper around `__has_declspec_attribute` for host compiler.

<details><summary>Details</summary>

Example:

```c
#define TPP_HOST_HAS_DECLSPEC_ATTRIBUTE(x) __has_declspec_attribute(x)
```
</details>



## TPP_HOST_HAS_CPP_ATTRIBUTE

Wrapper around `__has_cpp_attribute` for host compiler.

<details><summary>Details</summary>

Example:

```c
#define TPP_HOST_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
```
</details>



## TPP_HOST_HAS_INCLUDE

Wrapper around `__has_include` for host compiler.

<details><summary>Details</summary>

Example:

```c
#define TPP_HOST_HAS_INCLUDE(x) __has_include(x)
```
</details>



## TPP_HOST_HAS_BUILTIN

Wrapper around `__has_builtin` for host compiler.

<details><summary>Details</summary>

Example:

```c
#define TPP_HOST_HAS_BUILTIN(x) __has_builtin(x)
```
</details>



## TPP_HOST_HAVE_PP_VARARGS

Define to `1` if host compiler/preprocessor supports `#define varargs(...) __VA_ARGS__`-style macro definitions.

<details><summary>Details</summary>

Example:

```c
#define TPP_HOST_HAVE_PP_VARARGS 1
```
</details>



## TPPCALL

Calling convention to use in TPP APIs. Injected before function names of `TPP_DECL` (see below) function definitions:

```c
TPP_DECL int TPPCALL tpp_function(void);
```

<details><summary>Details</summary>

Example:

```c
#define TPPCALL __attribute__((cdecl))
```
</details>



## TPPVCALL

Same as `TPPCALL`, but for varargs function definitions:

```c
TPP_DECL int TPPVCALL tpp_function(char const *format, ...);
```

<details><summary>Details</summary>

Example:

```c
#define TPPVCALL __attribute__((cdecl))
```
</details>



## TPP_GCC_VERSION_NUM

The version number of a hosting GCC compiler, or `0` if the hosting compiler isn't GCC-compatible.

If the hosting compiler is GCC-compatible, this macro should be defined like `__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCH__`

<details><summary>Details</summary>

Example:

```c
#define TPP_GCC_VERSION_NUM (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCH__)
```
</details>



## TPP_USE_STATIC

Define to `1` to have TPP define its entire API using `static`. This means that everything that makes up TPP is confined to a single compilation unit, meaning that:
- You could in theory have multiple, distinctly configured instances of TPP within the same application
- Everything that makes up TPP is **only** accessible from the compilation unit that is including `tpp-amalgamation.c` (and if you want to access some part of TPP from outside that compilation unit, you'll have to write your own wrappers)
- The hosting compiler can better optimize TPP since anything that could normally only be done via LTO (link-time optimization) can now be done directly

<details><summary>Details</summary>

Example:

```c
#define TPP_USE_STATIC 1
```
</details>



## TPP_IMPL

The declaration prefix used by `tpp-amalgamation.c` when implemented API functions that were previously defined by `tpp-amalgamation.h`

<details><summary>Details</summary>

Example:

```c
#define TPP_IMPL __attribute__((visibility("hidden")))
```
</details>



## TPP_DECL

The declaration prefix used by `tpp-amalgamation.h` when defining API functions will later be implemented by `tpp-amalgamation.c`

<details><summary>Details</summary>

Example:

```c
#define TPP_DECL extern __attribute__((visibility("hidden")))
```
</details>



## TPP_CONST_IMPL

Same as `TPP_IMPL`, but used for implementing `const` data objects (like lookup arrays and the like). This macro also comes with a companion `TPP_CONST_DECL`, but that one cannot be overwritten. Additionally `TPP_CONST_DECL` (intentionally) isn't defined when `-DTPP_USE_STATIC=1`, since C doesn't offer a way to do forward-declaration of static data objects.

<details><summary>Details</summary>

Example:

```c
#ifdef __cplusplus
#define TPP_CONST_IMPL extern TPP_IMPL
#else /* __cplusplus */
#define TPP_CONST_IMPL TPP_IMPL
#endif /* !__cplusplus */
```
</details>



## tpp_assume

```c
tpp_assume(expression_that_is_always == true);
```

Hint to the host compiler that expressions are always true, and that it is undefined behavior if they somehow wouldn't be.

<details><summary>Details</summary>

Example:

```c
#if __has_builtin(__builtin_assume)
#define tpp_assume(expr) __builtin_assume(expr)
#else
#define tpp_assume(expr) (void)0
#endif
```
</details>



## tpp_restrict

Macro that should expand to `restrict` (or an equivalent keyword), so-long as the host compiler supports a C99-compliant `restrict` semantics. If the host compiler doesn't support C99-compliant `restrict`, this macro should be defined as empty.

<details><summary>Details</summary>

Example:

```c
#define tpp_restrict restrict
```
</details>



## TPP_NONNULL

Should be defined as a function-like macro to expand to GCC-like `__attribute__((nonnull(1, 2, 3)))` annotations: `TPP_NONNULL((1, 2, 3))` (note the double parenthesis, which is intentional so-as not to rely on the host compiler supporting `TPP_HOST_HAVE_PP_VARARGS`)

<details><summary>Details</summary>

Example:

```c
#define TPP_NONNULL(params) __attribute__((nonnull params))
```
</details>



## TPP_WUNUSED

Wrapper around GCC's `__attribute__((warn_unused_result))`

<details><summary>Details</summary>

Example:

```c
#define TPP_WUNUSED __attribute__((warn_unused_result))
```
</details>



## TPP_RETNONNULL

Wrapper around GCC's `__attribute__((returns_nonnull))`

<details><summary>Details</summary>

Example:

```c
#define TPP_RETNONNULL __attribute__((returns_nonnull))
```
</details>



## TPP_PURECALL

Wrapper around GCC's `__attribute__((pure))`

<details><summary>Details</summary>

Example:

```c
#define TPP_PURECALL __attribute__((pure))
```
</details>



## TPP_CONSTCALL

Wrapper around GCC's `__attribute__((const))`

<details><summary>Details</summary>

Example:

```c
#define TPP_CONSTCALL __attribute__((const))
```
</details>



## TPP_COLDCALL

Wrapper around GCC's `__attribute__((cold))`

<details><summary>Details</summary>

Example:

```c
#define TPP_COLDCALL __attribute__((cold))
```
</details>



## TPP_NOINLINE

Wrapper around GCC's `__attribute__((noinline))`

<details><summary>Details</summary>

Example:

```c
#define TPP_NOINLINE __attribute__((noinline))
```
</details>



## TPP_FLEX_ARRAY

Should expand to whatever must be written between the `[brackets]` of a trailing flexible-array-member in `struct` definitions, such that the host compiler understand the construct.

Note that TPP never invokes `sizeof()` on a structure that ends with a trailing `T array[TPP_FLEX_ARRAY]` member, but instead uses `offsetof(S, array)` in these cases, so if the host compiler doesn't understand flexible arrays you can just define this as some very larger integer values.

<details><summary>Details</summary>

Example:

```c
#define TPP_FLEX_ARRAY  /* nothing */
```
</details>



## TPP_FALLTHRU

Wrapper around GCC's `__attribute__((fallthrough));` (note the trailing `;`) -- this macro is inserted wherever TPP intentionally does a fallthrough from one switch-case to another.

<details><summary>Details</summary>

Example:

```c
#define TPP_FALLTHRU __attribute__((fallthrough));
```
</details>



## TPP_CHAR_BIT

Should expand to `CHAR_BIT`

<details><summary>Details</summary>

Example:

```c
#define TPP_CHAR_BIT 8
```
</details>



## TPP_INTERNAL

You probably shouldn't override this macro (the same way you shouldn't override `TPP_BUILDING` or `TPP_BUILDING_OPTIONAL`). This macro is used to guard (by renaming) *hidden* / *internal* parts of TPP's API, so you don't get any ideas about directly accessing internals you shouldn't be tinkering with.

By default, this macro prepends a leading `_` before the identifier it is given, following the general convention that any identifier with a leading `_` that might appear in `tpp-amalgamation.h` should be considered off-limits.

<details><summary>Details</summary>

Example:

```c
#define TPP_INTERNAL(x) _##x
```
</details>



## TPP_HAVE_TPP2_COMPAT

Define to `1` to enable some minor TPP2 compatibility hacks that are required by the `tpp2.h` compatibility header. If you're using TPP3 as intended, you should never need to overwrite this macro or define it to `1`.

<details><summary>Details</summary>

Example:

```c
#define TPP_HAVE_TPP2_COMPAT 0
```
</details>



## TPP_DEBUG

Define to `1` to enable some additional code used by TPP to debug itself. This includes internal assertions (see `tpp_assert`), as well as the trashing of structures during finalization.

<details><summary>Details</summary>

Example:

```c
#ifdef NDEBUG
#define TPP_DEBUG 0
#else /* NDEBUG */
#define TPP_DEBUG 1
#endif /* !NDEBUG */
```
</details>



## TPP_INLINE

Expand to the prefix for static+inline function definitions.

<details><summary>Details</summary>

Example:

```c
#define TPP_INLINE static __inline__
```
</details>



## tpp_offsetof

Wrapper around `offsetof()` as normally defined in `<stddef.h>`

<details><summary>Details</summary>

Example:

```c
#define tpp_offsetof(s, m) ((size_t)&((s *)0)->m)
```
</details>



## tpp_container_of

A helper macro to (safely) extract the parent-container from a pointer to one of its children, given the type of the parent-container, the name of the field in question, and a pointer to said field.

<details><summary>Details</summary>

Example:

```c
#define tpp_container_of(p, s, m) ((s *)((char *)(p) - tpp_offsetof(s, m)))
```
</details>



## tpp_lengthof

Helper macro to determine the length (in elements) of a statically allocated array.

<details><summary>Details</summary>

Example:

```c
#define tpp_lengthof(a) (sizeof(a) / sizeof(*(a)))
```
</details>



## tpp_unreachable

Wrapper around GCC's `__builtin_unreachable`

<details><summary>Details</summary>

Example:

```c
#define tpp_unreachable() __builtin_unreachable()
```
</details>



## tpp_expect

Wrapper around GCC's `__builtin_expect`

<details><summary>Details</summary>

Example:

```c
#ifdef __GNUC__
#define tpp_expect(expr, expected) __builtin_expect(expr, expected)
#else /* __GNUC__ */
#define tpp_expect(expr, expected) expr
#endif /* !__GNUC__ */
```
</details>



## tpp_likely

Wrapper around `tpp_expect` that is used like `if tpp_likely(expr)`

<details><summary>Details</summary>

Example:

```c
#define tpp_likely(x) (tpp_expect(!!(x), 1))
```
</details>



## tpp_unlikely

Wrapper around `tpp_expect` that is used like `if tpp_unlikely(expr)`

<details><summary>Details</summary>

Example:

```c
#define tpp_unlikely(x) (tpp_expect(!!(x), 0))
```
</details>



## tpp_uint_least8, tpp_int_least8, TPP_UINT_LEAST8_MAX, TPP_UINT_LEAST8_C

Types, limits and constant-creation for integer types that must have at least 8 bits of data

<details><summary>Details</summary>

Example:

```c
#include <stdint.h>

#define tpp_uint_least8      uint_least8_t
#define tpp_int_least8       int_least8_t
#define TPP_UINT_LEAST8_MAX  UINT_LEAST8_MAX
#define TPP_UINT_LEAST8_C(x) UINT8_C(x)  // OPTIONAL
```
</details>



## tpp_uint_least16, tpp_int_least16, TPP_UINT_LEAST16_MAX, TPP_UINT_LEAST16_C

Types, limits and constant-creation for integer types that must have at least 16 bits of data

<details><summary>Details</summary>

Example:

```c
#include <stdint.h>

#define tpp_uint_least16      uint_least16_t
#define tpp_int_least16       int_least16_t
#define TPP_UINT_LEAST16_MAX  UINT_LEAST16_MAX
#define TPP_UINT_LEAST16_C(x) UINT16_C(x)  // OPTIONAL
```
</details>



## tpp_uint_least32, tpp_int_least32, TPP_UINT_LEAST32_MAX, TPP_UINT_LEAST32_C

Types, limits and constant-creation for integer types that must have at least 32 bits of data

<details><summary>Details</summary>

Example:

```c
#include <stdint.h>

#define tpp_uint_least32      uint_least32_t
#define tpp_int_least32       int_least32_t
#define TPP_UINT_LEAST32_MAX  UINT_LEAST32_MAX
#define TPP_UINT_LEAST32_C(x) UINT32_C(x)  // OPTIONAL
```
</details>



## tpp_uint_least64, tpp_int_least64, TPP_UINT_LEAST64_MAX, TPP_UINT_LEAST64_C

Types, limits and constant-creation for integer types that must have at least 64 bits of data.

Unlike the other types above, this one is actually optional (if left undefined, TPP will make an attempt to define these itself, but if that also fails, then TPP is still able to function as normal (though certain features might behave differently or are more restrictive that they would otherwise be))

<details><summary>Details</summary>

Example:

```c
#include <stdint.h>

#define tpp_uint_least64      uint_least64_t
#define tpp_int_least64       int_least64_t
#define TPP_UINT_LEAST64_MAX  UINT_LEAST64_MAX
#define TPP_UINT_LEAST64_C(x) UINT64_C(x)  // OPTIONAL
```
</details>



## tpp_uint_fast8, tpp_int_fast8, TPP_UINT_FAST8_MAX, TPP_UINT_FAST8_C

Same as `tpp_uint_least8` with the same requirement of having at least 8 bits of data. However, this type may be larger than `tpp_uint_least8` if making it larger might improve performance on whatever host architecture TPP is being compiled for.

<details><summary>Details</summary>

Example:

```c
#include <stdint.h>

#define tpp_uint_fast8      uint_fast8_t
#define tpp_int_fast8       int_fast8_t
#define TPP_UINT_FAST8_MAX  UINT_FAST8_MAX
#define TPP_UINT_FAST8_C(x) UINT8_C(x)  // OPTIONAL
```
</details>



## tpp_uint_fast16, tpp_int_fast16, TPP_UINT_FAST16_MAX, TPP_UINT_FAST16_C

Same as `tpp_uint_least16` with the same requirement of having at least 16 bits of data. However, this type may be larger than `tpp_uint_least16` if making it larger might improve performance on whatever host architecture TPP is being compiled for.

<details><summary>Details</summary>

Example:

```c
#include <stdint.h>

#define tpp_uint_fast16      uint_fast16_t
#define tpp_int_fast16       int_fast16_t
#define TPP_UINT_FAST16_MAX  UINT_FAST16_MAX
#define TPP_UINT_FAST16_C(x) UINT16_C(x)  // OPTIONAL
```
</details>



## tpp_uint_fast32, tpp_int_fast32, TPP_UINT_FAST32_MAX, TPP_UINT_FAST32_C

Same as `tpp_uint_least32` with the same requirement of having at least 32 bits of data. However, this type may be larger than `tpp_uint_least32` if making it larger might improve performance on whatever host architecture TPP is being compiled for.

<details><summary>Details</summary>

Example:

```c
#include <stdint.h>

#define tpp_uint_fast32      uint_fast32_t
#define tpp_int_fast32       int_fast32_t
#define TPP_UINT_FAST32_MAX  UINT_FAST32_MAX
#define TPP_UINT_FAST32_C(x) UINT32_C(x)  // OPTIONAL
```
</details>



## tpp_intmax, tpp_uintmax, TPP_UINTMAX_MAX, TPP_UINTMAX_C

The largest integer type that should be supported by TPP. This is also the integer type that is used by various APIs, including TPP's builtin integer decoding facilities.

<details><summary>Details</summary>

Example:

```c
#include <stdint.h>

#define tpp_intmax       intmax_t
#define tpp_uintmax      uintmax_t
#define TPP_UINTMAX_MAX  UINTMAX_MAX
#define TPP_UINTMAX_C(x) UINTMAX_C(x)  // OPTIONAL
```
</details>



## tpp_size, TPP_SIZE_MAX

Wrapper around `size_t` and `SIZE_MAX` from `<stddef.h>`

<details><summary>Details</summary>

Example:

```c
#include <stddef.h>

#define tpp_size     size_t
#define TPP_SIZE_MAX SIZE_MAX
```
</details>



## tpp_ssize, TPP_SSIZE_MAX

Wrapper around `ptrdiff_t` and `PTRDIFF_MAX` from `<stddef.h>`

<details><summary>Details</summary>

Example:

```c
#include <stddef.h>

#define tpp_ssize     ptrdiff_t
#define TPP_SSIZE_MAX PTRDIFF_MAX
```
</details>



## tpp_hash, TPP_HASH_MAX, TPP_HASH_C

The type used by TPP to store keyword hashes. By default, this aliases `tpp_uint_fast32`

<details><summary>Details</summary>

Example:

```c
#define tpp_hash      tpp_uint_fast32
#define TPP_HASH_MAX  TPP_UINT_FAST32_MAX
#define TPP_HASH_C(x) TPP_UINT_FAST32_C(x)
```
</details>



## tpp_line, tpp_column

The types used by TPP to represent line/column numbers. By default, these alias `tpp_int_fast32`

<details><summary>Details</summary>

Example:

```c
#define tpp_line   tpp_int_fast32
#define tpp_column tpp_int_fast32
```
</details>



## tpp_char

The character type used when working with data from files. This pretty much **has** to be defined as `unsigned char`, since this *must* represent whatever data type is needed to describe file-bytes on a level that can also describe utf-8 bytes, **and** must be `undefined`, since `tpp_char` values are often used as indices in various lookup tables.

<details><summary>Details</summary>

Example:

```c
#define tpp_char unsigned char
```
</details>



## tpp_unichar, TPP_UNICHAR_C

The character type used to represent decoded unicode/utf-32/UCS-32 characters. By default, this aliases `tpp_uint_least32`

<details><summary>Details</summary>

Example:

```c
#define tpp_unichar   tpp_uint_least32
#define TPP_UNICHAR_C TPP_UINT_LEAST32_C
```
</details>



## tpp_counter

The data type that is internally used by the implementation of `__COUNTER__` and `__TPP_COUNTER` (see `TPP_HAVE_MACRO___COUNTER__` and `TPP_HAVE_MACRO___TPP_COUNTER`)

<details><summary>Details</summary>

Example:

```c
#define tpp_counter tpp_uint_least32
```
</details>



## tpp_float

The floating-point data type used by TPP's builtin float-decoder (see `tpp_lexer_decodefloat()` and `TPP_HAVE_LEXER_DECODEFLOAT`), as well as `#if`-style preprocessor expressions when `TPP_HAVE_BUILTIN_EXPR_FLOATS` is enabled (see `tpp_expr_value` below)

<details><summary>Details</summary>

Example:

```c
#define tpp_float long double
```
</details>



## TPP_REF

A hint annotation to indicate that a variable/field/return-value/parameter carries/stores a *reference-counted* to the pointed-to structure. Primarily used with `tpp_string`, in which case this macro may be overwritten by static analysis tools to trace ownership passing of reference-counted objects.

<details><summary>Details</summary>

Example:

```c
#define TPP_REF __attribute__((ref))
```
</details>



## TPP_STATIC_ASSERT

A function-like macro that performs compiler-time (as opposed to preprocessor-time, or runtime) assertion of a constant expressions. Used by TPP to assert some internal consistencies that would otherwise cause runtime problems, or hard-to-understand compilation errors further down the line.

<details><summary>Details</summary>

Example:

```c
#define TPP_STATIC_ASSERT(expr) _Static_assert(expr, #expr)
```
</details>



## TPP_STATIC_ASSERT_MSG

Same as `TPP_STATIC_ASSERT`, but takes a second argument specifying a message to give human-readable hints as to what is going on when the assertion fails.

<details><summary>Details</summary>

Example:

```c
#define TPP_STATIC_ASSERT_MSG(expr, mesg) _Static_assert(expr, mesg)
```
</details>



## tpp_memcpy, tpp_strlen, tpp_strchr, _tpp_strnlen, tpp_strcmp, tpp_memcmp, tpp_memset, tpp_memchr, tpp_memmove

Wrappers around C standard `<string.h>` functions of the same name to-be used by TPP.

Note the leading underscore on `_tpp_strnlen`. You can still provide a definition here, but you can also provide a definition for `tpp_strnlen` directly (see below). `_tpp_strnlen` is merely used by the default implementation of `tpp_strnlen`

<details><summary>Details</summary>

Example:

```c
#include <string.h>

#define tpp_strlen(s)        strlen(s)
#define tpp_strchr(s, c)     strchr(s, c)
#define _tpp_strnlen(s, n)   strnlen(s, n)
#define tpp_strcmp(a, b)     strcmp(a, b)
#define tpp_memcmp(a, b, n)  memcmp(a, b, n)
#define tpp_memcpy(d, s, n)  memcpy(d, s, n)
#define tpp_memset(d, c, n)  memset(d, c, n)
#define tpp_memchr(p, c, n)  memchr(p, c, n)
#define tpp_memmove(d, s, n) memmove(d, s, n)
```
</details>



## tpp_memmem

Optional: define as a compliant implementation of `memmem(3)`, or leave undefined if TPP should supply its own definition internally.

<details><summary>Details</summary>

Example:

```c
#include <string.h>

#define tpp_memmem(h, hs, n, ns) memmem(h, hs, n, ns)
```
</details>



## tpp_memmoveup

Same as `tpp_memmove`, but only called when it is compile-time known that `dst > src` (aka. `FIRST_ARGUMENT > SECOND_ARGUMENT`)

<details><summary>Details</summary>

Example:

```c
#include <string.h>

#define tpp_memmoveup(d, s, n)                       \
	(tpp_assert((char *)(d) >= (char *)(s) || !(n)), \
	 memmove(d, s, n))
```
</details>



## tpp_memmovedown

Same as `tpp_memmove`, but only called when it is compile-time known that `dst < src` (aka. `FIRST_ARGUMENT < SECOND_ARGUMENT`)

<details><summary>Details</summary>

Example:

```c
#include <string.h>

#define tpp_memmovedown(d, s, n)                     \
	(tpp_assert((char *)(d) <= (char *)(s) || !(n)), \
	 memmove(d, s, n))
```
</details>



## tpp_strnlen

Wrapper around `strnlen` (aka. `_tpp_strnlen`). If supported by the host compiler, the default implementation checks if the second argument is compile-time known to be a *very large value*, in which case `tpp_strlen()` is called. Aside from that (optional) aspect, this is merely a wrapper around `strnlen()` as defined by the C standard.

<details><summary>Details</summary>

Example:

```c
#include <string.h>

#ifdef __GNUC__
#define tpp_strnlen(s, n) \
	((__builtin_constant_p(n) && (n) >= TPP_SSIZE_MAX) ? strlen(s) : strnlen(s, n))
#else /* __GNUC__ */
#define tpp_strnlen(s, n) strnlen(s, n)
#endif /* !__GNUC__ */
```
</details>



## tpp_mempcpy

Same as `tpp_memcpy`, but return a pointer *after* the written-to portion, rather than at its start. Some systems provide a function `mempcpy(3)`, which may be supplied here to simplify / stream-line places where TPP uses this function.

<details><summary>Details</summary>

Example:

```c
#include <string.h>

#define tpp_mempcpy(d, s, n) ((void *)((char *)memcpy(d, s, n) + (n)))
```
</details>



## tpp_bzero

Wrapper around `tpp_memset` that only takes 2 arguments (`base` and `num_bytes`), and is equivalent to a call `(void)memset(base, 0, num_bytes)` (note the `void`-cast of the return value: whereas `memset` re-returns `base`, `tpp_bzero` returns `void`). Some systems supply an API-compatible function `bzero(3)` which may be supplied here to simplify / stream-line places where TPP uses this function.

<details><summary>Details</summary>

Example:

```c
#include <string.h>

#define tpp_bzero(p, n) (void)memset(p, 0, n)
```
</details>



## tpp_dbg_memset

Similar to `tpp_bzero` (i.e.: a 2-argument function returning `void`), but instead of filling memory with 0-es, this function fills memory with an easily recognizable bit-pattern, that is unlikely to be valid for most uses (e.g.: when treated as a pointer, the program will likely SEGFAULT). If `TPP_DEBUG` is disabled however, this function becomes a no-op.

TPP uses this function to trash component/object-memory after those components/objects have been finalized, in order to make use-after-free-like invalid API usages easier to spot.

<details><summary>Details</summary>

Example:

```c
#include <string.h>

#if TPP_DEBUG
#define tpp_dbg_memset(p, n) (void)memset(p, 0xcc, n)
#else /* TPP_DEBUG */
#define tpp_dbg_memset(p, n) (void)0
#endif /* !TPP_DEBUG */
```
</details>



## tpp_malloc, tpp_trymalloc, tpp_tryrealloc, tpp_realloc, tpp_free

Wrappers around heap-functions, as defined by the C standard.

The `tpp_try*` functions can usually be defined to alias their non-`try` counterparts. The differences comes into play when the hosting program needs to do special handling when an out-of-memory error happening within TPP for different reasons:

- `tpp_malloc` and `tpp_realloc` are used when out-of-memory should be treated as a **HARD_ERROR**
	- meaning the error will be propagated up the stack and out of TPP-controlled code, usually in the form or a `TPP_ENOMEM` error code
- `tpp_trymalloc` and `tpp_tryrealloc` are used when out-of-memory is a temporary error, aka a **SOFT_ERROR**
	- meaning that TPP will try to handle the out-of-memory condition itself, usually by:
		- re-attempting the allocation with a smaller buffer size (in cases where TPP was trying to over-allocate for the sake of performance),
		- continuing without allocating more memory (in cases where an optional buffer was being allocated, or TPP was trying to enlarge an already non-empty hash-map that can also also operate with a sub-optimally sized table)
		- using a buffer that is larger than it needs to be (in cases where a `tpp_tryrealloc()` fails to truncate a buffer that is larger than it needs to be)

If the hosting program doesn't need to differentiate between these 2 kinds of allocation errors, then it can simply define `tpp_try*` as aliases to their non-`try` counterparts:

```c
#define tpp_trymalloc  tpp_malloc
#define tpp_tryrealloc tpp_realloc
```

<details><summary>Details</summary>

Example:

```c
#include <stdlib.h>

#define tpp_trymalloc(s)     malloc(s)
#define tpp_malloc(s)        malloc(s)
#define tpp_tryrealloc(p, s) realloc(p, s)
#define tpp_realloc(p, s)    realloc(p, s)
#define tpp_free(p)          free(p)
```
</details>



## tpp_alloca

An optional wrapper around GCC's `__builtin_alloca`. If not defined, TPP makes an attempt to define this function itself, but if it fails, TPP will implement its APIs/functions without the use of *dynamic stack memory allocation*.

<details><summary>Details</summary>

Example:

```c
#define tpp_alloca __builtin_alloca
```
</details>



## tpp_assert

Wrapper around `assert`, as defined by `<assert.h>`. Additionally, if `TPP_DEBUG` is defined as `0`, this macro is defined as a no-op (that won't even expand/evaluate its argument)

<details><summary>Details</summary>

Example:

```c
#include <assert.h>

#if TPP_DEBUG
#define tpp_assert       assert
#else /* TPP_DEBUG */
#define tpp_assert(expr) (void)0
#endif /* !TPP_DEBUG */
```
</details>


## TPP_SYSCALL

An optional wrapper to inject additional behavior before/after *system calls* made by TPP. Such *system calls* are any calls TPP does to:

- Open files
- Read from files
- Query file attributes
- Close files (but see `TPP_SYSCALL_NOFAIL`)

TPP calls this macro with 2 arguments:

1. `expr`: The system call expression, including any assignment of its return value to local variables in the surrounding function (meaning that `TPP_SYSCALL` **must** be implemented in the form of a macro)
2. `return_error`: A function-like macro that may be called by the implementation of `TPP_SYSCALL` to perform an error-style early exist from the surrounding function. The implementation of `TPP_SYSCALL` can perform such an early exit by passing some **HARD_ERROR** error as argument, like `return_error(TPP_ENOMEM)` or `return_error(TPP_EIO)`

By default, the implementation of this macro simply dispatches the call to `TPP_SYSCALL_NOFAIL` (see below)

<details><summary>Details</summary>

Example:

```c
#define TPP_SYSCALL(expr, return_error)  \
	do {                                 \
		expr;                            \
		if (WAS_INTERRUPTED)             \
			return_error(TPP_EUSER(42)); \
	} while (0)
```
</details>



## TPP_SYSCALL_NOFAIL

Same as `TPP_SYSCALL`, but only takes a singular argument (`expr`, i.e.: the system call expression). Behavior of this macro is the same as `TPP_SYSCALL`, but it is used instead of that one in cases where the surrounding function has no way to indicating an error.

In general, this is only the case for `tpp_io_close()`, since that function is called during cleanup operations, and TPP (intentionally) doesn't define its cleanup functions as being able to throw errors.

<details><summary>Details</summary>

Example:

```c
#define TPP_SYSCALL_NOFAIL(expr)  \
	do {                          \
		expr;                     \
	} while (0)
```
</details>



## tpp_formatprinter, tpp_formatprinter_print, tpp_formatprinter_of, TPP_FORMATPRINTER_DEFINE

Overrides/hooks to describe how *format-printer* callbacks are:

- typed (`tpp_formatprinter`)
- invoked (`tpp_formatprinter_print`)
- created (`tpp_formatprinter_of`)
- (optionally) forward-declared (`TPP_FORMATPRINTER_DECLARE`)
- defined (`TPP_FORMATPRINTER_DEFINE`, `TPP_FORMATPRINTER_DECL`, `TPP_FORMATPRINTER_IMPL`)

These macros must be defined as follows:

```c
// Typing of format-printers. Can really be anything
typedef ... tpp_formatprinter;

// Wrapper describing how a `tpp_formatprinter` is invoked
tpp_ssize tpp_formatprinter_print(tpp_formatprinter printer, void *arg,
                                  tpp_char const *text, tpp_size num_bytes) {
	...
}

// Wrapper describing how to get the `tpp_formatprinter` which has previously
// been declared under the name `NAME` during the expansion of either:
// >> TPP_FORMATPRINTER_DECL(NAME)
// >> TPP_FORMATPRINTER_IMPL(NAME, ...)
// >> TPP_FORMATPRINTER_DEFINE(NAME, ...)
//
// Must evaluate to an instance of `tpp_formatprinter`
#define tpp_formatprinter_of(NAME)  ...

// Declare/implement a format-printer using `TPP_DECL` / `TPP_IMPL`
// These macros are only needed when `TPP_USE_STATIC` is disabled.
#if !TPP_USE_STATIC
#define TPP_FORMATPRINTER_DECL(NAME) ...
#define TPP_FORMATPRINTER_IMPL(NAME, arg, text, num_bytes) ...
#endif /* !TPP_USE_STATIC */

// Define a format-printer `NAME` with `static` visibility
// The `arg`, `text` and `num_bytes` arguments are simply the names
// of the respective arguments of the function-body implementation
// that follows the definition initiated using this macro
#define TPP_FORMATPRINTER_DEFINE(NAME, arg, text, num_bytes) ...
```

When `TPP_USE_STATIC` is enabled, `TPP_FORMATPRINTER_IMPL` will be unconditionally re-defined as an alias for `TPP_FORMATPRINTER_DEFINE`, and `TPP_FORMATPRINTER_DECL` will be unconditionally undefined.

<details><summary>Details</summary>

Example:

```c
#define tpp_formatprinter tpp_formatprinter
typedef tpp_ssize (TPPCALL *tpp_formatprinter)(void *arg, tpp_char const *text, tpp_size num_bytes);
#define tpp_formatprinter_print(printer, arg, text, num_bytes) \
	((*printer)(arg, text, num_bytes))
#define tpp_formatprinter_of(NAME) (&NAME)
#define TPP_FORMATPRINTER_DEFINE(NAME, arg, text, num_bytes) \
	static tpp_ssize (TPPCALL NAME)(void *arg, tpp_char const *text, tpp_size num_bytes)
#if !TPP_USE_STATIC
#define TPP_FORMATPRINTER_IMPL(NAME, arg, text, num_bytes) \
	TPP_IMPL tpp_ssize (TPPCALL NAME)(void *arg, tpp_char const *text, tpp_size num_bytes)
#define TPP_FORMATPRINTER_DECL(NAME) \
	TPP_DECL tpp_ssize (TPPCALL NAME)(void *arg, tpp_char const *text, tpp_size num_bytes)
#endif /* !TPP_USE_STATIC */
```
</details>



## tpp_formatprinter_print_byname

A convenience wrapper around `tpp_formatprinter_print` and `tpp_formatprinter_of`:

```c
#define tpp_formatprinter_print_byname(NAME, arg, text, num_bytes) \
	tpp_formatprinter_print(tpp_formatprinter_of(NAME), arg, text, num_bytes)
```

<details><summary>Details</summary>

Example:

```c
#define tpp_formatprinter_print_byname(NAME, arg, text, num_bytes) \
	NAME(arg, text, num_bytes)
```
</details>



## tpp_formatprinter_print_cstr

A wrapper around `tpp_formatprinter_print` that takes `char const *` for its `text` argument, rather than `tpp_char const *`

<details><summary>Details</summary>

Example:

```c
#define tpp_formatprinter_print_cstr(printer, arg, text, num_bytes) \
	tpp_formatprinter_print(printer, arg, (tpp_char const *)(text), num_bytes)
```
</details>



## tpp_formatprinter_print_conststr

A wrapper around `tpp_formatprinter_print_cstr` that takes `char const[?]` for its `CONSTstr` argument, and automatically determines the length of that constant string, such that constant strings can easily be printed without the need for manually determining its length

<details><summary>Details</summary>

Example:

```c
#define tpp_formatprinter_print_conststr(printer, arg, CONSTstr) \
	tpp_formatprinter_print_cstr(printer, arg, CONSTstr, sizeof(CONSTstr) - sizeof(char))
```
</details>



## tpp_lcinfo

Typing and API surrounding low-level line/column information. When `tpp_lcinfo` is pre-defined as a macro, you must supply your own implementation of the following API:

```c
typedef ... tpp_lcinfo;

tpp_line tpp_lcinfo_getline(tpp_lcinfo self);
tpp_column tpp_lcinfo_getcol(tpp_lcinfo self);
tpp_lcinfo tpp_lcinfo_of(tpp_line line, tpp_column col);
void tpp_lcinfo_init(tpp_lcinfo *p_self, tpp_line line, tpp_column col);  // OPTIONAL
void tpp_lcinfo_setline(tpp_lcinfo *p_self, tpp_line line);               // OPTIONAL
void tpp_lcinfo_setcol(tpp_lcinfo *p_self, tpp_column col);               // OPTIONAL
bool tpp_lcinfo_equals(tpp_lcinfo a, tpp_lcinfo b);                       // OPTIONAL
tpp_lcinfo const TPP_LCINFO_INVALID = ...;                                // OPTIONAL
bool tpp_lcinfo_isvalid(tpp_lcinfo self);                                 // OPTIONAL
void tpp_lcinfo_init_invalid(tpp_lcinfo *p_self);                         // OPTIONAL
```

All functions marked as `OPTIONAL` don't necessarily *have* to be defined by you. If you didn't define them, TPP will substitute them with default-implementations that use the other functions that you *did* define.

If you do define any of these functions, you must either do so as macros, or must also define them as macros that expand to themselves: `#define tpp_lcinfo_equals tpp_lcinfo_equals`

<details><summary>Details</summary>

Example:

```c
#define tpp_lcinfo tpp_lcinfo
typedef struct tpp_lcinfo {
	tpp_line   lci_line; /* Line */
	tpp_column lci_col;  /* Column */
} tpp_lcinfo;

#define tpp_lcinfo_getline(self) ((tpp_line)(self).lci_line)
#define tpp_lcinfo_getcol(self)  ((tpp_column)(self).lci_col)
#define tpp_lcinfo_init(self, line, col) \
	(void)((self)->lci_line = (line),    \
	       (self)->lci_col  = (col))
#define tpp_lcinfo_setline(p_self, line) (void)((p_self)->lci_line = (line))
#define tpp_lcinfo_setcol(p_self, col)   (void)((p_self)->lci_col = (col))
#define tpp_lcinfo_equals(a, b)                        \
	(tpp_lcinfo_getline(a) == tpp_lcinfo_getline(b) && \
	 tpp_lcinfo_getcol(a) == tpp_lcinfo_getcol(b))

tpp_lcinfo tpp_lcinfo_of(tpp_line line, tpp_column col) {
	tpp_lcinfo result;
	result.lci_line = line;
	result.lci_col  = col;
	return result;
}

#define TPP_LCINFO_INVALID              tpp_lcinfo_of(-1, -1)
#define tpp_lcinfo_isvalid(self)        (tpp_lcinfo_getcol(self) >= 0)
#define tpp_lcinfo_init_invalid(p_self) tpp_lcinfo_init(p_self, -1, -1)
```
</details>



## tpp_refcnt

API used to implement reference counters (that don't need to be thread-safe)

```c
typedef ... tpp_refcnt;
tpp_refcnt TPP_REFCNT_INIT(<unspecified_unsigned_int> initial_value);
void tpp_refcnt_init(tpp_refcnt *self, <unspecified_unsigned_int> initial_value);
void tpp_refcnt_inc(tpp_refcnt *self);

// Decrement and return indicative of `0` having been reached
bool tpp_refcnt_decfetch(tpp_refcnt *self);

// Return indicative of the reference count being `>= 2`
bool tpp_refcnt_isshared(tpp_refcnt *self);

void tpp_refcnt_dec(tpp_refcnt *self);  // OPTIONAL
```

All functions marked as `OPTIONAL` don't necessarily *have* to be defined by you. If you didn't define them, TPP will substitute them with default-implementations that use the other functions that you *did* define.

<details><summary>Details</summary>

Example:

```c
typedef struct {
	tpp_uint_fast32 trc_count; /* Reference counter */
} tpp_refcnt;
#define tpp_refcnt             tpp_refcnt
#define TPP_REFCNT_INIT(v)     { v }
#define tpp_refcnt_init(p, v)  (void)((p)->trc_count = (v))
#define tpp_refcnt_inc(p)      (void)(++(p)->trc_count)
#define tpp_refcnt_decfetch(p) (--(p)->trc_count)
#define tpp_refcnt_isshared(p) ((p)->trc_count > 1)
#define tpp_refcnt_dec(p)      (void)tpp_refcnt_decfetch(p)
```
</details>



## tpp_refcnt_atomic

Same as `tpp_refcnt`, but these reference counters are used when multiple threads *may* access the same shared, global object, even when conventions are followed and every thread has its own, private `tpp_lexer`.

```c
typedef ... tpp_atomic_refcnt;
tpp_refcnt TPP_ATOMIC_REFCNT_INIT(<unspecified_unsigned_int> initial_value);
void tpp_atomic_refcnt_init(tpp_refcnt *self, <unspecified_unsigned_int> initial_value);
void tpp_atomic_refcnt_inc(tpp_refcnt *self);

// Decrement and return indicative of `0` having been reached
bool tpp_atomic_refcnt_decfetch(tpp_refcnt *self);

// Return indicative of the reference count being `>= 2`
bool tpp_atomic_refcnt_isshared(tpp_refcnt *self);

void tpp_atomic_refcnt_dec(tpp_refcnt *self);  // OPTIONAL
```

All functions marked as `OPTIONAL` don't necessarily *have* to be defined by you. If you didn't define them, TPP will substitute them with default-implementations that use the other functions that you *did* define.

<details><summary>Details</summary>

Example:

```c
typedef struct {
	tpp_uint_fast32 trca_count; /* Reference counter */
} tpp_refcnt_atomic;
#define tpp_refcnt_atomic             tpp_refcnt_atomic
#define TPP_REFCNT_ATOMIC_INIT(v)     { v }
#define tpp_refcnt_atomic_init(p, v)  (void)((p)->trca_count = (v))
#define tpp_refcnt_atomic_inc(p)      (void)(++(p)->trca_count)
#define tpp_refcnt_atomic_decfetch(p) (--(p)->trca_count)
#define tpp_refcnt_atomic_isshared(p) ((p)->trca_count > 1)
#define tpp_refcnt_atomic_dec(p)      (void)tpp_refcnt_atomic_decfetch(p)
```
</details>



## tpp_once

Ensure that a given code-block is executed only once (in a way that thread-safe, such that with multiple parallel threads, only 1 will execute the block, and all other will wait for the first to finish)

```c
#define tpp_once(expr) ...
```

<details><summary>Details</summary>

Example:

```c
#define tpp_once(expr)             \
	do {                           \
		static int _to_didrun = 0; \
		if (!_to_didrun) {         \
			_to_didrun = 1;        \
			expr;                  \
		}                          \
	} while (0)
```
</details>



## tpp_ascii_issymstrt, tpp_ascii_issymcont, tpp_ascii_isdigit, tpp_ascii_isspace, tpp_ascii_islf

Used to implement low-level ASCII-based character trait tests.

```c
bool tpp_ascii_issymstrt(tpp_char ch); // a-z, A-Z, $, _
bool tpp_ascii_issymcont(tpp_char ch); // a-z, A-Z, $, _, 0-9
bool tpp_ascii_isdigit(tpp_char ch);   // 0-9
bool tpp_ascii_isspace(tpp_char ch);   // Space characters...
bool tpp_ascii_islf(tpp_char ch);      // \r and \n

// Same as `tpp_ascii_islf(ch) || ch == 0xc2 || ch == 0xe2`
// iow: is `ch` a line-feed, or the first byte of a utf-8
//      encoded unicode linefeed (see `tpp_unicode_islf`)
bool tpp_ascii_islf_or_mblf(tpp_char ch);  // OPTIONAL

// Same as `tpp_ascii_isspace(ch) && !tpp_ascii_islf(ch)`
bool tpp_ascii_isspace_nolf(tpp_char ch);  // OPTIONAL
```

<details><summary>Details</summary>

Example:

```c
#define tpp_ascii_issymstrt(ch)    (LOOKUP_TABLE[ch] & ISSYMSTRT)
#define tpp_ascii_issymcont(ch)    (LOOKUP_TABLE[ch] & ISSYMCONT)
#define tpp_ascii_isdigit(ch)      (LOOKUP_TABLE[ch] & ISDIGIT)
#define tpp_ascii_isspace(ch)      (LOOKUP_TABLE[ch] & (ISSPACE | ISLF))
#define tpp_ascii_islf(ch)         (LOOKUP_TABLE[ch] & ISLF)
#define tpp_ascii_islf_or_mblf(ch) (LOOKUP_TABLE[ch] & (ISLF | ISMBLF))
#define tpp_ascii_isspace_nolf(ch) (LOOKUP_TABLE[ch] & ISSPACE)
```
</details>



## tpp_ascii_islf_or_mblf

Convenience wrapper for `tpp_ascii_islf(ch) || ch == 0xc2 || ch == 0xe2` (iow: checks if `ch` a line-feed, or the first byte of a utf-8 encoded unicode linefeed (see `tpp_unicode_islf`)). If the implementations of `tpp_ascii_islf` use a lookup table, it may also be possible to express this combinatory check using a single table lookup.

<details><summary>Details</summary>

Example:

```c
#define tpp_ascii_islf_or_mblf(ch) (tpp_ascii_islf(ch) || (ch) == 0xc2 || (ch) == 0xe2)
```
</details>



## tpp_ascii_isspace_nolf

Convenience wrapper for `tpp_ascii_isspace(ch) && !tpp_ascii_islf(ch)`. If the implementations of `tpp_ascii_isspace` and `tpp_ascii_islf` use a lookup table, it may also be possible to express this combinatory check using a single table lookup.

<details><summary>Details</summary>

Example:

```c
#define tpp_ascii_isspace_nolf(ch) (tpp_ascii_isspace(ch) && !tpp_ascii_islf(ch))
```
</details>



## TPP_HAVE_ASSUME_ASCII_CTYPE

Defined to either `0` or `1` to specify if TPP is allowed to assume that the implementation of `tpp_ascii_is*` functions follows ASCII conventions. When that is the case, TPP may omit calls to (e.g.) `tpp_ascii_islf()` in large switch cases, and instead use `case 10: case 13:`.



## tpp_ascii_ismb

A function-like macro that is used to determine if a given character is part of a MultiByte utf-8 sequence. This macro may be defined in one of 2 ways, and essentially just determines how TPP treats stray bytes in the range `[0x80,0xbf]`:

```c
/* Treat `[0x80,0xbf]` as stray, illegal utf-8 (default) */
#define tpp_ascii_ismb(ch) ((ch) >= TPP_UINT_LEAST8_C(0x80))

/* Treat `[0x80,0xbf]` as extended-ASCII, thus reducing
 * the chances of utf-8 encoding errors being detected. */
#define tpp_ascii_ismb(ch) ((ch) >= TPP_UINT_LEAST8_C(0xc0))
```



## tpp_ascii_asdigit, tpp_ascii_ofdigit

Convert characters `0123456789` to/from their decimal integer value:

```c
int tpp_ascii_asdigit(tpp_char ch);
tpp_char tpp_ascii_ofdigit(int v);
```

<details><summary>Details</summary>

Example:

```c
#define tpp_ascii_asdigit(ch) ((ch) - '0')
#define tpp_ascii_ofdigit(v)  ('0' + (v))
```
</details>



## tpp_ascii_isoctdigit, tpp_ascii_asoctdigit, tpp_ascii_ofoctdigit

Convert characters `01234567` to/from their octal integer value:

```c
bool tpp_ascii_isoctdigit(tpp_char ch);
int tpp_ascii_asoctdigit(tpp_char ch);
tpp_char tpp_ascii_ofoctdigit(int v);
```

<details><summary>Details</summary>

Example:

```c
#define tpp_ascii_isoctdigit(ch) ((ch) >= '0' && (ch) <= '7')
#define tpp_ascii_asoctdigit(ch) tpp_ascii_asdigit(ch)
#define tpp_ascii_ofoctdigit(v)  tpp_ascii_ofdigit(v)
```
</details>



## tpp_ascii_islwrxdigit, tpp_ascii_aslwrxdigit, tpp_ascii_oflwrxdigit

Convert characters `abcdef` to/from their hexadecimal integer value:

```c
bool tpp_ascii_islwrxdigit(tpp_char ch);
int tpp_ascii_aslwrxdigit(tpp_char ch);
tpp_char tpp_ascii_oflwrxdigit(int v);
```

<details><summary>Details</summary>

Example:

```c
#define tpp_ascii_islwrxdigit(ch) ((ch) >= 'a' && (ch) <= 'f')
#define tpp_ascii_aslwrxdigit(ch) (10 + ((ch) - 'a'))
#define tpp_ascii_oflwrxdigit(v)  ('a' + ((v) - 10))
```
</details>



## tpp_ascii_isuprxdigit, tpp_ascii_asuprxdigit, tpp_ascii_ofuprxdigit

Convert characters `ABCDEF` to/from their hexadecimal integer value:

```c
bool tpp_ascii_isuprxdigit(tpp_char ch);
int tpp_ascii_asuprxdigit(tpp_char ch);
tpp_char tpp_ascii_ofuprxdigit(int v);
```

<details><summary>Details</summary>

Example:

```c
#define tpp_ascii_isuprxdigit(ch) ((ch) >= 'A' && (ch) <= 'F')
#define tpp_ascii_asuprxdigit(ch) (10 + ((ch) - 'A'))
#define tpp_ascii_ofuprxdigit(v)  ('A' + ((v) - 10))
```
</details>



## tpp_ascii_isxdigit, tpp_ascii_asxdigit

Convert characters `0123456789abcdefABCDEF` to their hexadecimal integer value:

```c
bool tpp_ascii_isxdigit(tpp_char ch);
int tpp_ascii_asxdigit(tpp_char ch);
```

<details><summary>Details</summary>

Example:

```c
#define tpp_ascii_isxdigit(ch) \
	(tpp_ascii_isdigit(ch) || tpp_ascii_islwrxdigit(ch) || tpp_ascii_isuprxdigit(ch))
#define tpp_ascii_asxdigit(ch) \
	(tpp_ascii_isdigit(ch) ? tpp_ascii_asdigit(ch) : tpp_ascii_islwrxdigit(ch) ? tpp_ascii_aslwrxdigit(ch) : tpp_ascii_asuprxdigit(ch))
```
</details>



## tpp_ascii_tolwrxdigit

Convert hexadecimal integer value to characters `0123456789abcdef`:

```c
tpp_char tpp_ascii_tolwrxdigit(int v);
```

<details><summary>Details</summary>

Example:

```c
#define tpp_ascii_tolwrxdigit(v) ((v) < 10 ? tpp_ascii_ofdigit(v) : tpp_ascii_oflwrxdigit(v))
```
</details>



## tpp_ascii_touprxdigit

Convert hexadecimal integer value to characters `0123456789ABCDEF`:

```c
tpp_char tpp_ascii_touprxdigit(int v);
```

<details><summary>Details</summary>

Example:

```c
#define tpp_ascii_touprxdigit(v) ((v) < 10 ? tpp_ascii_ofdigit(v) : tpp_ascii_ofuprxdigit(v))
```
</details>



## tpp_unicode_issymstrt, tpp_unicode_issymcont, tpp_unicode_isspace, tpp_unicode_islf

Used to implement low-level unicode-based character trait tests.

```c
bool tpp_unicode_isspace_nolf(tpp_unichar ord);
bool tpp_unicode_issymstrt(tpp_unichar ord);
bool tpp_unicode_issymcont(tpp_unichar ord);
bool tpp_unicode_isspace(tpp_unichar ord);
bool tpp_unicode_islf(tpp_unichar ord);

// Same as `tpp_unicode_isspace(ch) && !tpp_unicode_islf(ch)`
bool tpp_unicode_isspace_nolf(tpp_char ch);  // OPTIONAL
```

<details><summary>Details</summary>

Example:

```c
#define tpp_unicode_issymstrt(ord)    (GET_UNICODE_FLAGS(ord) & ISSYMSTRT)
#define tpp_unicode_issymcont(ord)    (GET_UNICODE_FLAGS(ord) & ISSYMCONT)
#define tpp_unicode_isdigit(ord)      (GET_UNICODE_FLAGS(ord) & ISDIGIT)
#define tpp_unicode_isspace(ord)      (GET_UNICODE_FLAGS(ord) & (ISSPACE | ISLF))
#define tpp_unicode_islf(ord)         (GET_UNICODE_FLAGS(ord) & ISLF)
#define tpp_unicode_isspace_nolf(ord) (GET_UNICODE_FLAGS(ord) & ISSPACE)
```
</details>



## tpp_unicode_isspace_nolf

Convenience wrapper for `tpp_unicode_isspace(ord) && !tpp_unicode_islf(ord)`. If the implementations of `tpp_unicode_isspace` and `tpp_unicode_islf` use a lookup table, it may also be possible to express this combinatory check using a single table lookup.

<details><summary>Details</summary>

Example:

```c
#define tpp_unicode_isspace_nolf(ord) (tpp_unicode_isspace(ord) && !tpp_unicode_islf(ord))
```
</details>



## tpp_unicode_utf8seqlen_mb_getcur

Function to determine the length (in bytes, including the first byte) of a utf-8 sequence. The given argument is always in range `[0x80,0xff]` (iow: this function is never called for ASCII characters).

If the given byte isn't a valid utf-8-sequence start byte (i.e. lies in range `[0x80,0xbf]`, or would indicate a utf-8 sequence length that doesn't contain any valid UTF-32 character), this function must return `0`.

```c
tpp_uint_least8 tpp_unicode_utf8seqlen_mb_getcur(tpp_char first_utf8_byte);
```


<details><summary>Details</summary>

Example:

```c
#define tpp_unicode_utf8seqlen_mb_getcur(first_utf8_byte) \
	VALID_UTF8_LENGTH_TABLE[first_utf8_byte]
```
</details>



## tpp_unicode_utf8seqlen_getcur

Same as `tpp_unicode_utf8seqlen_mb_getcur`, but may also be called when `first_utf8_byte` is in range `[0x00,0x7f]` (iow: must be able to handle the full `[0x00,0xff]` range of byte values).

If the given byte isn't a valid utf-8-sequence start byte (i.e. lies in range `[0x80,0xbf]`, or would indicate a utf-8 sequence length that doesn't contain any valid UTF-32 character), this function must return `0`.

```c
tpp_uint_least8 tpp_unicode_utf8seqlen_getcur(tpp_char first_utf8_byte);
```

<details><summary>Details</summary>

Example:

```c
#define tpp_unicode_utf8seqlen_getcur(first_utf8_byte) \
	VALID_UTF8_LENGTH_TABLE[first_utf8_byte]
```
</details>



## tpp_unicode_utf8seqlen_mb_getmax

Similar to `tpp_unicode_utf8seqlen_mb_getcur()`, but must never return `0` (instead: must return `1`). Additionally, this function mustn't reject UTF-8 sequences whose length doesn't contain any valid UTF-32 character (this function is used to decode the entire UCS-32 spectrum).

```c
tpp_uint_least8 tpp_unicode_utf8seqlen_mb_getmax(tpp_char first_utf8_byte);
```

<details><summary>Details</summary>

Example:

```c
#define tpp_unicode_utf8seqlen_mb_getmax(first_utf8_byte) \
	COMPLETE_UTF8_LENGTH_TABLE[first_utf8_byte]
```
</details>



## tpp_unicode_utf8seqlen_getmax

Same as `tpp_unicode_utf8seqlen_mb_getmax`, but may also be called when `first_utf8_byte` is in range `[0x00,0x7f]` (iow: must be able to handle the full `[0x00,0xff]` range of byte values).

```c
tpp_uint_least8 tpp_unicode_utf8seqlen_getmax(tpp_char first_utf8_byte);
```

<details><summary>Details</summary>

Example:

```c
#define tpp_unicode_utf8seqlen_getmax(first_utf8_byte) \
	COMPLETE_UTF8_LENGTH_TABLE[first_utf8_byte]
```
</details>



## tpp_ascii_isutf8cont

Used to check if a given `ch` is a valid utf-8 continuation byte.

```c
bool tpp_ascii_isutf8cont(tpp_char ch);
```

<details><summary>Details</summary>

Example:

```c
#define tpp_ascii_isutf8cont(ch) (((ch) & 0xc0) == 0x80)
```
</details>



## tpp_fuzzy_memcmp

Implementation of the *fuzzy* string comparison function that is enabled by `TPP_HAVE_TPP_FUZZY_MEMCMP`.

```c
tpp_size tpp_fuzzy_memcmp(tpp_char const *lhs, tpp_size lhs_len,
                          tpp_char const *rhs, tpp_size rhs_len);
```

<details><summary>Details</summary>

Example:

```c
#define _KOS_SOURCE 1
#include <string.h>

#define tpp_fuzzy_memcmp(lhs, lhs_len, rhs, rhs_len) \
	fuzzy_memcmp(lhs, lhs_len, rhs, rhs_len)
```
</details>



## tpp_xml_entity_lookup

Implementation of XML entity lookup that is enabled by `TPP_HAVE_XML_ENTITY_LOOKUP`.

When `TPP_HAVE_XML_ENTITY_PRINTNEAREST`, then an additional function `tpp_xml_entity_printnearest()` must also be supplied.

```c
/* Return the unicode ordinal associated with `name`
 * @return: TPP_XML_ENTITY_LOOKUP_UNKNOWN: Unknown entity name
 * @return: * : Unicode ordinal for specified entity */
tpp_unichar tpp_xml_entity_lookup(char const *name, bool has_trailing_semicolon);

/* Returned by `tpp_xml_entity_lookup()` when entity name is unrecognized */
#define TPP_XML_ENTITY_LOOKUP_UNKNOWN 0

#define TPP_XML_ENTITY_LOOKUP_MAXLEN 31 /* Length of the longest, known XML entity */
#define TPP_XML_ENTITY_LOOKUP_MINLEN 2  /* Length of the shortest, known XML entity */


#if TPP_HAVE_XML_ENTITY_PRINTNEAREST
/* Print the name (including a trailing `;` if there is one) of
 * some XML entity that matches the given `name` most closely.
 *
 * @return: * : Sum of return values of `printer`
 * @return: <0: First negative return value of `printer` */
tpp_ssize tpp_xml_entity_printnearest(char const *name,
                                      bool has_trailing_semicolon,
                                      tpp_formatprinter printer, void *arg);
#endif /* TPP_HAVE_XML_ENTITY_PRINTNEAREST */
```



## tpp_unicode_byname_lookup



## tpp_xml_entity_lookup

Implementation of unicode by-name lookup that is enabled by `TPP_HAVE_UNICODE_BYNAME_LOOKUP`.

When `TPP_HAVE_UNICODE_BYNAME_PRINTNEAREST`, then an additional function `tpp_unicode_byname_printnearest()` must also be supplied.

```c
/* Max # of unicode characters generated by `tpp_unicode_byname_lookup()` */
#define TPP_UNICODE_BYNAME_LOOKUP_MAXUC 10


/* Return the unicode ordinal associated with `*p_iter`
 * @return: 0 : Unknown (`*p_iter` was left unchanged)
 * @return: * : # of unicode ordinals written to `result` */
tpp_size tpp_unicode_byname_lookup(tpp_char const **p_iter, tpp_char const *end,
                                   tpp_unichar uc[TPP_UNICODE_BYNAME_LOOKUP_MAXUC],
                                   tpp_lexer const *lexer);


#if TPP_HAVE_UNICODE_BYNAME_PRINTNEAREST
/* Print the name of some unicode character name that matches the
 * given `name` most closely.
 *
 * @return: * : Sum of return values of `printer`
 * @return: <0: First negative return value of `printer` */
tpp_size tpp_unicode_byname_printnearest(tpp_char const *start, tpp_char const *end,
                                         tpp_formatprinter printer, void *arg,
                                         tpp_lexer const *lexer);
#endif /*  TPP_HAVE_UNICODE_BYNAME_PRINTNEAREST*/
```



## tpp_intvalue

In order to allow you to hook an arbitrary-precision integer types into TPP, the following API may be overwritten by the user, and will be used by TPP to implement functions like `tpp_lexer_decodeint()`. This API is also used by the default implementation of `tpp_expr_value` (see below), so any overrides defined here will also be used by that API.

NOTE: All functions returning `tpp_errno` are expected to return `TPP_EOK` on success, and `TPP_ISERR(*)` to indicate **HARD_ERROR**. Some functions are also allowed to return `TPP_ENOENT` to indicate overflow or some other function-specific value. When that is the case, such behavior is documented with the function below.

```c
#define tpp_intvalue my_tpp_intvalue
typedef ... my_tpp_intvalue;

void tpp_intvalue_fini(tpp_intvalue *self); // Finalizer
tpp_errno tpp_intvalue_init_zero(tpp_intvalue *self); // Initializer
tpp_errno tpp_intvalue_init_copy(tpp_intvalue *dst, tpp_intvalue const *src); // Copy-constructor


/* @return: TPP_EOK:      Success
 * @return: TPP_ENOENT:   SOFT_ERROR: Out-of-range (only if `TPP_INTVALUE_ASINTMAX_CANOVERFLOW`)
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_asintmax(tpp_intvalue *self, tpp_intmax *p_result);


#if TPP_HAVE_LEXER_DECODEINT
/* >> [self] = ([self] * mul) + add;
 * Used to implement `tpp_lexer_decodeint()`
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Overflow (only if `TPP_INTVALUE_MATH_CANOVERFLOW`)
 * @return: TPP_ISERR(*): HARD_ERROR */
#if TPP_INTVALUE_MATH_CANOVERFLOW
tpp_errno tpp_intvalue_muladd(tpp_intvalue *self, unsigned int mul, unsigned int add);
#endif /* TPP_HAVE_LEXER_DECODEINT */


#if TPP_HAVE_LEXER_PARSECHARACTER_EXPR
/* >> [self] = v;
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Overflow (only if `TPP_INTVALUE_MATH_CANOVERFLOW`)
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_init_uintmax(tpp_intvalue *self, tpp_uintmax v);
#endif /* TPP_HAVE_LEXER_PARSECHARACTER_EXPR */



/************************************************************************/
/* Extra APIs needed used by the builtin impl of `tpp_expr_value`       */
/************************************************************************/
#if TPP_HAVE_BUILTIN_EXPR_VALUE

tpp_errno tpp_intvalue_init_zero(tpp_intvalue *self); // Initialize to value `1`
tpp_errno tpp_intvalue_init_bool(tpp_intvalue *self, bool v); // Initialize to value `0` / `1`

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
/* >> [self] = v;
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Overflow (only if `TPP_INTVALUE_MATH_CANOVERFLOW`)
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_init_size(tpp_intvalue *self, tpp_size v);

/* >> [self] = v;
 * @return: TPP_EOK:      OK
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_init_char(tpp_intvalue *self, tpp_char v);
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

/* @return: TPP_EOK:      Non-zero
 * @return: TPP_ENOENT:   Zero
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_asbool(tpp_intvalue *self);

/* @return: TPP_EOK:      Yes
 * @return: TPP_ENOENT:   No
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_isneg(tpp_intvalue *self);


/* >> [p_result] = -[self];
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Overflow (only if `TPP_INTVALUE_MATH_CANOVERFLOW`)
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_neg(tpp_intvalue *self, tpp_intvalue *p_result);

/* >> [p_result] = ~[self];
 * @return: TPP_EOK:      OK
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_inv(tpp_intvalue *self, tpp_intvalue *p_result);

/* >> [p_result] = [lhs] + [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Overflow (only if `TPP_INTVALUE_MATH_CANOVERFLOW`)
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_add(tpp_intvalue *lhs, tpp_intvalue *rhs, tpp_intvalue *p_result);

/* >> [p_result] = [lhs] - [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Overflow (only if `TPP_INTVALUE_MATH_CANOVERFLOW`)
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_sub(tpp_intvalue *lhs, tpp_intvalue *rhs, tpp_intvalue *p_result);

/* >> [p_result] = [lhs] * [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Overflow (only if `TPP_INTVALUE_MATH_CANOVERFLOW`)
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_mul(tpp_intvalue *lhs, tpp_intvalue *rhs, tpp_intvalue *p_result);

/* >> [p_result] = [lhs] / [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Divide-by-zero
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_div(tpp_intvalue *lhs, tpp_intvalue *rhs, tpp_intvalue *p_result);

/* >> [p_result] = [lhs] % [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Divide-by-zero
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_mod(tpp_intvalue *lhs, tpp_intvalue *rhs, tpp_intvalue *p_result);

/* >> [p_result] = [lhs] << [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Overflow (only if `TPP_INTVALUE_MATH_CANOVERFLOW`)
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_shl(tpp_intvalue *lhs, tpp_intvalue *rhs, tpp_intvalue *p_result);

/* >> [p_result] = [lhs] >> [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Overflow (only if `TPP_INTVALUE_MATH_CANOVERFLOW`)
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_shr(tpp_intvalue *lhs, tpp_intvalue *rhs, tpp_intvalue *p_result);

/* >> [p_result] = [lhs] & [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_and(tpp_intvalue *lhs, tpp_intvalue *rhs, tpp_intvalue *p_result);

/* >> [p_result] = [lhs] ^ [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_xor(tpp_intvalue *lhs, tpp_intvalue *rhs, tpp_intvalue *p_result);

/* >> [p_result] = [lhs] | [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_or(tpp_intvalue *lhs, tpp_intvalue *rhs, tpp_intvalue *p_result);

/* Store `< 0`, `== 0` or `> 0` to `*p_delta`, based on result of `lhs <=> rhs`
 * @return: TPP_EOK:      OK
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_intvalue_cmp(tpp_intvalue *lhs, tpp_intvalue *rhs, int *p_delta);

/* Print the representation of `self` to `printer` (in target encoding; used to implement `__TPP_EVAL`)
 * @return: *  : Sum of positive return value of `printer`
 * @return: < 0: An error was thrown (`TPP_SSIZE_ISERR`), or `printer` returned this value */
#if TPP_HAVE_EXPR_VALUE_PRINTREPR
tpp_ssize tpp_intvalue_printrepr(tpp_lexer *lexer, tpp_intvalue *self, tpp_formatprinter printer, void *arg);
#endif /* TPP_HAVE_EXPR_VALUE_PRINTREPR */
#endif /* TPP_HAVE_BUILTIN_EXPR_VALUE */
```



## tpp_expr_value

In order to perform operations in builtin lexer expressions, a large, overwritable API of functions to perform those operations on abstract `tpp_expr_value` objects is used, and can be overwritten by the host compiler in order to better integrate its own expression system into TPP.

NOTE: All functions returning `tpp_errno` are expected to return `TPP_EOK` on success, and `TPP_ISERR(*)` to indicate **HARD_ERROR**. Some functions are also allowed to return `TPP_ENOENT` to indicate an additional success-style return value, but those are explicitly documented below.

```c
#define tpp_intvalue my_tpp_expr_value
typedef ... my_tpp_expr_value; // Opaque

void tpp_expr_value_fini(tpp_expr_value *self); // Finalizer
void tpp_expr_value_move(tpp_expr_value *dst, tpp_expr_value *src); // Move-constructor
tpp_errno tpp_expr_value_copy(tpp_expr_value *dst, tpp_expr_value const *src); // Copy-constructor

/************************************************************************/
/* Check which native representation is used by `self`                  */
/************************************************************************/
bool tpp_expr_value_isint(tpp_expr_value const *self);
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
bool tpp_expr_value_isfloat(tpp_expr_value const *self);
#endif // TPP_HAVE_BUILTIN_EXPR_FLOATS
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
bool tpp_expr_value_isstring(tpp_expr_value const *self);
#endif // TPP_HAVE_BUILTIN_EXPR_STRINGS


/************************************************************************/
/* Extract values from `self` (these are allowed to assume that `tpp_expr_value_is*(self)`) */
/************************************************************************/
// TPP_ENOENT: Stored value is too large to fit into `tpp_intmax`
tpp_errno tpp_expr_value_asintmax(tpp_expr_value const *self, tpp_intmax *p_result);
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
tpp_errno tpp_expr_value_asfloat(tpp_expr_value const *self, tpp_float *p_result);
#endif // TPP_HAVE_BUILTIN_EXPR_FLOATS
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
tpp_errno tpp_expr_value_asstringref(tpp_expr_value const *self, TPP_REF tpp_string **p_result);
#endif // TPP_HAVE_BUILTIN_EXPR_STRINGS


/************************************************************************/
/* Initialize `self`                                                    */
/************************************************************************/
tpp_errno tpp_expr_value_init_bool(tpp_expr_value *self, bool v); // Init as integer 0/1
tpp_errno tpp_expr_value_init_zero(tpp_expr_value *self); // Init as integer 0
tpp_errno tpp_expr_value_init_one(tpp_expr_value *self); // Init as integer 1

tpp_errno tpp_expr_value_init_expr_intvalue(tpp_expr_value *self, /*inherit(always)*/ tpp_intvalue *v);
#if TPP_HAVE_LEXER_PARSECHARACTER_EXPR
/* @return: TPP_ENOENT: Given value `v` is too large. */
tpp_errno tpp_expr_value_init_uintmax(tpp_expr_value *self, tpp_uintmax v);
#endif /* TPP_HAVE_LEXER_PARSECHARACTER_EXPR */

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
tpp_errno tpp_expr_value_init_float(tpp_expr_value *self, tpp_float v);
#endif // TPP_HAVE_BUILTIN_EXPR_FLOATS
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
tpp_errno tpp_expr_value_init_string(tpp_expr_value *self, tpp_string *v);
tpp_errno tpp_expr_value_init_string_inherited(tpp_expr_value *self, /*inherit(always)*/ TPP_REF tpp_string *v);
#endif // TPP_HAVE_BUILTIN_EXPR_STRINGS


/* Operator invocation
 * NOTE: Integer overflow (if it *can* happen) *must* be handled by *within* these
 *       functions, preferably by triggering a `TPP_W_INTEGER_OVERFLOW` warning.
 * iow: unlike the `tpp_intvalue_*` API (see above), these _DONT_ return `TPP_ENOENT` */
tpp_errno tpp_expr_value_pos(tpp_lexer *lexer, /*in*/ tpp_expr_value *self, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_neg(tpp_lexer *lexer, /*in*/ tpp_expr_value *self, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_inv(tpp_lexer *lexer, /*in*/ tpp_expr_value *self, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_add(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *rhs, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_sub(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *rhs, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_mul(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *rhs, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_div(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *rhs, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_mod(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *rhs, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_shl(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *rhs, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_shr(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *rhs, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_and(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *rhs, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_xor(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *rhs, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_or(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *rhs, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_cmp(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *rhs, /*out*/ int *p_delta);
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
tpp_errno tpp_expr_value_lengthof(tpp_lexer *lexer, /*in*/ tpp_expr_value *self, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_getindex(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *index, /*out*/ tpp_expr_value *result);
tpp_errno tpp_expr_value_getrange(tpp_lexer *lexer, /*in*/ tpp_expr_value *lhs, /*in*/ tpp_expr_value *lo, /*in*/ tpp_expr_value *hi, /*out*/ tpp_expr_value *result);
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */


/* Determine the boolean-style value of `self`
 * Works for any kind of expression value.
 * @return: TPP_EOK:      Is non-zero
 * @return: TPP_ENOENT:   Is zero
 * @return: TPP_ISERR(*): HARD_ERROR */
tpp_errno tpp_expr_value_asbool(tpp_lexer *lexer, /*in*/ tpp_expr_value *self);


#if TPP_HAVE_EXPR_VALUE_PRINTREPR
tpp_ssize tpp_expr_value_printrepr(tpp_lexer *lexer, tpp_expr_value *self,
                                   tpp_formatprinter printer, void *arg);
#endif /* TPP_HAVE_EXPR_VALUE_PRINTREPR */
```



## XXX: tpp_lexer_parsestring_expr

## XXX: TPP_FS_HAVE_DRIVES

## XXX: TPP_FS_HAVE_ICASE

## XXX: TPP_FS_SEP, TPP_FS_SEP_S

## XXX: TPP_FS_DELIM

## XXX: TPP_FS_ALTSEP

## XXX: TPP_FS_ISSEP

## XXX: TPP_FS_ISABS

## XXX: tpp_io_handle

## XXX: tpp_io_skip_blocking

## XXX: tpp_io_normalize_filename

## XXX: tpp_io_withenv

## XXX: tpp_time

## XXX: TPP_CONFIG_HAVE_LOCALTIME_R

## XXX: TPP_KWDIDENTIFIER_*

## XXX: TPP_EXTNAME_*

## XXX: TPP_HAVE_TPP_WG_*

## XXX: TPP_HAVE_BUILTIN_KEYWORDS_ENDING_IN_HEADER_GCC

## XXX: TPP_PATH_MAX


## __OPTIMIZE_SIZE__

Omit *fast* path optimizations where the same results are also produced by fallback/generic code-paths. The result is code that is slightly slower, but contains less branches and thus: less code in total.

Unlike many other config macros, the value that this macro is defined to doesn't matter to TPP. Instead, TPP will omit *fast* path optimizations whenever this macro is defined at all.

<details><summary>Details</summary>

Example:

```c
#define __OPTIMIZE_SIZE__ 1
```
</details>



# Buffer size hints

In order to allow you to tweak performance and buffer characteristics of TPP, the following configuration options allow you to configure how various internal buffers are allocated/resized/grow.

| Config | Default | Description |
| ------ | ------- | ----------- |
| `TPP_IO_CHUNKSIZE` | `65536` | Default size (in bytes) of chunks allocated when loading `TPP_FILE_KIND_IO`-files from disk |
| `TPP_IO_MINREAD` | `4096` | Lower bound for file chunk buffers when extending very large file chunks |
| `TPP_IO_READFILE_BLOCKING_STACK_BUFSIZE` | `1024` | Size of the fallback stack-buffer used for data-transfer by the implementation of `tpp_lexer_parseembed()` |
| `TPP_MACRO_ARGUMENT_BUFFER_MINSIZE` | `64` | Initial size of the string-buffer used to store the expanded form of macro arguments |
| `TPP_STRING_BUILDER_MINALLOC` | `64` | Initial size of `tpp_string_buffer` |

