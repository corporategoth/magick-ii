####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = en man

####### kdevelop will overwrite this part!!! (end)############

magick_CONF = magick.ini
EXTRA_DIST = magick.ini

install-data-am:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(sysconfdir)
	@list=$(magick_CONF); for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_DATA) $$p $(DESTDIR)$(sysconfdir)/$$p" ;\
	     $(INSTALL_DATA) $$p $(DESTDIR)$(sysconfdir)/$$p ;\
	  else :; fi; \
	done

uninstall-data-am:
	@$(NORMAL_UNINSTALL)
	@list=$(magick_CONF); for p in $$list; do \
	  rm -f $(DESTDIR)$(sysconfdir)/$$p ;\
	done
