magick_MANS = magick.pod magick_keygen.pod
EXTRA_DIST = magick.1 magick_keygen.1

POD2MANCMD = pod2man --center="Magick Development Team" --release="$(PACKAGE) $(VERSION)"

all-am:
	if test `which pod2man` ;\
	then	rm -fr man1 ;\
		mkdir man1 ;\
		$(POD2MANCMD) magick.pod >man1/magick.1 ;\
		$(POD2MANCMD) magick_keygen.pod >man1/magick_keygen.1 ;\
	fi
