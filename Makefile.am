####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = docs src include 

EXTRA_DIST = magick.kdevprj AUTHORS COPYING ChangeLog INSTALL README TODO magick.lsm what.c

####### kdevelop will overwrite this part!!! (end)############
noinst_PROGRAMS = what
what_SOURCES = what.c

distclean: distclean-recursive
	-rm -f config.status
	-rm -f *.ini *.log *.key *.mnd *.pid magick
	-cp -f Makefile.dist Makefile
	$(MAKE)
