# Configuration notes

- All configuration options are evaluated in `#if`-directives:
	- (aside from user-defined hooks and some API functions that can be overwritten by defining an identically-named macro), the state of all configs is defined by the value of its feature-macro in an `#if TPP_HAVE_FOO`-directive


# API Profile: `TPP_PROFILE`

The `TPP_PROFILE` itself has no impact on individual features. It merely defines how features are default-configured. It also does *NOT* affect features that you've configured explicitly: it's merely how TPP will decide . The following profiles exist:

| Profile | Description |
| ------- | ----------- |
| `TPP_PROFILE_DEFAULT` (default) | Default configuration for a pretty good compromise between `TPP_PROFILE_MINIMAL` and `TPP_PROFILE_ALL`. Use this for a nicely feature-packed preprocessor with all conflicting features disabled, in a configuration that doesn't make *too* many assumptions on your behalf.<br/>This is also the default when you don't `#define TPP_PROFILE` before your `#include "tpp-amalgamation.h"` |
| `TPP_PROFILE_MINIMAL` | Disable everything, except dependencies of features you (re-)enabled explicitly. In this configuration, TPP is basically just an over-complicated tokenizer with 90% of its code excluded from compilation. Unless you manually turn features like [`TPP_HAVE_CPP_MACROS`](config-conf.md#tpp_have_cpp_macros), `TPP_HAVE_CPP_DIRECTIVES` and `TPP_HAVE_CPP_DEFINE` back on, you won't even get macros or directives |
| `TPP_PROFILE_ALL` | Turn *everything* on that TPP has to offer, a I do mean **verything**. No feature exists that can't be enabled on way or another when this profile is used, and the default state of individual features sees *most* of them enabled (the exception here being that certain tokens are available, but runtime-disabled by default). Additionally, all features are configured to use extensions rather than features (meaning code can use `#pragma extension` to control pretty much everything TPP has to offer) |
| `TPP_PROFILE_C` | Use this if you're writing a C compiler: pretty much the same as `TPP_PROFILE_DEFAULT`, but some features required for that would otherwise be runtime-configurable become hard-enabled (such as `TPP_HAVE_BSE`) |
| `TPP_PROFILE_CXX` | Use this if you're writing a C++ compiler: same as `TPP_PROFILE_C`, but enables support for some C++-specific tokens like `.*` or `->*` |



# Configuration types

Due to the large number of available configurations, they have been grouped onto the following categories:

- [API Limits / Numeric configs](./config-limit.md)
- [CORE API/System/Lexer Features](./config-core.md)
- [Common configuration groups](./config-common.md)
- [Feature configs](./config-conf.md)
- [Token configs](./config-token.md)
- [Value Configurations](./config-value.md)
- [Warning configuration](./config-warn.md)
- [Hook configuration](./config-hook.md)
