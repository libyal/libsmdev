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
	/bin/rm -f libsmdev.h
	/bin/rm -f libsmdev/definitions.h
	/bin/rm -f libsmdev/features.h
	/bin/rm -f libsmdev/types.h
	/bin/rm -f Makefile

