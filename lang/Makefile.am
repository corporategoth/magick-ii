magick_LANG = *.lng *.lfo *.hlp
EXTRA_DIST = *.lng *.lfo *.hlp

install-data-am:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(datadir)/magick/lang
	@list=`ls -1 $(magick_LANG)`; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_DATA) $$p $(DESTDIR)$(datadir)/lang/$$p" ;\
	     $(INSTALL_DATA) $$p $(DESTDIR)$(datadir)/magick/lang/$$p ;\
	  else :; fi; \
	done

uninstall-data-am:
	@$(NORMAL_UNINSTALL)
	@list=`ls -1 $(magick_LANG)`; for p in $$list; do \
	  rm -f $(DESTDIR)$(datadir)/magick/lang/$$p ;\
	done
