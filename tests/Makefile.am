AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBSMDEV_DLL_IMPORT@

TESTS = \
	test_api_functions.sh

check_SCRIPTS = \
	test_api_functions.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	smdev_test_get_version

smdev_test_get_version_SOURCES = \
	smdev_test_libsmdev.h \
	smdev_test_libcstring.h \
	smdev_test_get_version.c \
	smdev_test_unused.h

smdev_test_get_version_LDADD = \
	../libsmdev/libsmdev.la \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

