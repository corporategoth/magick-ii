####### kdevelop will overwrite this part!!! (begin)##########

ircd_CONF = ircd.*.ini
EXTRA_DIST = ircd.*.ini

install-data-am:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(datadir)/magick/ircd
	for p in $(ircd_CONF); do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_DATA) $$p $(DESTDIR)$(datadir)/magick/ircd/$$p" ;\
	     $(INSTALL_DATA) $$p $(DESTDIR)$(datadir)/magick/ircd/$$p ;\
	  else :; fi; \
	done

uninstall-data-am:
	@$(NORMAL_UNINSTALL)
	for p in $(ircd_CONF); do \
	  rm -f $(DESTDIR)$(datadir)/magick/ircd/$$p ;\
	done

clean:
