AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBSMDEV_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	smdevinfo

smdevinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	info_handle.c info_handle.h \
	smdevinfo.c \
	smdevtools_getopt.c smdevtools_getopt.h \
	smdevtools_i18n.h \
	smdevtools_libbfio.h \
	smdevtools_libcerror.h \
	smdevtools_libclocale.h \
	smdevtools_libcnotify.h \
	smdevtools_libsmdev.h \
	smdevtools_libuna.h \
	smdevtools_output.c smdevtools_output.h \
	smdevtools_signal.c smdevtools_signal.h \
	smdevtools_unused.h

smdevinfo_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libsmdev/libsmdev.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on smdevinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(smdevinfo_SOURCES)

