####### kdevelop will overwrite this part!!! (begin)##########

magick_INFO = AUTHORS COPYING ChangeLog INSTALL KNOWN-BUGS NEWS README TODO
SUBDIRS = docs src include 
EXTRA_DIST = magick.kdevprj AUTHORS COPYING ChangeLog INSTALL KNOWN-BUGS NEWS README TODO magick.lsm what.c

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
