####### kdevelop will overwrite this part!!! (begin)##########
STAMPED_PROGS = magick magick.debug magick_keygen
bin_PROGRAMS = $(STAMPED_PROGS) stampkeys
stampkeys_SOURCES = stampkeys.c
magick_SOURCES = variant.cpp utils.cpp trace.cpp servmsg.cpp server.cpp operserv.cpp nickserv.cpp mstring.cpp memoserv.cpp main.cpp mconfig.cpp magick.cpp lockable.cpp ircsocket.cpp filesys.cpp dccengine.cpp datetime.cpp convert_magick.cpp convert_esper.cpp commserv.cpp chanserv.cpp base.cpp 
magick_LDADD   = ./xml/libxml.a ./des/libdes.a
magick_keygen_SOURCES = magick_keygen.c
magick_keygen_LDADD   = ./des/libdes.a


SUBDIRS = des xml 

EXTRA_DIST = base.cpp chanserv.cpp commserv.cpp convert_magick.cpp convert_esper.cpp datetime.cpp dccengine.cpp filesys.cpp ircsocket.cpp lockable.cpp magick.cpp mconfig.cpp main.cpp memoserv.cpp mstring.cpp nickserv.cpp operserv.cpp server.cpp servmsg.cpp trace.cpp utils.cpp variant.cpp magick_keygen.c stampkeys.c

####### kdevelop will overwrite this part!!! (end)############

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
if STAMPED
	$(srcdir)/stampkeys $(STAMPED_PROGS)
else
	@echo "Creating 'open host' binaries has disabled stamping of the keyfile keys."
	@echo "To stamp these binaries, run:"
	@echo "    $(srcdir)/stampkeys $(STAMPED_PROGS)"
endif

all-am: Makefile $(PROGRAMS)
if STAMPED
	$(srcdir)/stampkeys $(STAMPED_PROGS)
else
	@echo "Creating 'open host' binaries has disabled stamping of the keyfile keys."
	@echo "To stamp these binaries, run:"
	@echo "    $(srcdir)/stampkeys $(STAMPED_PROGS)"
endif
