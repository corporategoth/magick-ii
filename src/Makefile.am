####### kdevelop will overwrite this part!!! (begin)##########
noinst_PROGRAMS = genrankeys
bin_PROGRAMS = magick magick_keygen
genrankeys_SOURCES = genrankeys.c
magick_SOURCES = variant.cpp utils.cpp trace.cpp servmsg.cpp server.cpp operserv.cpp nickserv.cpp mstring.cpp memoserv.cpp main.cpp mconfig.cpp magick.cpp lockable.cpp ircsocket.cpp filesys.cpp dccengine.cpp datetime.cpp convert.cpp commserv.cpp chanserv.cpp base.cpp 
magick_LDADD   = ./xml/libxml.a ./des/libdes.a
magick_keygen_SOURCES = magick_keygen.c
magick_keygen_LDADD   = ./des/libdes.a

SUBDIRS = des xml 

EXTRA_DIST = base.cpp chanserv.cpp commserv.cpp convert.cpp datetime.cpp dccengine.cpp filesys.cpp ircsocket.cpp lockable.cpp magick.cpp main.cpp memoserv.cpp mstring.cpp nickserv.cpp operserv.cpp server.cpp servmsg.cpp trace.cpp utils.cpp variant.cpp mconfig.cpp magick_keygen.c genrankeys.c 

####### kdevelop will overwrite this part!!! (end)############

mostlyclean-compile:
	-rm -f *.o core *.core *.debug

helper: genrankeys
	@echo ""
	@echo "======================================================="
	@echo "This will probably take a while ..."
	@echo "May as well go grab your favourite caffeine supplement."
	@echo "======================================================="
	@$(srcdir)/genrankeys $(top_srcdir)/include/crypt.h
	#$(top_srcdir)/helper/build-ver $(top_srcdir)

magick: helper $(magick_OBJECTS) $(magick_DEPENDENCIES)
	@rm -f magick
	$(CXXLINK) $(magick_LDFLAGS) $(magick_OBJECTS) $(magick_LDADD) $(LIBS)
	cp magick magick.debug
	strip magick

all-am: Makefile $(PROGRAMS)
	@rm -f $(top_srcdir)/include/crypt.h

distclean: distclean-recursive
	-cp -f Makefile.am.dist Makefile.am
