####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = index.sgml 
magick_INFO = index.sgml

SGML2HTMLPARAM  = 
SGML2INFOPARAM  =
SGML2TXTPARAM   =
SGML2LATEXPARAM =

all-am:
	if test -n "`which sgml2html 2>/dev/null`" ;\
	then	if test -r `which sgml2html 2>/dev/null` ;\
	then	if test -x `which sgml2html 2>/dev/null` ;\
	then	for x in $(magick_INFO) ;\
		do	echo "`which sgml2html 2>/dev/null` $(SGML2HTMLPARAM) $$x" ;\
		`which sgml2html 2>/dev/null` $(SGML2HTMLPARAM) $$x ;\
		done ;\
	fi; fi; fi


	if test -n "`which sgml2latex 2>/dev/null`" ;\
	then	if test -r `which sgml2latex 2>/dev/null` ;\
	then	if test -x `which sgml2latex 2>/dev/null` ;\
	then	if test -n "`which latex 2>/dev/null`" ;\
	then	if test -r `which latex 2>/dev/null` ;\
	then	if test -x `which latex 2>/dev/null` ;\
	then	for x in $(magick_INFO) ;\
		do	echo "`which sgml2latex 2>/dev/null` $(SGML2LATEXPARAM) $$x" ;\
		`which sgml2latex 2>/dev/null` $(SGML2LATEXPARAM) $$x ;\
		done ;\
	fi; fi; fi; fi; fi; fi

	if test -n "`which sgml2info 2>/dev/null`" ;\
	then	if test -r `which sgml2info 2>/dev/null` ;\
	then	if test -x `which sgml2info 2>/dev/null` ;\
	then	for x in $(magick_INFO) ;\
		do	echo "`which sgml2info 2>/dev/null` $(SGML2INFOPARAM) $$x" ;\
		`which sgml2info 2>/dev/null` $(SGML2INFOPARAM) $$x ;\
		done ;\
	fi; fi; fi

	if test -n "`which sgml2txt 2>/dev/null`" ;\
	then	if test -r `which sgml2txt 2>/dev/null` ;\
	then	if test -x `which sgml2txt 2>/dev/null` ;\
	then	for x in $(magick_INFO) ;\
		do	echo "`which sgml2txt 2>/dev/null` $(SGML2TXTPARAM) $$x" ;\
		`which sgml2info 2>/dev/null` $(SGML2TXTPARAM) $$x ;\
		done ;\
	fi; fi; fi

install-exec-am:
	@$(NORMAL_INSTALL)
	@BASE=`basename \`pwd\`` ;\
	$(mkinstalldirs) $(DESTDIR)$(infodir)/magick/$$BASE ;\
	list=`ls -1 *.html* *.latex* *.info* *.txt* 2>/dev/null`; for p in $$list; do \
	  echo "  $(INSTALL_DATA) $$p $(DESTDIR)$(infodir)/magick/$$BASE/$$p" ;\
	   $(INSTALL_DATA) $$p $(DESTDIR)$(infodir)/magick/$$BASE/$$p ;\
	done

uninstall-exec-am:
	@$(NORMAL_UNINSTALL)
	@BASE=`basename \`pwd\``
	@list=`ls -1 *.html* *.latex* *.info* *.txt*`; for p in $$list; do \
	  rm -f $(DESTDIR)$(infodir)/magick/$$BASE/$$p ;\
	done
