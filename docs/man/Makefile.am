####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = magick.pod magick_keygen.pod magick_stampkeys.pod
magick_MANS = magick.pod magick_keygen.pod magick_stampkeys.pod

POD2MANPARAM = --center="Magick Development Team" --release="$(PACKAGE) $(VERSION)"

all-am:
	if test -n "`which pod2man`" ;\
	then	if test -r `which pod2man` ;\
	then	if test -x `which pod2man` ;\
	then	rm -fr man1 ;\
		mkdir man1 ;\
		for x in $(magick_MANS) ;\
		do	echo "`which pod2man` $(POD2MANPARAM) $$x >man1/`echo $$x | cut -f1 -d.`.1" ;\
		`which pod2man` $(POD2MANPARAM) $$x >man1/`echo $$x | cut -f1 -d.`.1 ;\
		done ;\
	fi; fi; fi

install-exec-am:
	@$(NORMAL_INSTALL)
	@dirlist=`ls -1d man?`; for d in $$dirlist; do \
	  $(mkinstalldirs) $(DESTDIR)$(mandir)/$$d ;\
	  cd $$d; list=`ls -1 *.?`; for p in $$list; do \
	    echo "  $(INSTALL_DATA) $$p $(DESTDIR)$(mandir)/$$d/$$p" ;\
	     $(INSTALL_DATA) $$p $(DESTDIR)$(mandir)/$$d/$$p ;\
	  done; cd .. ;\
	done

uninstall-exec-am:
	@$(NORMAL_UNINSTALL)
	@list=`ls -1 man?/*`; for p in $$list; do \
	  rm -f $(DESTDIR)$(mandir)/$$p ;\
	done
