####### kdevelop will overwrite this part!!! (begin)##########
STAMPED_PROGS = magick magick.debug magick_keygen
bin_PROGRAMS = $(STAMPED_PROGS) magick_stampkeys
magick_stampkeys_SOURCES = magick_stampkeys.c
magick_SOURCES = variant.cpp utils.cpp trace.cpp stages.cpp servmsg.cpp server.cpp operserv.cpp nickserv.cpp mstring.cpp memoserv.cpp main.cpp mconfig.cpp magick.cpp lockable.cpp ircsocket.cpp filesys.cpp dccengine.cpp datetime.cpp convert_magick.cpp convert_esper.cpp convert_epona.cpp commserv.cpp chanserv.cpp base.cpp 
magick_LDADD   = ./xml/libxml.a ./crypt/libcrypt.a
magick_keygen_SOURCES = magick_keygen.c
magick_keygen_LDADD   = ./crypt/libcrypt.a


SUBDIRS = crypt xml 

EXTRA_DIST = base.cpp chanserv.cpp commserv.cpp convert_magick.cpp convert_esper.cpp convert_epona.cpp datetime.cpp dccengine.cpp filesys.cpp ircsocket.cpp lockable.cpp magick.cpp mconfig.cpp main.cpp memoserv.cpp mstring.cpp nickserv.cpp operserv.cpp server.cpp servmsg.cpp stages.cpp trace.cpp utils.cpp variant.cpp magick_keygen.c magick_stampkeys.c

####### kdevelop will overwrite this part!!! (end)############

if STAMPED
STAMP = 1
else
STAMP = 0
endif

mostlyclean-compile:
	-rm -f *.o core *.core *.debug

helper:
	@echo ""
	@echo "======================================================="
	@echo "This will probably take a while ..."
	@echo "May as well go grab your favourite caffiene supplement."
	@echo "======================================================="
if INCREMENT
	$(top_srcdir)/helper/build-ver $(top_srcdir)
endif

magick.debug: helper $(magick_OBJECTS) $(magick_DEPENDENCIES)
	@rm -f magick.debug
	$(CXXLINK) $(magick_LDFLAGS) $(magick_OBJECTS) $(magick_LDADD) $(LIBS)

magick: magick.debug
	@rm -f magick
	cp magick.debug magick
	strip magick

relink:
	@rm -f $(STAMPED_PROGS)
	$(MAKE) $(STAMPED_PROGS)
	@if test $(STAMP) -eq 1 ; then \
	  echo "$(srcdir)/magick_stampkeys $(STAMPED_PROGS)" ;\
	  $(srcdir)/magick_stampkeys $(STAMPED_PROGS) ;\
	else \
	  echo "Creating 'open host' binaries has disabled stamping of the keyfile keys." ;\
	  echo "To stamp these binaries, run:" ;\
	  echo "    $(srcdir)/magick_stampkeys $(STAMPED_PROGS)" ;\
	fi

all-am: Makefile $(PROGRAMS)
	@if test $(STAMP) -eq 1 ; then \
	  echo "$(srcdir)/magick_stampkeys $(STAMPED_PROGS)" ;\
	  $(srcdir)/magick_stampkeys $(STAMPED_PROGS) ;\
	else \
	  echo "Creating 'open host' binaries has disabled stamping of the keyfile keys." ;\
	  echo "To stamp these binaries, run:" ;\
	  echo "    $(srcdir)/magick_stampkeys $(STAMPED_PROGS)" ;\
	fi
