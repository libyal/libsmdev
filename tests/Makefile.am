AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBSMDEV_DLL_IMPORT@

BUILT_SOURCES = package.m4

check_AUTOTESTS = \
	test_library \
	test_manpages

if HAVE_PYTHON_TESTS
check_AUTOTESTS += test_python_module
endif

check_SCRIPTS = \
	pysmdev_test_handle.py \
	pysmdev_test_support.py

check_PROGRAMS = \
	smdev_test_ata \
	smdev_test_error \
	smdev_test_handle \
	smdev_test_notify \
	smdev_test_optical_disc \
	smdev_test_scsi \
	smdev_test_sector_range \
	smdev_test_string \
	smdev_test_support \
	smdev_test_track_value

smdev_test_ata_SOURCES = \
	smdev_test_ata.c \
	smdev_test_libcerror.h \
	smdev_test_libcfile.h \
	smdev_test_libsmdev.h \
	smdev_test_macros.h \
	smdev_test_unused.h

smdev_test_ata_LDADD = \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	../libsmdev/libsmdev.la \
	@LIBCERROR_LIBADD@

smdev_test_error_SOURCES = \
	smdev_test_error.c \
	smdev_test_libsmdev.h \
	smdev_test_macros.h \
	smdev_test_unused.h

smdev_test_error_LDADD = \
	../libsmdev/libsmdev.la

smdev_test_handle_SOURCES = \
	smdev_test_functions.c smdev_test_functions.h \
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
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libsmdev/libsmdev.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

smdev_test_notify_SOURCES = \
	smdev_test_libcerror.h \
	smdev_test_libsmdev.h \
	smdev_test_macros.h \
	smdev_test_notify.c \
	smdev_test_unused.h

smdev_test_notify_LDADD = \
	../libsmdev/libsmdev.la \
	@LIBCERROR_LIBADD@

smdev_test_optical_disc_SOURCES = \
	smdev_test_libcerror.h \
	smdev_test_libcfile.h \
	smdev_test_libsmdev.h \
	smdev_test_macros.h \
	smdev_test_optical_disc.c \
	smdev_test_unused.h

smdev_test_optical_disc_LDADD = \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	../libsmdev/libsmdev.la \
	@LIBCERROR_LIBADD@

smdev_test_scsi_SOURCES = \
	smdev_test_libcerror.h \
	smdev_test_libcfile.h \
	smdev_test_libsmdev.h \
	smdev_test_macros.h \
	smdev_test_memory.c smdev_test_memory.h \
	smdev_test_scsi.c \
	smdev_test_unused.h

smdev_test_scsi_LDADD = \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	../libsmdev/libsmdev.la \
	@LIBCERROR_LIBADD@

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

smdev_test_string_SOURCES = \
	smdev_test_libcerror.h \
	smdev_test_libsmdev.h \
	smdev_test_macros.h \
	smdev_test_string.c \
	smdev_test_unused.h

smdev_test_string_LDADD = \
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

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

SUFFIXES = .at

.at:
	$(AM_V_GEN)rm -f $@ $@.tmp; \
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $< && \
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status tests/$@

check-build: $(check_PROGRAMS)

check-local: $(check_AUTOTESTS)
	@fail=0; \
	for test_suite in $(check_AUTOTESTS); do \
		$(AM_TESTS_ENVIRONMENT) $(SHELL) "./$$test_suite" $(TESTSUITEFLAGS) || fail=1; \
		echo ""; \
	done; \
	exit $$fail

clean-local:
	for test_suite in $(check_AUTOTESTS); do \
		test ! -f "$$test_suite" || $(SHELL) "./$$test_suite" --clean; \
	done

package.m4: $(top_srcdir)/configure.ac
	$(AM_V_GEN):; \
	{ \
	  echo 'm4_define([AT_PACKAGE_NAME], [$(PACKAGE_NAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_TARNAME], [$(PACKAGE_TARNAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_VERSION], [$(PACKAGE_VERSION)])' ; \
	  echo 'm4_define([AT_PACKAGE_STRING], [$(PACKAGE_STRING)])' ; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])' ; \
	} > $@

test_library: \
	package.m4 \
	test_library.at \
	test_macros.at

test_manpages: \
	package.m4 \
	test_manpages.at

test_python_module: \
	package.m4 \
	test_python_module.at

EXTRA_DIST = \
	$(check_SCRIPTS) \
	package.m4 \
	test_library.at \
	test_macros.at \
	test_manpages.at \
	test_python_module.at

CLEANFILES = \
	$(check_AUTOTESTS) \
	*.exe \
	*.tmp \
	notify_stream.log

DISTCLEANFILES = \
	*.log \
	atconfig \
	Makefile \
	Makefile.in

