magick_MANS = magick.pod magick_keygen.pod
EXTRA_DIST = man1/magick.1 man1/magick_keygen.1

POD2MANCMD = pod2man --center="Magick Development Team" --release="$(PACKAGE) $(VERSION)"

all-am:
	if test `which pod2man` ;\
	then	rm -fr man1 ;\
		mkdir man1 ;\
		for x in $(magick_MANS) ;\
		do	echo "$(POD2MANCMD) $$x >man1/`echo $$x | cut -f1 -d.`.1" ;\
		$(POD2MANCMD) $$x >man1/`echo $$x | cut -f1 -d.`.1 ;\
		done ;\
	fi

install-exec-am:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(mandir)
	@list=`ls -1 man?/*.?`; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_DATA) $$p $(DESTDIR)$(mandir)/$$p" ;\
	     $(INSTALL_DATA) $$p $(DESTDIR)$(mandir)/$$p ;\
	  else :; fi; \
	done

uninstall-exec-am:
	@$(NORMAL_UNINSTALL)
	@list=`ls -1 man?/*`; for p in $$list; do \
	  rm -f $(DESTDIR)$(mandir)/$$p ;\
	done
