magick_INFO = index.sgml
EXTRA_DIST = index.html index-1.html index-2.html index-3.html index-4.html index-5.html index-6.html 

SGML2HTMLPARAM = 

all-am:
	if test -n "`which sgml2html`" ;\
	then	if test -r `which sgml2html` ;\
	then	if test -x `which sgml2html` ;\
	then	for x in $(magick_INFO) ;\
		do	echo "`which sgml2html` $(SGML2HTMLPARAM) $$x" ;\
		`which sgml2html` $(SGML2HTMLPARAM) $$x ;\
		done ;\
	fi; fi; fi

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
