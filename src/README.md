# Development notes

TPP builds its main amalgamation files (`tpp-amalgamation.h` and `tpp-amalgamation.c`) from a number of smaller files in `/src/parts`.

Those smaller files in turn have many components to them that are are also generated in the form of `[[[deemon]]]` inline code generation scripts. Furthermore, a couple of those smaller files are also generated as a whole.

Executing the source generation scripts from those smaller files (and building the combinatory amalgamation files) is automated by running `make` in the root directory (one directory above this one)

The final amalgamation files only include the contents of `/*[[[tpp-begin]]]*/ ... /*[[[tpp-end]]]*/` regions of code. Anything outside such regions is *NOT* included in amalgamations. With this in mind, the idea is to make it so TPP can also be compiled in its many-source-files configuration (though this is only done to make development easier as it makes that you don't have to re-generate the amalgamations if you're building from parts)

## Development dependencies

- `gcc` + `make`: TPP is written in C, so these should come as no surprise
- [`deemon`](https://github.com/GrieferAtWork/deemon): Needed to run code generation scripts


## Files / Folders

| File | Description |
| ---- | ----------- |
| `external/*` | External dependencies |
| `parts/*` | Directory containing the individual source files that make up `tpp-amalgamation.h` and `tpp-amalgamation.c` |
| `tpp.h` | Template for `tpp-amalgamation.h` |
| `tpp.c` | Template for `tpp-amalgamation.c` |
| `tpp-amalgamation.h` | Amalgamation header file |
| `tpp-amalgamation.c` | Amalgamation source file |
| `tpp2.h` | Compatibility header for TPP2 (for use with `tpp-amalgamation.h` and `tpp-amalgamation.c`) |
| `makecache.py` | A script to generate a cache file that can be used to skip some startup initialization steps that TPP otherwise needs to do to build builtin keyword/extension/warning tables. See use in [`/samples/simple-with-cache/Makefile`](../samples/simple-with-cache/Makefile) |

