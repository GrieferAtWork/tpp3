
.DEFAULT_TARGET = all
.PHONY: all

all: ../ctype-unicode.c ../ctype-xml.c

../ctype-unicode.c: ctype-unicode.dee UnicodeData.txt DerivedCoreProperties.txt
	deemon ctype-unicode.dee > ../ctype-unicode.c
../ctype-xml.c: ctype-xml.dee \
		../../external/libiconv/iconvdata/xml-entities \
		../../external/libiconv/iconvdata/xml-entities.dee
	deemon ctype-xml.dee > ../ctype-xml.c



# "--no-check-certificate" is here because (at least right now), the SSL cert that
# the unicode guys are using for "ftp.unicode.org" is only valid for "unicode.org",
# but not for the "ftp." sub-domain...
UnicodeData.txt:
	wget --no-check-certificate https://ftp.unicode.org/Public/UNIDATA/UnicodeData.txt
DerivedCoreProperties.txt:
	wget --no-check-certificate https://ftp.unicode.org/Public/UNIDATA/DerivedCoreProperties.txt
