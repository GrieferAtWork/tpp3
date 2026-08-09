# tpp3 - Tiny PreProcessor

TPP is a tiny (single source file) C Preprocessor, meant as a low-level backend for compilers of C and C-like languages, whilst implementing pretty much all preprocessor extensions supported by other compilers, ontop of a large number of its own (tpp-specific) extensions.

## Basic design

TPP is a *single-pass*, *text-based* preprocessor (as opposed to multi-pass or token-based, though TPP obviously still produces tokens in the end). If you want, it also lets you disable pretty much everything that makes it a preprocessor in the conventional sense, allowing you to strip it down to a fairly basic tokenizer -- *everything* is configurable.

TPP is *not* exclusive to C/C++ -- it has builtin support for lots of different token types normally only found in other languages (e.g. java `""" block strings """`, or SQL `-- comments`). Like all other features, every supported token can be individually configured at compile-time and/or runtime.

TPP includes full Unicode support (and not just in strings), based on `XID_Start` and `XID_Continue`:

```c
#define ㄒ卩卩(ᗪㄖ乇丂) ㄩ几丨匚ㄖᗪ乇•ᗪㄖ乇丂•山ㄖ尺Ҝ
ㄒ卩卩(sure does)
```

<details>
<summary>Expansion</summary>

```c
ㄩ几丨匚ㄖᗪ乇•sure does•山ㄖ尺Ҝ
```
</details>


## Using TPP

TPP is primarily designed to be statically included in other projects (similar to sqlite3).

As such, it accepts lots of different configuration options, allowing you to configure pretty much every aspect of its functionality either

- at compile-time (thus hard-disabling code for features you don't want/need)
- or at runtime, allowing you to control features dynamically (e.g. via commandline flags) or directly from within code being processed (via `#pragma extension`)

If this is what you want, you just need to download the following 2 files (this should be familiar to you if you've used sqlite3) ...

- [tpp-amalgamation.h](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp-amalgamation.h)
- [tpp-amalgamation.c](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp-amalgamation.c)

... And add them to your C project. Examples for a simple integration setup can be found in [/samples](./samples) and available configuration options (and the features they control) are listed in [/doc/config.md](./doc/config.md).


### Frontend

Secondly, TPP provides a CLI [`frontend`](/src/frontend.c) that implements a (highly) GCC/CPP-compatible commandline utility. This one's mainly there as a proof-of-concept, since such a tool alone doesn't warrant the degree of customization offered by TPP.


### Basic example

For more examples, see [/samples](./samples)

```c
tpp_errno error;
tpp_lexer lexer;
tpp_lexer_init(&lexer);
error = tpp_lexer_initfile_open(&lexer, "input.c", TPP_SIZE_MAX);
if (TPP_ISERR(error))
	HANDLE_ERROR();
for (;;) {
	tpp_token_id tok = tpp_lexer_yield(&lexer);
	if (TPP_TOK_ISERR(tok))
		HANDLE_ERROR();
	if (tok == TPP_TOK_EOF)
		break;
	fwrite(tpp_lexer_gettokenstart(&lexer), 1,
	       tpp_lexer_gettokenlen(&lexer), stdout);
}
tpp_lexer_finifile(&lexer);
tpp_lexer_fini(&lexer);
```

NOTES:

- `tpp_lexer lexer;` the heartpiece of TPP: a lexer is what keep track of everything related to preprocessing: the current token, the `#include`-stack (as well as the current file), keywords, macros, configuration (extensions/features), context flags, counters, include-paths, etc.  
  Initializing a lexer is done in 2 steps:
	- `tpp_lexer_init(&lexer);`:  
	  Initialize common fields of a lexer (everything except for the current file and last token). This step of initialization is designed to never fail (there are no mandatory heap-buffers or anything like that), meaning that `tpp_lexer_init(&lexer);` returns `void` so you don't have to check for errors!
	- `tpp_lexer_initfile_open(&lexer, "input.c", TPP_SIZE_MAX)`:  
	  Initialize the initial input file. For this purpose, other `tpp_lexer_initfile_*()` also exist. This function can only be used to initialize the *initial* input file. Assuming that`TPP_HAVE_INCLUDE_STACK` is enabled, additional files would need to be pushed onto the `#include`-stack using`tpp_lexer_pushfile_*`
- `tok = tpp_lexer_yield(&lexer)`:  
  Yield the next token whilst dealing with preprocessor directives, macro expansion, automatically popping empty files off the `#include`-stack, etc.
- `tpp_lexer_finifile(&lexer);`, `tpp_lexer_fini(&lexer);`:  
  Like initialization, finalization is also done in 2 steps:
	- `tpp_lexer_finifile(&lexer);`:  
	  Undo the effects of `tpp_lexer_initfile_*()`. Also finalizes all additional files pushed onto the`#include`-stack
	- `tpp_lexer_fini(&lexer);`:  
	  Undo the effects of `tpp_lexer_init(&lexer);`. Also finalizes all other dynamically allocated components of the lexer.

### Configuration

- [/doc/config.md](doc/config.md)


### Source Extensions

In addition to the amalgamations, TPP also offers some additional, secondary amalgamations that can be used to gain additional functionality that has intentionally not been included within the TPP core.

Each of these extensions can be included the same way as the TPP core can be, each coming with its own stand-alone amalgamation-header and source. To use any one of these, simply download its header and source, and put them in the same directory as you've put [`tpp-amalgamation.h`](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp-amalgamation.h) and [`tpp-amalgamation.c`](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp-amalgamation.c). Then, you can simply `#include` the extra amalgamations just like you're already doing with the core ones.

| Name | Header | Source | Description |
| ---- | ------ | ------ | ----------- |
| **EMITTER** | [`tpp-emitter-amalgamation.h`](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp-emitter-amalgamation.h) | [`tpp-emitter-amalgamation.c`](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp-emitter-amalgamation.c) | Standardized ways of re-emitting tokens (think: `gcc -E`), whilst injecting `#line` directives to ensure that `__FILE__`, `__LINE__` and `__COLUMN__` are retained for every token.<br/>Also used to implement TPP's [frontend](#frontend) |
| **MAKEFILE** | [`tpp-makefile-amalgamation.h`](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp-makefile-amalgamation.h) | [`tpp-makefile-amalgamation.c`](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp-makefile-amalgamation.c) | Helpers for generating a *Makefile*-compatible dependency descriptor of the parsed input file, and implementing support for GCC CLI switches `-M`, `-MM`, `-MF`, `-MG`, `-MP`, `-MT`, `-MQ`, `-MD`, `-MMD`.<br/>Also used to implement TPP's [frontend](#frontend) |
| - | - | - | - |


## Implementation Notes

- Disregarding macros and other functions that obviously require dynamic allocation, TPP has `O(1)` memory usage, meaning it automatically unloads old, unreferenced and already processed parts of input files at the same time as new parts are loaded
- TPP can preprocess input on-the-fly, meaning it (normally) doesn't need to load an entire file into memory all-at-once. As a consequence, TPP also falls under the category of *single-pass* preprocessors
- TPP is *text-based*, meaning it converts raw input strings into tokens on-the-fly, rather than operating on an already tokenized input stream. Meanwhile, a *token-based* preprocessor would initially convert its input to a stream of distinct tokens, with all further processing then performed on those tokens. Both approaches have their advantages and disadvantages:
	- Advantage: Faster, because no conversion step is required
	- Advantage: Because the original input can be re-used for every step, TPP doesn't require any heap memory to preprocess inputs that are already loaded into memory (and doesn't contain anything that requires dynamic definitions, like macros or keywords)
	- Advantage: It becomes trivial to convert between token types, escape/unescape strings, or figure out where tokens originate from (file/line/column)
	- Advantage: Availability and behavior of tokens can be (re-)configured on-the-fly and will immediately show results (as opposed to requiring something along the lines of a *retokenization pass*)
	- Disadvantage: sometimes, tokens have to be parsed multiple times, meaning that certain error messages (like `-Wmultiline-string`) may be emitted multiple times for the same string location
	- Disadvantage: TPP has to be careful not to accidentally concatenate pasted tokens in situations where input has to be re-processed (s.a. `TPP_HAVE_MAGIC_WHITESPACE`)
- Not every combination of features will necessarily be able to compile. All features are automatically turned on if they're needed (or based on `TPP_PROFILE`), but if you manually disable a feature needed to implement something that is enabled, you'll probably get syntax/linker errors complaining about undefined functions



## Features (abbr.)

NOTE: A full list of configuration options and their meaning can be found in [/doc/config.md](doc/config.md)

| Feature | Origin | Configuration (`TPP_HAVE_*`) | Extension name (default) | Description |
| ------- | ------ | ------------- | ------------------------ | ----------- |
| `??/` | pre_stdc-C23 | `TRIGRAPHS` | `-ftrigraphs` | Textual replacements for various `??x` sequences; e.g. `??/` being replaced by `\` |
| `<%` | C95+ | `DIGRAPHS` | `-fdigraphs` | Token replacements for some 2-char sequences like `<%` being parsed as `{` |
| `__VA_ARGS__` | pre_stdc+ | `VA_ARGS_IN_MACROS` | `-fva-args-in-macros` | variable-argument macros: `#define varargs(...) __VA_ARGS__` |
| `args...` | GCC | `NAMED_VARARGS_IN_MACROS` | `-fnamed-varargs-in-macros` | variable-argument macros with named varargs: `#define varargs(args...) args` |
| `,##__VA_ARGS__` | GCC | `VA_GLUE_COMMA_IN_MACROS` | `-fglue-comma-in-macros` | GCC-style va-comma: `#define printf(format, ...) fprintf(stdout, format, ##__VA_ARGS__)` |
| `?:` | GCC | `BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT` | `-fif-else-optional-true` | GCC-style if-else in expressions: `#if foo ?: bar` |
| `__VA_OPT__` | C++20 | `VA_OPT_IN_MACROS` | `-fva-opt-in-macros` | C++20 `__VA_OPT__`: `#define printf(format, ...) fprintf(stdout, format __VA_OPT__(,) __VA_ARGS__)` |
| `\e` | GCC | `STRING_ESCAPE_E` | `-fstring-escape-e` | Support for `"\e"` (for U+001B) escape sequences |
| `0b00101010` | C++14 | `LEXER_DECODEINT_BINARY_LITERALS` | `-fbinary-literals` | Enable support for `0b` literals in builtin lexer expressions |
| `__pragma` | MSVC | `MACRO___pragma` | `-f__pragma` | MSVC's `__pragma(push_macro("foo"))` keyword/macro |
| `_Pragma` | C++11 | `MACRO__Pragma` | `-f_Pragma` | STDC's `_Pragma("push_macro(\"foo\")")` keyword/macro |
| `#if`, `#ifdef`, ... | C | `CPP_IF_ELSE_ENDIF` | `-fif-directives` | Conditional code blocks |
| `#define`, `#undef` | C | `CPP_DEFINE` | `-fdefine-directives` | User-defined macros |
| `#pragma` | C | `CPP_PRAGMA` | `-fpragma-directives` | Entry point for pragma extensions |
| `#warning` | GCC | `CPP_WARNING` | `-fwarning-directives` | GCC's `#warning My Message Here` directive |
| `#error` | C | `CPP_ERROR` | `-ferror-directives` | C's `#error My Message Here` directive |
| `#line` | C | `CPP_LINE` | `-fline-directives` | C's `#line 42 "my-file.h"` directive |
| `#include` | C | `CPP_INCLUDE` | `-finclude-directives` | Your good 'ol `#include <stdio.h>` directive |
| `#include_next` | GCC | `CPP_INCLUDE_NEXT` | `-finclude-next-directives` | GCC's `#warning My Message Here` directive |
| `#import` | OBJc | `CPP_IMPORT` | `-fimport-directives` | OBJc-style `#import <header.h>` directive |
| `#embed` | C23 / C++26 | `CPP_EMBED` | `-fembed-directives` | C++26 `#embed "resource.dat" offset(5) limit(10)` directive |
| `#assert`, `#unassert` | GCC | `CPP_ASSERT` | `-fassertions` | GCC `#assert foo(bar)` directive and `#if #foo(bar)` checks |
| `#ident`, `#sccs` | pre_stdc | `CPP_IDENT_SCCS` | `-fident-directives` | Ancient compiler insert-comment-into-`.o`-file directive |
| `__FILE__` | C | `MACRO___FILE__` | `-f__FILE__` | Expand to current file's name |
| `__LINE__` | C | `MACRO___LINE__` | `-f__LINE__` | Expand to current line's number |
| `__TIME__` | C | `MACRO___TIME__` | `-f__TIME__` | Expand to something like `"12:37:58"` |
| `__DATE__` | C | `MACRO___DATE__` | `-f__DATE__` | Expand to something like `"Jul  6 2026"` |
| `__BASE_FILE__` | GCC | `MACRO___BASE_FILE__` | `-fbasefile-macro` | Expand to base file's name |
| `__INCLUDE_LEVEL__` | GCC | `MACRO___INCLUDE_LEVEL__` | `-finclude-level-macro` | Expand to numerical representation of include depth |
| `__COUNTER__` | GCC | `MACRO___COUNTER__` | `-fcounter-macro` | Expand to `1+` its previous expansion |
| `__TIMESTAMP__` | GCC | `MACRO___TIMESTAMP__` | `-ftimestamp-macro` | Expand to something like `"Mon Jul  6 12:37:58 2026"` |
| `__COLUMN__` | TPP | `MACRO___COLUMN__` | `-fcolumn-macro` | Expand to current column's number |
| `__has_embed()` | C23 / C++26 | `MACRO___has_embed` | `-f__has_embed` | Check if `#embed` exists: `#if __has_embed("resource.dat" offset(5) limit(10))` |
| `__has_include()` | Clang / C23 / C++17 | `MACRO___has_include` | `-f__has_include` | Check if `#include` exists: `#if __has_include(<stdio.h>)` |
| `__has_include_next()` | Clang | `MACRO___has_include_next` | `-f__has_include_next` | Check if `#include_next` exists: `#if __has_include_next(<stdio.h>)` |
| `__has_attribute()` | Clang | `CLANG_MACRO___has_attribute` | `-fclang-__has_attribute` | Check support of `__attribute__((foo))` |
| `__has_builtin()` | Clang | `CLANG_MACRO___has_builtin` | `-fclang-__has_builtin` | Check support of `__builtin_foo()` |
| `__has_cpp_attribute()` | Clang | `CLANG_MACRO___has_cpp_attribute` | `-fclang-__has_cpp_attribute` | Check support of `[[foo]]` |
| `__has_declspec_attribute()` | Clang | `CLANG_MACRO___has_declspec_attribute` | `-fclang-__has_declspec_attribute` | Check support of `__declspec(foo)` |
| `__has_extension()` | Clang | `CLANG_MACRO___has_extension` | `-fclang-__has_extension` | Check support of compiler extensions |
| `__has_feature()` | Clang | `CLANG_MACRO___has_feature` | `-fclang-__has_feature` | Check support of compiler features |
| `__has_c_attribute()` | Clang | `CLANG_MACRO___has_c_attribute` | `-fclang-__has_c_attribute` | Check support of `[[foo]]` |
| `__is_identifier()` | Clang | `MACRO___is_identifier` | `-f__is_identifier` | Check if something is a builtin identifier |
| `__is_deprecated()` | TPP | `MACRO___is_deprecated` | `-f__is_deprecated` | Check if an identifier has been `#pragma deprecated("foo")`-ed |
| `__is_poisoned()` | TPP | `MACRO___is_poisoned` | `-f__is_poisoned` | Check if an identifier has been `#pragma GCC poison foo`-ed |
| `__has_extension()` | TPP | `MACRO___has_extension` | `-f__has_extension` | Check if a TPP extension is known + enabled: `#if __has_extension("-ftrigraphs")` |
| `__has_known_extension()` | TPP | `MACRO___has_known_extension` | `-f__has_known_extension` | Check if a TPP extension is known: `#if __has_known_extension("-ftrigraphs")` |
| `__has_warning()` | TPP | `MACRO___has_warning` | `-f__has_warning` | Check if a TPP warning is known + enabled: `#if __has_warning("-Wmultiline-string")` |
| `__has_known_warning()` | TPP | `MACRO___has_known_warning` | `-f__has_known_warning` | Check if a TPP warning is known: `#if __has_known_extension("-Wmultiline-string")` |
| `#! comment` | TCC, TPP | `CPP_EXCLAIM` | `-fshebang-directives` | Treat `#!` preprocessor directives as comments |
| `$` | - | `TPP_TOK_DOLLAR` | `-ftok-dollar` | Treat `$` as its own token, rather than as part of identifiers |
| `'abc'` | - | `TPP_W_MULTICHAR_LITERAL` | `-Wmultichar` | Multiple characters in `'abc'` character literals |
| `__VA_COMMA__` | TPP | `VA_COMMA_IN_MACROS` | `-fva-comma-in-macros` | Expands to `,` if varargs are non-empty: `#define printf(format, ...) fprintf(stdout, format __VA_COMMA__ __VA_ARGS__)` |
| `__VA_NARGS__` | TPP | `VA_NARGS_IN_MACROS` | `-fva-nargs-in-macros` | Expands to the # of varargs, allowing easy macro overloading<details><summary>Example</summary><pre><code>#define min\_1(a)       a</code><br/><code>#define min\_2(a, b)    ((a) < (b) ? (a) : (b))</code><br/><code>#define min\_3(a, b, c) min\_2(min\_2(a, b), c)</code><br/><code>#define min(\.\.\.)       min\_##\_\_VA_NARGS\_\_(\_\_VA_ARGS\_\_)</code></pre></details> |
| - | pre_stdc | `TRADITIONAL_MACROS` | `-ftraditional-macro` | Traditional macro expansion rules<details><summary>Example</summary><pre><code>#define CAT(a, b) a##b</code><br/><code>#define STR(x)    #x</code><br/><code>#pragma extension(push, "-ftraditional-macro")</code><br/><code>#define T\_CAT(a, b) a/\*\*/b</code><br/><code>#define T\_STR(x)    "x"</code><br/><code>#pragma extension(pop)</code><br/><code>CAT(10, 20)   // 1020</code><br/><code>STR(10)       // "10"</code><br/><code>T\_CAT(10, 20) // 1020</code><br/><code>T\_STR(10)     // "10"</code></pre></details> |
| `#define foo{...}` | TPP | `ALTERNATIVE_MACRO_PARENTHESIS` | `-falternative-macro-parenthesis` | Alternative parenthesis pairs in macros<details><summary>Example</summary><pre><code>#define normal\_macro(x) you\_should("know this", x)</code><br/><code>#define array\[index\]    get\_ident(ARRAY, index)</code><br/><code>#define block{\.\.\.}      do { \_\_VA_ARGS\_\_ } while (should\_continue())</code><br/><code>#define point&lt;T&gt;        struct { T x; T y; }</code></pre></details> |
| - | TPP | `MACRO_RECURSION` | `-fmacro-recursion` | Allow self-recursion for certain macros<details><summary>Example</summary><pre><code>#define REPEAT\_0(n, x)</code><br/><code>#pragma extension(push, "-fmacro-recursion")</code><br/><code>#define REPEAT\_1(n, x) #!x REPEAT(n, #!x)</code><br/><code>#define REPEAT\_\_(v, n, x) REPEAT\_##v(n, #!x)</code><br/><code>#define REPEAT\_(v, n, x) REPEAT\_\_(v, n, #!x)</code><br/><code>#define REPEAT(n, x) REPEAT\_(\_\_TPP_EVAL((n) > 0), \_\_TPP_EVAL((n)-1), #!x)</code><br/><code>#pragma extension(pop)</code><br/><code>REPEAT(42, HELLO\_WORLD)</code></pre></details> |
| - | TPP | `MACRO_ARGUMENT_WHITESPACE` | `-fmacro-argument-whitespace` | Retain whitespace around macro arguments<details><summary>Example</summary><pre><code>#define STR1(x) #x</code><br/><code>#pragma extension(push, "-fmacro-argument-whitespace")</code><br/><code>#define STR2(x) #x</code><br/><code>#pragma extension(pop)</code><br/><code>STR1(  foo  ) // "foo"</code><br/><code>STR2(  foo  ) // "  foo  "</code></pre></details> |
| - | TPP | `BUILTIN_EXPR_STRINGS` | `-fstrings-in-expressions` | Strings and some new operators in `#if`-directives<details><summary>Example</summary><br/><pre><code>#if "FOO" != "BAR"</code><br/><code>\.\.\.</code><br/><code>#endif</code><br/><code></code><br/><code>#if "FOO"[0] == 'F'</code><br/><code>\.\.\.</code><br/><code>#endif</code><br/><code></code><br/><code>#if "FOO"[1:] == "OO"</code><br/><code>\.\.\.</code><br/><code>#endif</code><br/><code></code><br/><code>#if #"FOO" == 3</code><br/><code>\.\.\.</code><br/><code>#endif</code><br/><code></code></pre></details> |
| `#@` | MSVC | `CHARIZE_MACRO_ARGUMENT` | `-fcharize-macro-argument` | Use `#@` to "charize" arguments in macros<details><summary>Example</summary><pre><code>#define STR(x) #x</code><br/><code>#define CHR(x) #@x</code><br/><code>STR(f) // "f"</code><br/><code>CHR(f) // 'f'</code></pre></details> |
| `#!` | TPP | `DONT_EXPAND_MACRO_ARGUMENT` | `-fdont-expand-macro-argument` | Pass macro argument without expanding it<details><summary>Example</summary><pre><code>#define STR1(x) #x</code><br/><code>#define STR2(x) STR1(x)</code><br/><code>#define STR3(x) STR1(#!x)</code><br/><code>#define FOO 42</code><br/><code>STR1(FOO) // "FOO"</code><br/><code>STR2(FOO) // "42"</code><br/><code>STR3(FOO) // "FOO"</code></pre></details> |
| `__DATE_DAY__`, `__DATE_WDAY__`, `__DATE_YDAY__`, `__DATE_MONTH__`, `__DATE_YEAR__` | TPP | `NUMERIC_DATE_MACROS` | `-fnumeric-date-macros` | Precise date/time macros (behave like `__LINE__`, but expand to components of `__DATE__` in the form of an INT-token) |
| `__TIME_SEC__`, `__TIME_MIN__`, `__TIME_HOUR__` | TPP | `NUMERIC_TIME_MACROS` | `-fnumeric-time-macros` | Precise date/time macros (behave like `__LINE__`, but expand to components of `__TIME__` in the form of an INT-token) |
| `__TPP_EVAL()` | TPP | `MACRO___TPP_EVAL` | `-ftpp-eval-macro` | Evaluate an expression like in `#if`, then expand to its result in the form of 1-2 tokens: `[opt:MINUS][INT]` (or a `"string"` if `TPP_HAVE_BUILTIN_EXPR_STRINGS` is enabled)<br/>Example: `__TPP_EVAL(10 + 20)` expands to `30` |
| `__TPP_EXEC()` | TPP | `MACRO___TPP_EXEC` | `-ftpp-exec-macro` | Takes a string that is then re-interpreted as preprocessor input, and expands to whatever that string expands to. Note that the string may contain any kind of directive<details><summary>Example</summary><pre><code>#define RESOURCE\_SIZE2(...) \_\_VA_NARGS\_\_</code><br/><code>#define RESOURCE\_SIZE1(x)   RESOURCE\_SIZE2(x)</code><br/><code>#define RESOURCE\_SIZE(name) RESOURCE\_SIZE1(\_\_TPP_EXEC("#embed " #name))</code><br/><code>RESOURCE\_SIZE("resource.dat") // Expands to file size in bytes</code></pre>NOTE: If you just want to execute code but discard whatever it expands to, use `#pragma tpp_exec()` instead</details> |
| `__TPP_UNIQUE()` | TPP | `MACRO___TPP_UNIQUE` | `-ftpp-unique-macro` | Called with some keyword/identifier, this macro expands a unique INT-token representative of the given keyword/identifier. The value of that INT-token is distinct from any other keyword/identifier and remains the same for the remainder of input (though it will differ if re-run with different input; this is not a hash-function) |
| `__TPP_LOAD_FILE()` | TPP | `MACRO___TPP_LOAD_FILE` | `-ftpp-load-file-macro` | Same as `#include`, but package the file's entire contents into a string. Could be implemented as:</code><br/><code>`#define __TPP_LOAD_FILE(filename) __TPP_STR_PACK(__TPP_EXEC("#embed " #filename))` |
| `__TPP_COUNTER()` | TPP | `MACRO___TPP_COUNTER` | `-ftpp-counter-macro` | Called the same way as `__TPP_UNIQUE`, but returns an ever-increasing value starting at `0` (same as `__COUNTER__`), but that counter is specific to the given keyword. i.e.: `__TPP_COUNTER(foo)` and `__TPP_COUNTER(bar)` increment different counters |
| `__TPP_RANDOM()` | TPP | `MACRO___TPP_RANDOM` | `-ftpp-random-macro` | Overloaded macro taking 1 or 2 arguments, and expanding to a random number that stays the same during repeated compilations, only changing if source code is altered.<br><ul><li>The single-argument form `__TPP_RANDOM(hi)` expands to a pseudo-random integer token in the range `[0,hi)`</li><li>The 2-argument form `__TPP_RANDOM(lo, hi)` expands to a pseudo-random integer token in the range `[lo,hi)`</li></ul> |
| `__TPP_STR_DECOMPILE()` | TPP | `MACRO___TPP_STR_DECOMPILE` | `-ftpp-str-decompile-macro` | Very similar to `__TPP_EXEC`, except that `__TPP_EXEC` will expand other macros and directives, while this one doesn't: it simply takes a string and expands to its decoded form *without* expanding macros/directives in the process (however: expansion may still occur as returned tokens are read). In practice this usually only means: `__TPP_EXEC` accepts preprocessor directives, `__TPP_STR_DECOMPILE` only does basic string-to-token conversion |
| `__TPP_STR_PACK()` | TPP | `MACRO___TPP_STR_PACK` | `-ftpp-str-pack-macro` | Expands to a string literal that is made up of the arguments taken by this macro. There can be any number of arguments (and arguments don't even have to be separated by `,`), but every argument must be:<ul><li>Another string (that is added to the result after being decoded and re-encoded)</li><li>An INT token (that must evaluate to a value in range `[0,0xFF]`)</li></ul> |

Lots more extensions (especially supported `#pragma` directives) exist and are document in [/doc/config.md](doc/config.md)

## Migrating from TPP2

In order to migrate from TPP2, an additional compatibility-file [tpp2.h](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp2.h) is provided. As an initial step to migrating, you must:

- Add [tpp2.h](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp2.h), as well as [tpp-amalgamation.h](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp-amalgamation.h) and [tpp-amalgamation.c](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp-amalgamation.c) to your project
- Remove TPP2 files from your project: `tpp.c`, `tpp.h`, `tpp-defs.inl` (and, if used: `tpp-gcc-defs.inl`)
- Replace all includes of TPP2's `tpp.h` with the [tpp2.h](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp2.h) from this repository
	- This is primarily meant as a stop-gap measure to easy in transitioning.
	- Eventually, you should remove this file again and include [tpp-amalgamation.h](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp-amalgamation.h) instead
- Replace all includes of TPP2's `tpp.c` with [tpp-amalgamation.c](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp-amalgamation.c), but make sure that [tpp2.h](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp2.h) has also been included (first) at all of those instances
- Try to build your project and handle compilation errors as they appear.
	- Hints on how to migrate some of TPP2's internal constructs that didn't make the cut can be found in comments in [tpp2.h](https://raw.githubusercontent.com/GrieferAtWork/tpp3/refs/heads/master/src/tpp2.h)


## Older versions

- [TPP2: 2017-04-07 -- 2025-12-07](https://github.com/GrieferAtWork/tpp)
- [TPP1: 2015-09-25 -- 2016-10-13](https://sourceforge.net/projects/typp/)
