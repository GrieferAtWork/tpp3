#!/usr/bin/env python
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

# NOTE: This script works with both python2 and python3!

import sys
import os
import threading
from subprocess import Popen, PIPE

MYDIR = os.path.split(__file__)[0]
if not MYDIR:
	MYDIR = "."
if os.path.isabs(MYDIR):
	MYDIR = os.path.relpath(MYDIR, os.getcwd())

# Prevent \r\n on windows + python2
try:
	import msvcrt
	msvcrt.setmode(sys.stdout.fileno(), os.O_BINARY)
except:
	pass


try:
	MY_STDOUT = sys.stdout.buffer
except:
	MY_STDOUT = sys.stdout
try:
	MY_STDERR = sys.stderr.buffer
except:
	MY_STDERR = sys.stderr

def writeln(line):
	MY_STDOUT.write(line.encode("utf-8") + b"\n")

def writeln_stderr(line):
	MY_STDERR.write(line.encode("utf-8") + b"\n")
	MY_STDERR.flush()


def tpp_hashof(s):
	result = 1
	for ch in s:
		o = ord(ch)
		result = (result * 263) + o
	return result

def encode_c_escape(s):
	return s.encode("unicode_escape")

def decode_c_escape(s):
	return s.decode("unicode_escape")

def as_chr(x):
	if isinstance(x, int):
		return chr(x)
	return x

def isalnum(x):
	if isinstance(x, int):
		x = chr(x)
	return x.isalnum()



class TppHashTableItem():
	def __init__(self):
		self.next  = None # TppHashTableItem | None
		self.hash  = 0    # int
		self.key   = ""   # string
		self.value = ""   # string

class TppHashTable():
	def __init__(self):
		self.size  = 0      # int
		self.mask  = 0      # int
		self.table = [None] # {(TppHashTableItem | none)...}

	def _do_add(self, item): # (item: TppHashTableItem)
		index = item.hash & self.mask
		item.next = self.table[index]
		self.table[index] = item

	def _set_mask(self, mask): # (mask: int)
		old_table  = self.table
		self.mask  = mask
		self.table = list([None] * (mask + 1))
		for bucket in old_table:
			while not(bucket is None):
				next = bucket.next
				self._do_add(bucket)
				bucket = next

	def get(self, key): # (key: string): TppHashTableItem
		hash = tpp_hashof(key);
		bucket = self.table[hash & self.mask];
		while not(bucket is None):
			if bucket.hash == hash and bucket.key == key:
				break
			bucket = bucket.next
		return bucket

	def add(self, key, value): # (key: string, value: string)
		if self.size >= self.mask:
			new_mask = (self.mask << 1) | 1
			if new_mask < 15:
				new_mask = 15
			self._set_mask(new_mask)
		new_item = TppHashTableItem()
		new_item.hash  = tpp_hashof(key)
		new_item.key   = key
		new_item.value = value
		self._do_add(new_item)
		self.size = self.size + 1



class AmalgationBuiltins:
	def __init__(self):
		self.keywords     = dict() # {"if": "TPP_KWD_if"}
		self.keyword_keys = []
		self.extensions   = dict() # {"bse": "TPP_EXT_BSE"}
		self.wgroups      = dict() # {"comment": ("TPP_WG_COMMENT", 0)}

	def add(self, kind, args):
		if kind == b"CACHE_KWD": # args: TPP_KWD_if,"if"
			keyword_id  = args.partition(b",")[0].strip().decode("utf-8")
			keyword_str = decode_c_escape(args.partition(b",")[2].strip()[1:-1])
			if keyword_str in self.keywords:
				raise Exception("Duplicate keyword: '" + keyword_str + "'")
			self.keywords[keyword_str] = keyword_id
			self.keyword_keys.append(keyword_str)
		elif kind == b"CACHE_EXTENSION": # args: TPP_EXT_BSE,"bse"
			extension_id  = args.partition(b",")[0].strip().decode("utf-8")
			extension_str = decode_c_escape(args.partition(b",")[2].strip()[1:-1])
			if extension_str in self.extensions:
				raise Exception("Duplicate extension: '-f" + extension_str + "'")
			self.extensions[extension_str] = extension_id
		elif kind == b"CACHE_WGROUP": # args: TPP_WG_COMMENT,0,"comment"
			wgroup_id  = args.partition(b",")[0].strip().decode("utf-8")
			args       = args.partition(b",")[2]
			wgroup_num = int(args.partition(b",")[0].strip())
			wgroup_str = decode_c_escape(args.partition(b",")[2].strip()[1:-1])
			if wgroup_str in self.wgroups:
				raise Exception("Duplicate wgroup: '-W" + wgroup_str + "'")
			self.wgroups[wgroup_str] = (wgroup_id, wgroup_num)

	def generate(self):
		kwds_table = TppHashTable()
		for keyword_str in self.keyword_keys:
			keyword_id = self.keywords[keyword_str]
			kwds_table.add(keyword_str, keyword_id)

		writeln("/* GENERATED FILE -- DO NOT EDIT */")
		writeln("")
		for i in range(kwds_table.mask + 1):
			items = []
			bucket = kwds_table.table[i]
			while not(bucket is None):
				items.append(bucket)
				bucket = bucket.next
			items.reverse()
			for kwd in items:
				line = "TPP_BUILTIN_KEYWORD("
				line = line + kwd.value
				line = line + ", "
				line = line + str(len(kwd.key))
				line = line + ", \""
				line = line + encode_c_escape(kwd.key).decode("utf-8")
				line = line + "\", "
				if kwd.next is None:
					line = line + "NULL"
				else:
					line = line + "&tpp_builtin_keyword_"
					line = line + kwd.next.value
				line = line + ", "
				line = line + hex((kwd.hash >> 32) & 0xffffffff)[2:].rstrip("L").zfill(8).upper()
				line = line + ", "
				line = line + hex((kwd.hash) & 0xffffffff)[2:].rstrip("L").zfill(8).upper()
				line = line + ")"
				writeln(line)
		writeln("")
		writeln("#define TPP_BUILTIN_KEYWORD_COUNT " + str(kwds_table.size))
		writeln("#define TPP_BUILTIN_KEYWORD_MASK  " + str(kwds_table.mask))
		writeln("TPP_BUILTIN_KEYWORD_TABLE(")
		for i in range(kwds_table.mask + 1):
			bucket = kwds_table.table[i]
			if not(bucket is None):
				writeln("	TPP_BUILTIN_KEYWORD_TABLE_ITEM(&tpp_builtin_keyword_" + bucket.value + ")")
			else:
				writeln("	TPP_BUILTIN_KEYWORD_TABLE_ITEM_BLANK");
		writeln(")");
		writeln("");

		extension_strs_sorted = list(self.extensions.keys())
		extension_strs_sorted.sort()
		writeln("TPP_BUILTIN_EXTENSION_NAMES(")
		for extension_str in extension_strs_sorted:
			extension_id = self.extensions[extension_str]
			writeln("	TPP_BUILTIN_EXTENSION_NAME(" + extension_id + ", \"" + \
				encode_c_escape(extension_str).decode("utf-8") + "\")")
		writeln(")")
		writeln("")

		wgroup_strs_sorted = list(self.wgroups.keys())
		wgroup_strs_sorted.sort()
		writeln("TPP_BUILTIN_WARNING_GROUP_NAMES(")
		for wgroup_str in wgroup_strs_sorted:
			wgroup_id, wgroup_num = self.wgroups[wgroup_str]
			writeln("	TPP_BUILTIN_WARNING_GROUP_NAME(" + wgroup_id + ", " + str(wgroup_num) + \
				", \"" + encode_c_escape(wgroup_str).decode("utf-8") + "\")")
		writeln(")")
		MY_STDOUT.flush()


def load_amalgamation_builtins(CC, CFLAGS, AMALGAMATION_H, CONFIG_FILE):
	popen_args = list(CC.split())
	popen_args.extend(CFLAGS)
	popen_args.append("-")
	writeln_stderr("popen_args='" + "' '".join(popen_args) + "'\n")
	proc = Popen(popen_args, stdin=PIPE, stdout=PIPE)
	def write_stdin():
		try:
			proc.stdin.write(b"\
#define TPP_KWD(id,string) CACHE_KWD(id,string)\n\
#define TPP_EXTENSION(id,name,default) CACHE_EXTENSION(id,name)\n\
#define _CACHE_WGROUP_NAME_ITEM(wgroup_id,index,value) CACHE_WGROUP(wgroup_id,index,value)\n\
#define TPP_WGROUP(wgroup_id,names,default) TPP_TUPLE_FOREACH(names,TPP_TUPLE_FOREACH_DUMMY_SEP,_CACHE_WGROUP_NAME_ITEM,wgroup_id)\n\
#undef GUARD_TPP_AMALGAMATION_H\n\
#define TPP_DEFS\n\
#include \"" + encode_c_escape(AMALGAMATION_H) + b"\"\n\
")
			proc.stdin.close()
		except:
			proc.kill()
			raise
	thread = threading.Thread(target=write_stdin, args=())
	thread.start()
	try:
		data = proc.stdout.read()
		proc.stdout.close()
	except:
		proc.kill()
		raise
	proc.wait()
	thread.join()

	i = 0
	data_len = len(data)
	result = AmalgationBuiltins()
	while i < data_len:
		i = data.find(b"CACHE_", i)
		if i < 0:
			break
		entry_end = i + 6
		while entry_end < data_len and as_chr(data[entry_end]).isalnum():
			entry_end = entry_end + 1
		entry_kind = data[i:entry_end]
		while entry_end < data_len and as_chr(data[entry_end]).isspace():
			entry_end = entry_end + 1
		if entry_end >= data_len:
			break
		i = entry_end
		if as_chr(data[entry_end]) == "(":
			i = i + 1
			entry_end = data.find(b")", i)
			if entry_end < 0:
				break
			entry_args = data[i:entry_end]
			result.add(entry_kind, entry_args)
	return result




def printhelp():
	print("Usage: bash makecache.sh [OPTIONS...] [CONFIG_FILE]")
	print("OPTIONS:")
	print("	--cc=*                       Specify the C compiler to use")
	print("	-*                           Extra flags for C compiler")
	print("	--help                       Display this help")
	print("CONFIG_FILE:")
	print("	Filename for C source file containing your #define TPP_* config macros")
	sys.exit(0)

def main(args):
	CC = "gcc -E"
	CFLAGS = []
	AMALGAMATION_H = os.path.join(MYDIR, "tpp-amalgamation.h")
	CONFIG_FILE = None

	while len(args) > 0:
		arg = args[0]
		if arg in ["-help", "--help"]:
			printhelp()
		elif arg.startswith("-cc=") or arg.startswith("--cc="):
			CC = arg.partition("=")[2]
		elif arg == "-cc" or arg == "--cc":
			CC = args[1]
			args = args[1:]
		elif arg.startswith("-"):
			CFLAGS.append(arg)
			if len(args) >= 2 and not args[1].startswith("-"):
				CFLAGS.append(args[1])
				args = args[1:]
		elif len(args) == 1:
			CONFIG_FILE = arg
		else:
			print("Invalid argument '" + arg + "' (run 'python makecache.py --help' for options)")
			sys.exit(1)
		args = args[1:]
	CFLAGS.append("-P")  # Prevent comments from being re-emitted
	if CONFIG_FILE:
		CFLAGS.append("--imacros=" + CONFIG_FILE)
	CFLAGS.append("--imacros=" + AMALGAMATION_H)    # Substitute missing configs
	CFLAGS.append("-iquote")
	CFLAGS.append(MYDIR)
	builtins = load_amalgamation_builtins(CC, CFLAGS, AMALGAMATION_H, CONFIG_FILE)
	builtins.generate()


if __name__ == '__main__':
	main(sys.argv[1:])
