magick_INFO = index.sgml
EXTRA_DIST = index.html index-1.html index-2.html index-3.html index-4.html index-5.html index-6.html 

SGML2HTMLCMD = sgml2html

all-am:
	if test `which sgml2html` ;\
	then	for x in $(magick_INFO) ;\
		do	echo "$(SGML2HTMLCMD) $$x" ;\
		$(SGML2HTMLCMD) $$x ;\
		done ;\
	fi

install-exec-am:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(infodir)/magick
	@list=`ls -1 *.html`; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_DATA) $$p $(DESTDIR)$(infodir)/magick/$$p" ;\
	     $(INSTALL_DATA) $$p $(DESTDIR)$(infodir)/magick/$$p ;\
	  else :; fi; \
	done

uninstall-exec-am:
	@$(NORMAL_UNINSTALL)
	@list=`ls -1 *.html`; for p in $$list; do \
	  rm -f $(DESTDIR)$(infodir)/magick/$$p ;\
	done
