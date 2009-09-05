#EXTRA_DIST=

INCLUDES = -I$(top_builddir)/headers $(gwenhywfar_includes) $(aqfinance_includes)

AM_CFLAGS=@visibility_cflags@

bin_PROGRAMS=aqbanking-cli

if WITH_AQFINANCE

aqbanking_cli_LDADD= \
  $(aqhbci_internal_libs) \
  $(aqbanking_internal_libs) \
  $(gwenhywfar_libs) \
  $(aqfinance_libs)

else

aqbanking_cli_LDADD= \
  $(aqhbci_internal_libs) \
  $(aqbanking_internal_libs) \
  $(gwenhywfar_libs)
endif

aqbanking_cli_SOURCES= \
  main.c
aqbanking_cli_LDFLAGS=
#-Wl,-static

DEFS += -DLOCALEDIR=\"$(localedir)\"

noinst_HEADERS=globals.h
EXTRA_DIST=\
  addtransaction.c \
  chkacc.c \
  chkiban.c \
  debitnote.c \
  debitnotes.c \
  fillgaps.c \
  import.c \
  listaccs.c \
  listbal.c \
  listtrans.c \
  listtransfers.c \
  request.c \
  senddtazv.c \
  transfer.c \
  transfers.c \
  util.c \
  versions.c \
  af_utils.c \
  dbinit.c \
  dbrecontrans.c \
  dblisttrans.c \
  dblisttransfers.c \
  updateconf.c


if IS_WINDOWS

aqbanking_cli_LDFLAGS+=-mconsole

endif

sources:
	for f in $(aqbanking_cli_SOURCES); do \
	  echo $(subdir)/$$f >>$(top_srcdir)/i18nsources; \
	done


built_sources:
	for d in $(SUBDIRS); do \
	  $(MAKE) -C $$d built_sources; \
	done ;
