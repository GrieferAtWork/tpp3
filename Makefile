# Copyright (c) 2017-2026 Griefer@Work
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgement (see the following) in the product
#    documentation is required:
#    Portions Copyright (c) 2017-2026 Griefer@Work
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

#.DELETE_ON_ERROR: # Actually: don't turn this on: some of the files here aren't entirely generated, meaning we actually *don't* want to delete them on error

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif



all: Makefile.autoformat src/tpp-amalgamation.h src/tpp-amalgamation.c doc/config.md
.PHONY: all

# Inline code generation self-dependencies...
Makefile.autoformat: src/scripts/Makefile.autoformat.dee src/tpp.h src/tpp.c
	deemon src/scripts/Makefile.autoformat.dee > Makefile.autoformat
-include Makefile.autoformat

# The big one: the TPP source amalgamation files...
src/tpp-amalgamation.h: Makefile.autoformat src/tpp.h src/scripts/make-amalgamation.dee
	deemon src/scripts/make-amalgamation.dee src/tpp.h '"tpp-amalgamation.h"' > src/tpp-amalgamation.h
src/tpp-amalgamation.c: Makefile.autoformat src/tpp.h src/scripts/make-amalgamation.dee
	deemon src/scripts/make-amalgamation.dee src/tpp.c 'TPP_AMALGAMATION_H' > src/tpp-amalgamation.c

# Generated documentation
doc/config.md: Makefile.autoformat src/parts/.format-cache/config.h src/scripts/make-config-doc.dee src/tpp-amalgamation.h
	deemon src/scripts/make-config-doc.dee


# Frontend executable
bin/tpp$(EXE): src/tpp-amalgamation.c src/tpp-amalgamation.h src/frontend.c
	gcc -Wall -Wextra -Wno-misleading-indentation -DUSE_AMALGAMATION -o bin/tpp$(EXE) src/frontend.c


# Unit tests...
test: bin/tpp$(EXE)
	deemon -F test/_all.h
	@echo bin/tpp$(EXE) test/_all.h
	@bin/tpp$(EXE) test/_all.h > /dev/null && echo "Tests passed" || echo "TESTS FAILED"
.PHONY: test
