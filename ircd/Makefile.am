####### kdevelop will overwrite this part!!! (begin)##########

ircd_CONF = ircd.aurora.ini ircd.bahamut.ini ircd.dal4.4.15.ini ircd.dal.ini ircd.hybrid5.ini ircd.hybrid7.ini ircd.ircu2.10ext.ini ircd.ircu2.10.ini ircd.ircu.ini ircd.rfc1459.ini ircd.rfc1459+ts8.ini ircd.unreal.ini
EXTRA_DIST = ircd.aurora.ini ircd.bahamut.ini ircd.dal4.4.15.ini ircd.dal.ini ircd.hybrid5.ini ircd.hybrid7.ini ircd.ircu2.10ext.ini ircd.ircu2.10.ini ircd.ircu.ini ircd.rfc1459.ini ircd.rfc1459+ts8.ini ircd.unreal.ini

install-data-am:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(datadir)/magick/ircd
	@list=$(ircd_CONF); for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_DATA) $$p $(DESTDIR)$(datadir)/magick/ircd/$$p" ;\
	     $(INSTALL_DATA) $$p $(DESTDIR)$(datadir)/magick/ircd/$$p ;\
	  else :; fi; \
	done

uninstall-data-am:
	@$(NORMAL_UNINSTALL)
	@list=$(ircd_CONF); for p in $$list; do \
	  rm -f $(DESTDIR)$(datadir)/magick/ircd/$$p ;\
	done
