####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = en man

magick_CONF = magick.ini
EXTRA_DIST = magick.ini

install-data-am:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(sysconfdir)
	for p in $(magick_CONF); do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_DATA) $$p $(DESTDIR)$(sysconfdir)/$$p" ;\
	     $(INSTALL_DATA) $$p $(DESTDIR)$(sysconfdir)/$$p ;\
	  else :; fi; \
	done

uninstall-data-am:
	@$(NORMAL_UNINSTALL)
	for p in $(magick_CONF); do \
	  rm -f $(DESTDIR)$(sysconfdir)/$$p ;\
	done

distclean:  distclean-recursive
	rm -rf devel
