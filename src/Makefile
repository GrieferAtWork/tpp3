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

.DELETE_ON_ERROR:

CONFIG_MD := \
	../doc/config.md \
	../doc/config-limit.md \
	../doc/config-core.md \
	../doc/config-conf.md \
	../doc/config-token.md \
	../doc/config-value.md \
	../doc/config-warn.md \
	../doc/config-hook.md \
	../doc/config-common.md


all: tpp-amalgamation.h tpp-amalgamation.c $(CONFIG_MD)
.PHONY: all

$(CONFIG_MD): parts/config.h make-config-doc.dee
	deemon make-config-doc.dee

tpp-amalgamation.c: tpp.h tpp.c make-amalgamation.dee $(wildcard parts/*.c) $(wildcard parts/*.h)
	deemon -Wno-usage -Wno-user -F parts/*.h parts/*.c
#	This one must be run last because it depends on the generated contents of other files
	deemon -Wno-usage -Wno-user -F parts/expose-internals.h
	deemon make-amalgamation.dee
tpp-amalgamation.h: tpp-amalgamation.c
