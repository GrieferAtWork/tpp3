
.DEFAULT_TARGET = all
.PHONY: all

all: ../ctype-unicode.c ../ctype-xml.c ../ctype-names.h

../ctype-unicode.c: ctype-unicode.dee UnicodeData.txt DerivedCoreProperties.txt
	deemon ctype-unicode.dee > ../ctype-unicode.c
../ctype-xml.c: ctype-xml.dee \
		../../external/libiconv/iconvdata/xml-entities \
		../../external/libiconv/iconvdata/xml-entities.dee
	deemon ctype-xml.dee > ../ctype-xml.c
../ctype-names.h: ctype-names.dee \
		UnicodeData.txt NameAliases.txt NamedSequences.txt \
		emoji-zwj-sequences.txt emoji-sequences.txt
	deemon ctype-names.dee > ../ctype-names.h



# "--no-check-certificate" is here because (at least right now), the SSL cert that
# the unicode guys are using for "ftp.unicode.org" is only valid for "unicode.org",
# but not for the "ftp." sub-domain...
UnicodeData.txt:
	wget --no-check-certificate https://ftp.unicode.org/Public/UNIDATA/UnicodeData.txt
DerivedCoreProperties.txt:
	wget --no-check-certificate https://ftp.unicode.org/Public/UNIDATA/DerivedCoreProperties.txt
NameAliases.txt:
	wget --no-check-certificate https://ftp.unicode.org/Public/UNIDATA/NameAliases.txt
NamedSequences.txt:
	wget --no-check-certificate https://ftp.unicode.org/Public/UNIDATA/NamedSequences.txt
emoji-zwj-sequences.txt:
	wget --no-check-certificate https://ftp.unicode.org/Public/UCD/latest/emoji/emoji-zwj-sequences.txt
emoji-sequences.txt:
	wget --no-check-certificate https://ftp.unicode.org/Public/UCD/latest/emoji/emoji-sequences.txt
