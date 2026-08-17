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



all: Makefile.autoformat \
	src/tpp-amalgamation.h \
	src/tpp-amalgamation.c \
	src/tpp-emitter-amalgamation.h \
	src/tpp-emitter-amalgamation.c \
	src/tpp-makefile-amalgamation.h \
	src/tpp-makefile-amalgamation.c \
	doc/config.md
.PHONY: all

# Inline code generation self-dependencies...
Makefile.autoformat: src/scripts/Makefile.autoformat.dee src/tpp.h src/tpp.c src/tpp-emitter.h src/tpp-emitter.c src/tpp-makefile.h src/tpp-makefile.c
	deemon src/scripts/Makefile.autoformat.dee > Makefile.autoformat
-include Makefile.autoformat

# The big one: the TPP source amalgamation files...
src/tpp-amalgamation.h: Makefile.autoformat src/tpp.h src/scripts/make-amalgamation.dee
	deemon src/scripts/make-amalgamation.dee src/tpp.h '"tpp-amalgamation.h"' > src/tpp-amalgamation.h
src/tpp-amalgamation.c: Makefile.autoformat src/tpp.c src/scripts/make-amalgamation.dee
	deemon src/scripts/make-amalgamation.dee src/tpp.c 'TPP_AMALGAMATION_H' > src/tpp-amalgamation.c

# Optional TPP component: "emitter"
src/tpp-emitter-amalgamation.h: Makefile.autoformat src/tpp-emitter.h src/scripts/make-amalgamation.dee
	deemon src/scripts/make-amalgamation.dee src/tpp-emitter.h '' > src/tpp-emitter-amalgamation.h
src/tpp-emitter-amalgamation.c: Makefile.autoformat src/tpp-emitter.c src/scripts/make-amalgamation.dee
	deemon src/scripts/make-amalgamation.dee src/tpp-emitter.c '' > src/tpp-emitter-amalgamation.c

# Optional TPP component: "makefile"
src/tpp-makefile-amalgamation.h: Makefile.autoformat src/tpp-makefile.h src/scripts/make-amalgamation.dee
	deemon src/scripts/make-amalgamation.dee src/tpp-makefile.h '' > src/tpp-makefile-amalgamation.h
src/tpp-makefile-amalgamation.c: Makefile.autoformat src/tpp-makefile.c src/scripts/make-amalgamation.dee
	deemon src/scripts/make-amalgamation.dee src/tpp-makefile.c '' > src/tpp-makefile-amalgamation.c

# Generated documentation
doc/config.md: Makefile.autoformat src/parts/.format-cache/config.h src/parts/optional/emitter/config.h src/parts/optional/makefile/config.h src/scripts/make-config-doc.dee src/tpp-amalgamation.h src/tpp-emitter-amalgamation.h src/tpp-makefile-amalgamation.h
	deemon src/scripts/make-config-doc.dee


# Frontend executable
bin/tpp$(EXE): src/tpp-amalgamation.c src/tpp-amalgamation.h src/tpp-emitter-amalgamation.c src/tpp-emitter-amalgamation.h src/tpp-makefile-amalgamation.c src/tpp-makefile-amalgamation.h src/frontend.c
	gcc -Wall -Wextra -Wno-misleading-indentation -DUSE_AMALGAMATION -o bin/tpp$(EXE) src/frontend.c


# Unit tests...
test: bin/tpp$(EXE)
	deemon -F test/_all.h
	@echo bin/tpp$(EXE) --mode=dispose test/_all.h
	@bin/tpp$(EXE) --mode=dispose test/_all.h && echo "[92m✅[m Tests passed" || echo "[91m❌[m TESTS FAILED (exit code: $$?)"
.PHONY: test
