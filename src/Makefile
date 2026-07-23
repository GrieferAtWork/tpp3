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


all: Makefile.autoformat tpp-amalgamation.h tpp-amalgamation.c ../doc/config.md
.PHONY: all

Makefile.autoformat: Makefile.autoformat.dee tpp.h tpp.c
	deemon Makefile.autoformat.dee > Makefile.autoformat

-include Makefile.autoformat

tpp-amalgamation.h: tpp.h make-amalgamation.dee
	deemon make-amalgamation.dee tpp.h > tpp-amalgamation.h
tpp-amalgamation.c: tpp.h make-amalgamation.dee
	deemon make-amalgamation.dee tpp.c > tpp-amalgamation.c
../doc/config.md: parts/format-cache/config.h make-config-doc.dee tpp-amalgamation.h
	deemon make-config-doc.dee

