# Value Configurations

These configurations all define something other than an enabled/disabled state.

<!--BEGIN:value-->
### TPP_CONFIG_SYSTEM_INCLUDE_PATH

A preprocessor tuple describing the built-in, hard-coded, system-include path.
- The paths specified here are searched in order of specification.
- For information on the full `#include`-path resolution order, see `tpp_include_paths`
- Try not to include trailing slashes in paths hard-coded using this (if TPP3 needs
  trailing slashes in these strings, it will add those itself)

Example:

```c
#define TPP_CONFIG_SYSTEM_INCLUDE_PATH  2("/usr/local/include", "/usr/include")
```

Default: `0()`


### TPP_CONFIG_VALUEOF_STDC_EMBED_NOT_FOUND

String representations of what `__has_embed()`
should expand to when the file wasn't found

Default: `"0"`


### TPP_CONFIG_VALUEOF_STDC_EMBED_FOUND

String representations of what `__has_embed()` should
expand to when the file was found and is non-empty

Default: `"1"`


### TPP_CONFIG_VALUEOF_STDC_EMBED_EMPTY

String representations of what `__has_embed()` should
expand to when the file was found, but is empty

Default: `"2"`


### TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT

Format to use for file+line+column log messages
XXX: Configuration where "TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT"
     can be overwritten at runtime on a per-lexer basis

Default: `defined(_MSC_VER) ? "%Pf(%Pl, %Pc): " : "%Pf:%Pl:%Pc: "`


<!--END:value-->
