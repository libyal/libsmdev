if HAVE_PYTHON
AM_CFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBSMDEV_DLL_IMPORT@

pyexec_LTLIBRARIES = pysmdev.la

pysmdev_la_SOURCES = \
	pysmdev.c pysmdev.h \
	pysmdev_error.c pysmdev_error.h \
	pysmdev_handle.c pysmdev_handle.h \
	pysmdev_integer.c pysmdev_integer.h \
	pysmdev_libcerror.h \
	pysmdev_libclocale.h \
	pysmdev_libsmdev.h \
	pysmdev_metadata.c pysmdev_metadata.h \
	pysmdev_python.h \
	pysmdev_unused.h

pysmdev_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libsmdev/libsmdev.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@

pysmdev_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pysmdev_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

DISTCLEANFILES = \
	Makefile \
	Makefile.in

