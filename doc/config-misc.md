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

For an example of a (simple) project that uses `TPP_CONFIG_BUILTINS_FILENAME`, see [`/examples/simple-with-cache`](../examples/simple-with-cache/README.md).



## TPP_AMALGAMATION_H

Define to the relative `#include "filename"` with which `tpp-amalgamation.c` can include `tpp-amalgamation.h`.

By default, this is simply defined as `"tpp-amalgamation.h"`, which assumes that you've put `tpp-amalgamation.h` and `tpp-amalgamation.c` into the same directory. If you've decided to separate the 2, you must re-define this config to be whatever `#include`-string is needed for `tpp-amalgamation.c` to `#include TPP_AMALGAMATION_H` the associated `tpp-amalgamation.h`.



## TPP_OS_WINDOWS

Define to `1` if the host OS is windows/dos-like. Otherwise, defined to `0`



## TPP_OS_UNIX

Define to `1` if the host OS is unix/linux-like. Otherwise, defined to `0`



## TPP_HOST_NO_SYSTEM_INCLUDES

Define to `1` to prevent TPP from ever doing `#include <some/header.h>`. This allows you to use TPP in an environment where you're supplying your own definitions for system functions needed by TPP (see below), and don't want to bother emulating various headers like `<stdlib.h>` or `<unistd.h>`



## TPP_HOST_HAS_ATTRIBUTE

Wrapper around `__has_attribute` for host compiler.



## TPP_HOST_HAS_DECLSPEC_ATTRIBUTE

Wrapper around `__has_declspec_attribute` for host compiler.



## TPP_HOST_HAS_CPP_ATTRIBUTE

Wrapper around `__has_cpp_attribute` for host compiler.



## TPP_HOST_HAS_INCLUDE

Wrapper around `__has_include` for host compiler.



## TPP_HOST_HAS_BUILTIN

Wrapper around `__has_builtin` for host compiler.



## TPP_HOST_HAVE_PP_VARARGS

Define to `1` if host compiler/preprocessor supports `#define varargs(...) __VA_ARGS__`-style macro definitions.



## TPPCALL

Calling convention to use in TPP APIs. Injected before function names of `TPP_DECL` (see below) function definitions: `TPP_DECL int TPPCALL tpp_function(void);`



## TPPVCALL

Same as `TPPCALL`, but for varargs function definitions: `TPP_DECL int TPPVCALL tpp_function(char const *format, ...);`



## TPP_GCC_VERSION_NUM

The version number of a hosting GCC compiler, or `0` if the hosting compiler isn't GCC-compatible. If the hosting compiler is GCC-compatible, this macro should be defined like `__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCH__`



## TPP_USE_STATIC

Define to `1` to have TPP define its entire API using `static`. This means that everything that makes up TPP is confined to a single compilation unit, meaning that:
- You could in theory have multiple, distinctly configured instances of TPP within the same application
- Everything that makes up TPP is **only** accessible from the compilation unit that is including `tpp-amalgamation.c` (and if you want to access some part of TPP from outside that compilation unit, you'll have to write your own wrappers)
- The hosting compiler can better optimize TPP since anything that could normally only be done via LTO (link-time optimization) can now be done directly



## TPP_IMPL

The declaration prefix used by `tpp-amalgamation.c` when implemented API functions that were previously defined by `tpp-amalgamation.h`



## TPP_DECL

The declaration prefix used by `tpp-amalgamation.h` when defining API functions will later be implemented by `tpp-amalgamation.c`



## TPP_CONST_IMPL

Same as `TPP_IMPL`, but used for implementing `const` data objects (like lookup arrays and the like). This macro also comes with a companion `TPP_CONST_DECL`, but that one cannot be overwritten. Additionally `TPP_CONST_DECL` (intentionally) isn't defined when `-DTPP_USE_STATIC=1`, since C doesn't offer a way to do forward-declaration of static data objects.



## tpp_assume

```c
tpp_assume(expression_that_is_always == true);
```

Hint to the host compiler that expressions are always true, and that it is undefined behavior if they somehow wouldn't be.



## tpp_restrict

Macro that should expand to `restrict` (or an equivalent keyword), so-long as the host compiler supports a C99-compliant `restrict` semantics. If the host compiler doesn't support C99-compliant `restrict`, this macro should be defined as empty.



## TPP_NONNULL

Should be defined as a function-like macro to expand to GCC-like `__attribute__((nonnull(1, 2, 3)))` annotations: `TPP_NONNULL((1, 2, 3))` (note the double parenthesis, which is intentional so-as not to rely on the host compiler supporting `TPP_HOST_HAVE_PP_VARARGS`)



## TPP_WUNUSED

Wrapper around GCC's `__attribute__((warn_unused_result))`



## TPP_RETNONNULL

Wrapper around GCC's `__attribute__((returns_nonnull))`



## TPP_PURECALL

Wrapper around GCC's `__attribute__((pure))`



## TPP_CONSTCALL

Wrapper around GCC's `__attribute__((const))`



## TPP_COLDCALL

Wrapper around GCC's `__attribute__((cold))`



## TPP_NOINLINE

Wrapper around GCC's `__attribute__((noinline))`



## TPP_FLEX_ARRAY

Should expand to whatever must be written between the `[brackets]` of a trailing flexible-array-member in `struct` definitions, such that the host compiler understand the construct.

Note that TPP never invokes `sizeof()` on a structure that ends with a trailing `T array[TPP_FLEX_ARRAY]` member, but instead uses `offsetof(S, array)` in these cases, so if the host compiler doesn't understand flexible arrays you can just define this as some very larger integer values.



## TPP_FALLTHRU

Wrapper around GCC's `__attribute__((__fallthrough__));` (note the trailing `;`) -- this macro is inserted wherever TPP intentionally does a fallthrough from one switch-case to another.



## TPP_CHAR_BIT

Should expand to `CHAR_BIT`



## TPP_INTERNAL

You probably shouldn't override this macro (the same way you shouldn't override `TPP_BUILDING` or `TPP_BUILDING_OPTIONAL`). This macro is used to guard (by renaming) *hidden* / *internal* parts of TPP's API, so you don't get any ideas about directly accessing internals you shouldn't be tinkering with.

By default, this macro prepends a leading `_` before the identifier it is given, following the general convention that any identifier with a leading `_` that might appear in `tpp-amalgamation.h` should be considered off-limits.



## TPP_HAVE_TPP2_COMPAT

Define to `1` to enable some minor TPP2 compatibility hacks that are required by the `tpp2.h` compatibility header. If you're using TPP3 as intended, you should never need to overwrite this macro or define it to `1`.



## TPP_DEBUG

Define to `1` to enable some additional code used by TPP to debug itself. This includes internal assertions (see `tpp_assert`), as well as the trashing of structures during finalization.



## TPP_INLINE

Expand to the prefix for static+inline function definitions.



## tpp_offsetof

Wrapper around `offsetof()` as normally defined in `<stddef.h>`



## tpp_container_of

A helper macro to (safely) extract the parent-container from a pointer to one of its children, given the type of the parent-container, the name of the field in question, and a pointer to said field.



## tpp_lengthof

Helper macro to determine the length (in elements) of a statically allocated array.



## tpp_unreachable

Wrapper around GCC's `__builtin_unreachable`



## tpp_expect

Wrapper around GCC's `__builtin_expect`



## tpp_likely

Wrapper around `tpp_expect` that is used like `if tpp_likely(expr)`



## tpp_unlikely

Wrapper around `tpp_expect` that is used like `if tpp_unlikely(expr)`



## tpp_uint_least8, tpp_int_least8, TPP_UINT_LEAST8_MAX, TPP_UINT_LEAST8_C

Types, limits and constant-creation for integer types that must have at least 8 bits of data



## tpp_uint_least16, tpp_int_least16, TPP_UINT_LEAST16_MAX, TPP_UINT_LEAST16_C

Types, limits and constant-creation for integer types that must have at least 16 bits of data



## tpp_uint_least32, tpp_int_least32, TPP_UINT_LEAST32_MAX, TPP_UINT_LEAST32_C

Types, limits and constant-creation for integer types that must have at least 32 bits of data



## tpp_uint_least64, tpp_int_least64, TPP_UINT_LEAST64_MAX, TPP_UINT_LEAST64_C

Types, limits and constant-creation for integer types that must have at least 64 bits of data.

Unlike the other types above, this one is actually optional (if left undefined, TPP will make an attempt to define these itself, but if that also fails, then TPP is still able to function as normal (though certain features might behave differently or are more restrictive that they would otherwise be))



## tpp_uint_fast8, tpp_int_fast8, TPP_UINT_FAST8_MAX, TPP_UINT_FAST8_C

Same as `tpp_uint_least8` with the same requirement of having at least 8 bits of data. However, this type may be larger than `tpp_uint_least8` if making it larger might improve performance on whatever host architecture TPP is being compiled for.



## tpp_uint_fast16, tpp_int_fast16, TPP_UINT_FAST16_MAX, TPP_UINT_FAST16_C

Same as `tpp_uint_least16` with the same requirement of having at least 16 bits of data. However, this type may be larger than `tpp_uint_least16` if making it larger might improve performance on whatever host architecture TPP is being compiled for.



## tpp_uint_fast32, tpp_int_fast32, TPP_UINT_FAST32_MAX, TPP_UINT_FAST32_C

Same as `tpp_uint_least32` with the same requirement of having at least 32 bits of data. However, this type may be larger than `tpp_uint_least32` if making it larger might improve performance on whatever host architecture TPP is being compiled for.



## tpp_intmax, tpp_uintmax, TPP_UINTMAX_MAX, TPP_UINTMAX_C

The largest integer type that should be supported by TPP. This is also the integer type that is used by various APIs, including TPP's builtin integer decoding facilities.



## tpp_size, TPP_SIZE_MAX

Wrapper around `size_t` and `SIZE_MAX` from `<stddef.h>`



## tpp_ssize, TPP_SSIZE_MAX

Wrapper around `ptrdiff_t` and `PTRDIFF_MAX` from `<stddef.h>`



## tpp_hash, TPP_HASH_MAX, TPP_HASH_C

The type used by TPP to store keyword hashes. By default, this aliases `tpp_uint_fast32`



## tpp_line, tpp_column

The types used by TPP to represent line/column numbers. By default, these alias `tpp_int_fast32`



## tpp_char

The character type used when working with data from files. This pretty much **has** to be defined as `unsigned char`, since this *must* represent whatever data type is needed to describe file-bytes on a level that can also describe utf-8 bytes, **and** must be `undefined`, since `tpp_char` values are often used as indices in various lookup tables.



## tpp_unichar, TPP_UNICHAR_C

The character type used to represent decoded unicode/utf-32/UCS-32 characters. By default, this aliases `tpp_uint_least32`



## tpp_counter

The data type that is internally used by the implementation of `__COUNTER__` and `__TPP_COUNTER` (see `TPP_HAVE_MACRO___COUNTER__` and `TPP_HAVE_MACRO___TPP_COUNTER`)



## tpp_float

The floating-point data type used by TPP's builtin float-decoder (see `tpp_lexer_decodefloat()` and `TPP_HAVE_LEXER_DECODEFLOAT`), as well as `#if`-style preprocessor expressions when `TPP_HAVE_BUILTIN_EXPR_FLOATS` is enabled (see `tpp_expr_value` below)



## TPP_REF

A hint annotation to indicate that a variable/field/return-value/parameter carries/stores a *reference-counted* to the pointed-to structure. Primarily used with `tpp_string`, in which case this macro may be overwritten by static analysis tools to trace ownership passing of reference-counted objects.



## TPP_STATIC_ASSERT

A function-like macro that performs compiler-time (as opposed to preprocessor-time, or runtime) assertion of a constant expressions. Used by TPP to assert some internal consistencies that would otherwise cause runtime problems, or hard-to-understand compilation errors further down the line.



## TPP_STATIC_ASSERT_MSG

Same as `TPP_STATIC_ASSERT`, but takes a second argument specifying a message to give human-readable hints as to what is going on when the assertion fails.



## tpp_memcpy, tpp_strlen, tpp_strchr, _tpp_strnlen, tpp_strcmp, tpp_memcmp, tpp_memset, tpp_memchr, tpp_memmove

Wrappers around C standard `<string.h>` functions of the same name to-be used by TPP.

Note the leading underscore on `_tpp_strnlen`. You can still provide a definition here, but you can also provide a definition for `tpp_strnlen` directly (see below). `_tpp_strnlen` is merely used by the default implementation of `tpp_strnlen`



## tpp_memmem

Optional: define as a compliant implementation of `memmem(3)`, or leave undefined if TPP should supply its own definition internally.



## tpp_memmoveup

Same as `tpp_memmove`, but only called when it is compile-time known that `dst > src` (aka. `FIRST_ARGUMENT > SECOND_ARGUMENT`)



## tpp_memmovedown

Same as `tpp_memmove`, but only called when it is compile-time known that `dst < src` (aka. `FIRST_ARGUMENT < SECOND_ARGUMENT`)



## tpp_strnlen

Wrapper around `strnlen` (aka. `_tpp_strnlen`). If supported by the host compiler, the default implementation checks if the second argument is compile-time known to be a *very large value*, in which case `tpp_strlen()` is called. Aside from that (optional) aspect, this is merely a wrapper around `strnlen()` as defined by the C standard.



## tpp_mempcpy

Same as `tpp_memcpy`, but return a pointer *after* the written-to portion, rather than at its start. Some systems provide a function `mempcpy(3)`, which may be supplied here to simplify / stream-line places where TPP uses this function.



## tpp_bzero

Wrapper around `tpp_memset` that only takes 2 arguments (`base` and `num_bytes`), and is equivalent to a call `(void)memset(base, 0, num_bytes)` (note the `void`-cast of the return value: whereas `memset` re-returns `base`, `tpp_bzero` returns `void`). Some systems supply an API-compatible function `bzero(3)` which may be supplied here to simplify / stream-line places where TPP uses this function.



## tpp_dbg_memset

Similar to `tpp_bzero` (i.e.: a 2-argument function returning `void`), but instead of filling memory with 0-es, this function fills memory with an easily recognizable bit-pattern, that is unlikely to be valid for most uses (e.g.: when treated as a pointer, the program will likely SEGFAULT). If `TPP_DEBUG` is disabled however, this function becomes a no-op.

TPP uses this function to trash component/object-memory after those components/objects have been finalized, in order to make use-after-free-like invalid API usages easier to spot.



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



## tpp_alloca

An optional wrapper around GCC's `__builtin_alloca`. If not defined, TPP makes an attempt to define this function itself, but if it fails, TPP will implement its APIs/functions without the use of *dynamic stack memory allocation*.



## tpp_assert

Wrapper around `assert`, as defined by `<assert.h>`. Additionally, if `TPP_DEBUG` is defined as `0`, this macro is defined as a no-op (that won't even expand/evaluate its argument)


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



## TPP_SYSCALL_NOFAIL

Same as `TPP_SYSCALL`, but only takes a singular argument (`expr`, i.e.: the system call expression). Behavior of this macro is the same as `TPP_SYSCALL`, but it is used instead of that one in cases where the surrounding function has no way to indicating an error.

In general, this is only the case for `tpp_io_close()`, since that function is called during cleanup operations, and TPP (intentionally) doesn't define its cleanup functions as being able to throw errors.



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



## tpp_formatprinter_print_byname

A convenience wrapper around `tpp_formatprinter_print` and `tpp_formatprinter_of`:

```c
#define tpp_formatprinter_print_byname(NAME, arg, text, num_bytes) \
	tpp_formatprinter_print(tpp_formatprinter_of(NAME), arg, text, num_bytes)
```



## tpp_formatprinter_print_cstr

A wrapper around `tpp_formatprinter_print` that takes `char const *` for its `text` argument, rather than `tpp_char const *`



## tpp_formatprinter_print_conststr

A wrapper around `tpp_formatprinter_print_cstr` that takes `char const[?]` for its `CONSTstr` argument, and automatically determines the length of that constant string, such that constant strings can easily be printed without the need for manually determining its length



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



## tpp_once

Ensure that a given code-block is executed only once (in a way that thread-safe, such that with multiple parallel threads, only 1 will execute the block, and all other will wait for the first to finish)

```c
#define tpp_once(expr) ...
```



## XXX: tpp_ascii_is*

## XXX: TPP_HAVE_ASSUME_ASCII_CTYPE

## XXX: tpp_ascii_ismb

## XXX: tpp_ascii_isspace_nolf

## XXX: tpp_ascii_islf_or_mblf

## XXX: tpp_ascii_asdigit, tpp_ascii_ofdigit

## XXX: tpp_ascii_isoctdigit, tpp_ascii_asoctdigit, tpp_ascii_ofoctdigit

## XXX: tpp_ascii_islwrxdigit, tpp_ascii_aslwrxdigit, tpp_ascii_oflwrxdigit

## XXX: tpp_ascii_isuprxdigit, tpp_ascii_asuprxdigit, tpp_ascii_ofuprxdigit

## XXX: tpp_ascii_isxdigit, tpp_ascii_asxdigit

## XXX: tpp_ascii_tolwrxdigit

## XXX: tpp_ascii_touprxdigit

## XXX: tpp_unicode_issymstrt, tpp_unicode_issymcont, tpp_unicode_isspace, tpp_unicode_islf

## XXX: tpp_unicode_isspace_nolf

## XXX: tpp_unicode_utf8seqlen_mb_getcur

## XXX: tpp_unicode_utf8seqlen_getcur

## XXX: tpp_unicode_utf8seqlen_mb_getmax

## XXX: tpp_unicode_utf8seqlen_getmax

## XXX: tpp_ascii_isutf8cont

## XXX: tpp_fuzzy_memcmp

## XXX: tpp_xml_entity_lookup

## XXX: tpp_unicode_byname_lookup

## XXX: tpp_decode_named_printnearest

## XXX: tpp_expr_value

## XXX: tpp_lexer_decodeint_expr

## XXX: tpp_lexer_decodefloat_expr

## XXX: tpp_lexer_parsestring_expr

## XXX: tpp_lexer_parsecharacter_expr

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



# Buffer size hints

In order to allow you to tweak performance and buffer characteristics of TPP, the following configuration options allow you to configure how various internal buffers are allocated/resized/grow.

| Config | Default | Description |
| ------ | ------- | ----------- |
| `TPP_IO_CHUNKSIZE` | `65536` | Default size (in bytes) of chunks allocated when loading `TPP_FILE_KIND_IO`-files from disk |
| `TPP_IO_MINREAD` | `4096` | Lower bound for file chunk buffers when extending very large file chunks |
| `TPP_IO_READFILE_BLOCKING_STACK_BUFSIZE` | `1024` | Size of the fallback stack-buffer used for data-transfer by the implementation of `tpp_lexer_parseembed()` |
| `TPP_MACRO_ARGUMENT_BUFFER_MINSIZE` | `64` | Initial size of the string-buffer used to store the expanded form of macro arguments |
| `TPP_STRING_BUILDER_MINALLOC` | `64` | Initial size of `tpp_string_buffer` |

