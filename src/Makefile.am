####### kdevelop will overwrite this part!!! (begin)##########
STAMPED_PROGS = magick magick_keygen
bin_PROGRAMS = $(STAMPED_PROGS) magick_stampkeys

magick_stampkeys_SOURCES = magick_stampkeys.c

magick_SOURCES = variant.cpp utils.cpp trace.cpp stages.cpp servmsg.cpp server.cpp operserv.cpp nickserv.cpp nickdata.cpp mstring.cpp memoserv.cpp main.cpp mconfig.cpp magick.cpp lockable.cpp ircsocket.cpp filesys.cpp dccengine.cpp datetime.cpp commserv.cpp chanserv.cpp chandata.cpp base.cpp 
if CONVERT
magick_LDADD   = ./xml/libxml.a ./crypt/libcrypt.a ./convert/libconvert.a
else
magick_LDADD   = ./xml/libxml.a ./crypt/libcrypt.a
endif

magick_keygen_SOURCES = magick_keygen.c
magick_keygen_LDADD   = ./crypt/libcrypt.a

if CONVERT
SUBDIRS = crypt xml convert
else
SUBDIRS = crypt xml
endif

AM_CPPFLAGS = -Wunused
CXXCOMPILE = $(CXX) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(AM_CPPFLAGS) $(CXXFLAGS) $(AM_CXXFLAGS)
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(AM_CPPFLAGS) $(CFLAGS) $(AM_CFLAGS)

EXTRA_DIST = base.cpp chanserv.cpp chandata.cpp commserv.cpp datetime.cpp dccengine.cpp filesys.cpp ircsocket.cpp lockable.cpp magick.cpp mconfig.cpp main.cpp memoserv.cpp mstring.cpp nickserv.cpp nickdata.cpp operserv.cpp server.cpp servmsg.cpp stages.cpp trace.cpp utils.cpp variant.cpp magick_keygen.c magick_stampkeys.c

####### kdevelop will overwrite this part!!! (end)############

PROGRAMS=relink

if STAMPED
STAMP = 1
else
STAMP = 0
endif

mostlyclean-compile:
	-rm -f *.o core *.core *.debug

version_increment:
if INCREMENT
	$(top_srcdir)/helper/build-ver $(top_srcdir)
endif

magick.debug: $(magick_OBJECTS) $(magick_DEPENDENCIES)
	@rm -f $@
	$(CXXLINK) $(magick_LDFLAGS) $(magick_OBJECTS) $(magick_LDADD) $(LIBS)

magick:
	@echo ""
	@echo "======================================================="
	@echo "This will probably take a while ..."
	@echo "May as well go grab your favourite caffiene supplement."
	@echo "======================================================="
	@$(MAKE) $(AM_MAKEFLAGS) magick.debug
	@rm -f $@
	cp magick.debug $@
	strip $@

relink:
	@rm -f $(STAMPED_PROGS)
	@$(MAKE) $(AM_MAKEFLAGS) $(bin_PROGRAMS)
	@if test $(STAMP) -eq 1 ; then \
	  echo "$(srcdir)/magick_stampkeys $(STAMPED_PROGS)" ;\
	  $(srcdir)/magick_stampkeys $(STAMPED_PROGS) ;\
	else \
	  echo "Creating 'open host' binaries has disabled stamping of the keyfile keys." ;\
	  echo "To stamp these binaries, run:" ;\
	  echo "    $(srcdir)/magick_stampkeys $(STAMPED_PROGS)" ;\
	fi

install-binPROGRAMS:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	for p in $(bin_PROGRAMS); do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	     $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done
