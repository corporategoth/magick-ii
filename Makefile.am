####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = docs src include lang 

EXTRA_DIST = magick.kdevprj AUTHORS COPYING ChangeLog INSTALL README TODO magick.lsm FAQ NEWS acconfig.h KNOWN-BUGS rtfm Magick.spec 

####### kdevelop will overwrite this part!!! (end)############
noinst_PROGRAMS = what
what_SOURCES = what.c

distclean: distclean-recursive
	-rm -f config.status
	-rm -f *.ini *.log *.key *.mnd *.pid magick
	-cp -f Makefile.dist Makefile
	$(MAKE)

install-exec-am:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(datadir)/magick/files/memo
	$(mkinstalldirs) $(DESTDIR)$(datadir)/magick/files/pic
	$(mkinstalldirs) $(DESTDIR)$(datadir)/magick/files/public
	$(mkinstalldirs) $(DESTDIR)$(datadir)/magick/files/temp
	$(mkinstalldirs) $(DESTDIR)$(infodir)/magick
	@list=$(magick_INFO); for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_DATA) $$p $(DESTDIR)$(infodir)/magick/$$p" ;\
	     $(INSTALL_DATA) $$p $(DESTDIR)$(infodir)/magick/$$p ;\
	  else :; fi; \
	done

uninstall-exec-am:
	@$(NORMAL_UNINSTALL)
	@list=$(magick_INFO); for p in $$list; do \
	  rm -f $(DESTDIR)$(infodir)/magick/$$p ;\
	done

ace_uname:
	$(CXX) -o ace_uname ace_uname.cpp
