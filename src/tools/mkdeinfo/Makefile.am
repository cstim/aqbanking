AM_CPPFLAGS = -I$(top_srcdir)/src/libs -I$(top_builddir)/src/libs $(gwenhywfar_includes) -I$(srcdir)/..




if IS_WINDOWS
# Doesn't link on windows; too lazy to fix this for now.
noinst_PROGRAMS=
else
noinst_PROGRAMS=mkdeinfo
mkdeinfo_SOURCES=mkdeinfo.c
mkdeinfo_LDADD = $(aqbanking_internal_libs) $(gwenhywfar_libs)
endif
# IS_WINDOWS


built_sources:

typefiles:

typedefs:


sources:


cppcheck:
	for f in $(mkdeinfo_SOURCES); do \
	  cppcheck --force $$f ; \
	done
	for d in $(SUBDIRS); do \
	  $(MAKE) -C $$d cppcheck; \
	done

