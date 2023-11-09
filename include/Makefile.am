include_HEADERS = \
	libsmdev.h

pkginclude_HEADERS = \
	libsmdev/codepage.h \
	libsmdev/definitions.h \
	libsmdev/error.h \
	libsmdev/extern.h \
	libsmdev/features.h \
	libsmdev/types.h

EXTRA_DIST = \
	libsmdev.h.in \
	libsmdev/definitions.h.in \
	libsmdev/features.h.in \
	libsmdev/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libsmdev.h
	-rm -f libsmdev/definitions.h
	-rm -f libsmdev/features.h
	-rm -f libsmdev/types.h
	-rm -f Makefile

