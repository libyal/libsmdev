# Note that we cannot use: AUTOMAKE_OPTIONS = subdir-objects
# subdir-objects will compile the source files to a single version of Python.
# Since subdir-objects is being deprecated we copy the source files instead.
am__installdirs = "$(DESTDIR)$(pyexecdir3)"
pyexecdir = $(pyexecdir3)

if HAVE_PYTHON3
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@
	@LIBFGUID_CPPFLAGS@

am_pysmdev_la_rpath = -rpath $(pyexecdir3)

BUILT_SOURCES = \
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

pyexec_LTLIBRARIES = pysmdev.la

nodist_pysmdev_la_SOURCES = $(BUILT_SOURCES)

pysmdev_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libsmdev/libsmdev.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@

pysmdev_la_CPPFLAGS = $(PYTHON3_CPPFLAGS)
pysmdev_la_LDFLAGS  = -module -avoid-version $(PYTHON3_LDFLAGS)

$(BUILT_SOURCES):
	/bin/cp -f $(top_srcdir)/pysmdev/$@ $@

endif

MAINTAINERCLEANFILES = \
	Makefile.in

clean-local:
	/bin/rm -f pysmdev*.[ch]

distclean: clean
	/bin/rm -f Makefile

