AM_CPPFLAGS = -I$(srcdir)/../.. -I$(srcdir)/.. -I$(top_builddir)/src/libs $(gwenhywfar_includes)

AM_CFLAGS=-DBUILDING_AQBANKING @visibility_cflags@


typefiles= \
  accountqueue.xml \
  userqueue.xml \
  providerqueue.xml \
  jobqueue.xml \
  queue.xml \
  account.xml \
  user.xml \
  swiftdescr.xml



typedatadir=$(aqbanking_pkgdatadir)/typemaker2/c
dist_typedata_DATA=\
  ab_account.tm2 ab_account_list.tm2 ab_account_list2.tm2 \
  ab_accountqueue.tm2 ab_accountqueue_list.tm2 \
  ab_jobqueue.tm2 ab_jobqueue_list.tm2 \
  ab_providerqueue.tm2 ab_providerqueue_list.tm2 \
  ab_queue.tm2 \
  ab_user.tm2 ab_user_list.tm2 ab_user_list2.tm2 \
  ab_userqueue.tm2 ab_userqueue_list.tm2 \
  ab_swift_descr.tm2 ab_swift_descr_list.tm2


# order is important here
typedata_sources= \
  jobqueue.c \
  accountqueue.c \
  userqueue.c \
  providerqueue.c \
  queue.c \
  account.c \
  user.c \
  swiftdescr.c

built_headers_priv= \
  accountqueue_p.h \
  userqueue_p.h \
  providerqueue_p.h \
  jobqueue_p.h \
  queue_p.h \
  account_p.h \
  user_p.h \
  swiftdescr_p.h

built_headers_pub= \
  accountqueue.h \
  userqueue.h \
  providerqueue.h \
  jobqueue.h \
  queue.h \
  account.h \
  user.h \
  swiftdescr.h



built_headers=$(built_headers_priv) $(built_headers_pub)


#iheaderdir=@aqbanking_headerdir_am@/aqbanking
#iheader_HEADERS=$(built_headers_pub)


noinst_HEADERS= $(built_headers_pub)\
  siotlsext.h \
  httpsession.h \
  msgengine.h \
  provider.h \
  provider_be.h \
  bankinfoplugin.h \
  bankinfoplugin_be.h \
  siotlsext_p.h \
  httpsession_p.h \
  msgengine_p.h \
  provider_l.h \
  provider_p.h \
  bankinfoplugin_l.h \
  bankinfoplugin_p.h \
  imexporter_be.h \
  imexporter_l.h \
  imexporter_p.h \
  imexporter.h


noinst_LTLIBRARIES=libabbesupport.la
libabbesupport_la_SOURCES=$(typedata_sources) \
  siotlsext.c \
  httpsession.c \
  msgengine.c \
  provider.c \
  bankinfoplugin.c \
  imexporter.c


extra_sources=\
  provider_account.c \
  provider_accspec.c \
  provider_user.c \
  provider_queues.c \
  provider_utils.c



EXTRA_DIST=$(typefiles) $(typedata_sources) $(built_headers) $(extra_sources)



MAINTAINERCLEANFILES = $(typedata_sources) $(built_headers) $(dist_typedata_DATA)



built_sources: $(typedata_sources)


typefiles: $(typefiles)
	$(typemaker2_exe) -I $(top_srcdir)/src/libs/aqbanking/typemaker2/c -I $(gwenhywfar_dir)/share/gwenhywfar/typemaker2/c -I $(srcdir) -I $(srcdir)/../types --destfolder="$(srcdir)" $(typefiles)

typedefs: $(typefiles)
	$(typemaker2_exe) --defs-only -I $(top_srcdir)/src/libs/aqbanking/typemaker2/c -I $(gwenhywfar_dir)/share/gwenhywfar/typemaker2/c -I $(srcdir) -I $(srcdir)/../types --destfolder="$(srcdir)" $(typefiles)

clean_build_files:
	rm -f $(typedata_sources) $(built_headers)


format_build_files:
	for f in $(typedata_sources) $(built_headers); do \
	  $(ASTYLE) --style=stroustrup -s2 --min-conditional-indent=0 --indent-labels --max-continuation-indent=80 --pad-comma --pad-header --unpad-paren --align-pointer=name --break-closing-braces --break-one-line-headers --attach-return-type --convert-tabs --max-code-length=120 --break-after-logical --suffix=none $$f; \
	done


sources: $(typedata_sources)
	for f in $(libabbesupport_la_SOURCES) $(extra_sources); do \
	  echo $(subdir)/$$f >>$(top_srcdir)/i18nsources; \
	done


cppcheck:
	for f in $(libabbesupport_la_SOURCES); do \
	  cppcheck --force $$f ; \
	done

