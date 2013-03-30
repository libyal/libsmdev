AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBSMDEV_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	smdevinfo

smdevinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	info_handle.c info_handle.h \
	smdevinfo.c \
	smdevoutput.c smdevoutput.h \
	smdevtools_libbfio.h \
	smdevtools_libcerror.h \
	smdevtools_libclocale.h \
	smdevtools_libcnotify.h \
	smdevtools_libcstring.h \
	smdevtools_libcsystem.h \
	smdevtools_libsmdev.h \
	smdevtools_libuna.h

smdevinfo_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libsmdev/libsmdev.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on smdevinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(smdevinfo_SOURCES)

