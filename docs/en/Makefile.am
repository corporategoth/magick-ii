magick_INFO = index.sgml
EXTRA_DIST = index.html index-1.html index-2.html index-3.html index-4.html index-5.html index-6.html index.latex index.info index.txt

SGML2HTMLPARAM  = 
SGML2INFOPARAM  =
SGML2TXTPARAM   =
SGML2LATEXPARAM =

all-am:
	if test -n "`which sgml2html`" ;\
	then	if test -r `which sgml2html` ;\
	then	if test -x `which sgml2html` ;\
	then	for x in $(magick_INFO) ;\
		do	echo "`which sgml2html` $(SGML2HTMLPARAM) $$x" ;\
		`which sgml2html` $(SGML2HTMLPARAM) $$x ;\
		done ;\
	fi; fi; fi


	if test -n "`which sgml2latex`" ;\
	then	if test -r `which sgml2latex` ;\
	then	if test -x `which sgml2latex` ;\
	then	if test -n "`which latex`" ;\
	then	if test -r `which latex` ;\
	then	if test -x `which latex` ;\
	then	for x in $(magick_INFO) ;\
		do	echo "`which sgml2latex` $(SGML2LATEXPARAM) $$x" ;\
		`which sgml2latex` $(SGML2LATEXPARAM) $$x ;\
		done ;\
	fi; fi; fi; fi; fi; fi

	if test -n "`which sgml2info`" ;\
	then	if test -r `which sgml2info` ;\
	then	if test -x `which sgml2info` ;\
	then	for x in $(magick_INFO) ;\
		do	echo "`which sgml2info` $(SGML2INFOPARAM) $$x" ;\
		`which sgml2info` $(SGML2INFOPARAM) $$x ;\
		done ;\
	fi; fi; fi

	if test -n "`which sgml2txt`" ;\
	then	if test -r `which sgml2txt` ;\
	then	if test -x `which sgml2txt` ;\
	then	for x in $(magick_INFO) ;\
		do	echo "`which sgml2txt` $(SGML2TXTPARAM) $$x" ;\
		`which sgml2info` $(SGML2TXTPARAM) $$x ;\
		done ;\
	fi; fi; fi

install-exec-am:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(infodir)/magick
	@list=`ls -1 *.html *.latex *.info *.txt`; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_DATA) $$p $(DESTDIR)$(infodir)/magick/$$p" ;\
	     $(INSTALL_DATA) $$p $(DESTDIR)$(infodir)/magick/$$p ;\
	  else :; fi; \
	done

uninstall-exec-am:
	@$(NORMAL_UNINSTALL)
	@list=`ls -1 *.html *.latex *.info *.txt`; for p in $$list; do \
	  rm -f $(DESTDIR)$(infodir)/magick/$$p ;\
	done
