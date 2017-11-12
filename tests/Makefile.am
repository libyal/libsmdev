AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBSMDEV_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYSMDEV = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_smdevinfo.sh \
	$(TESTS_PYSMDEV)

check_SCRIPTS = \
	pysmdev_test_handle.py \
	pysmdev_test_support.py \
	test_library.sh \
	test_python_module.sh \
	test_runner.sh \
	test_smdevinfo.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	smdev_test_error \
	smdev_test_handle \
	smdev_test_notify \
	smdev_test_sector_range \
	smdev_test_support \
	smdev_test_track_value

smdev_test_error_SOURCES = \
	smdev_test_error.c \
	smdev_test_libsmdev.h \
	smdev_test_macros.h \
	smdev_test_unused.h

smdev_test_error_LDADD = \
	../libsmdev/libsmdev.la

smdev_test_handle_SOURCES = \
	smdev_test_functions.c smdev_test_functions.h \
	smdev_test_getopt.c smdev_test_getopt.h \
	smdev_test_handle.c \
	smdev_test_libcerror.h \
	smdev_test_libclocale.h \
	smdev_test_libcnotify.h \
	smdev_test_libsmdev.h \
	smdev_test_libuna.h \
	smdev_test_macros.h \
	smdev_test_memory.c smdev_test_memory.h \
	smdev_test_unused.h

smdev_test_handle_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libsmdev/libsmdev.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

smdev_test_notify_SOURCES = \
	smdev_test_libsmdev.h \
	smdev_test_macros.h \
	smdev_test_notify.c \
	smdev_test_unused.h

smdev_test_notify_LDADD = \
	../libsmdev/libsmdev.la

smdev_test_sector_range_SOURCES = \
	smdev_test_libcerror.h \
	smdev_test_libsmdev.h \
	smdev_test_macros.h \
	smdev_test_memory.c smdev_test_memory.h \
	smdev_test_sector_range.c \
	smdev_test_unused.h

smdev_test_sector_range_LDADD = \
	../libsmdev/libsmdev.la \
	@LIBCERROR_LIBADD@

smdev_test_support_SOURCES = \
	smdev_test_libcerror.h \
	smdev_test_libsmdev.h \
	smdev_test_macros.h \
	smdev_test_support.c \
	smdev_test_unused.h

smdev_test_support_LDADD = \
	../libsmdev/libsmdev.la \
	@LIBCERROR_LIBADD@

smdev_test_track_value_SOURCES = \
	smdev_test_libcerror.h \
	smdev_test_libsmdev.h \
	smdev_test_macros.h \
	smdev_test_memory.c smdev_test_memory.h \
	smdev_test_track_value.c \
	smdev_test_unused.h

smdev_test_track_value_LDADD = \
	../libsmdev/libsmdev.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

