####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = index.sgml 
magick_INFO = index.sgml

SGMLCONV    = sgml2html sgml2info sgml2latex sgml2lyx sgml2rtf sgml2txt
SGMLCONVEXT = *.htm* *.info* *.dvi *.lyx *.rtf *.txt

all-am:
	@for x in $(SGMLCONV); do \
		if test -n "`which $$x 2>/dev/null`" ;\
		then	if test -x `which $$x 2>/dev/null` ;\
		then	for y in $(magick_INFO) ;\
			do	echo "`which $$x 2>/dev/null` $$y" ;\
			`which $$x 2>/dev/null` $$y ;\
			done ;\
		fi; fi; \
	done

clean-generic:
	rm -f $(SGMLCONVEXT)

install-exec-am:
	@$(NORMAL_INSTALL)
	@BASE=`basename \`pwd\`` ;\
	$(mkinstalldirs) $(DESTDIR)$(infodir)/magick/$$BASE ;\
	list=`ls -1 $(SGMLCONVEXT) 2>/dev/null`; for p in $$list; do \
	  echo "  $(INSTALL_DATA) $$p $(DESTDIR)$(infodir)/magick/$$BASE/$$p" ;\
	   $(INSTALL_DATA) $$p $(DESTDIR)$(infodir)/magick/$$BASE/$$p ;\
	done

uninstall-exec-am:
	@$(NORMAL_UNINSTALL)
	@BASE=`basename \`pwd\``
	@list=`ls -1 $(SGMLCONVEXT)`; for p in $$list; do \
	  rm -f $(DESTDIR)$(infodir)/magick/$$BASE/$$p ;\
	done
