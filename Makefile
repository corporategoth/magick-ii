#!/usr/bin/make
#
# Temporary UNIX makefile for Magick 2.0
#

##########################################################################
# You must first select where the pre-compiled modules
# are for each of the Magick dependancies. 
##########################################################################

# The Adaptive Communication Environment (ACE) library
# should be installed and available as -lace.

# Z (compression) Library
#ZDIR=../support/zlib-1.1.3
#LIBZ=$(ZDIR)/libz.a

##########################################################################
# Second, you must specify some other system-specific
# flags and veriables magick needs.
##########################################################################

include Makefile.inc

TOPDIR=$${PWD}

# What directories to enter and compile.
SUBDIRS=$(CRYPTDIR)

# --[ Dont edit below this line ]-----------------------------------------

all: rmexec magick

rmexec:
	rm -f magick

magick:
	@for x in $(SUBDIRS) ./src; \
	do	cd $$x; \
		if [ -f ./configure ]; \
		then	./configure; \
		fi; \
		cd $${OLDPWD}; \
		$(MAKE) -C $$x TOPDIR=$(TOPDIR); \
	done
	$(CC) $(LFLAGS) $(LIBS) -o magick
	cp magick magick.debug
	strip magick

clean:
	@for x in $(SUBDIRS) ./src; \
	do	$(MAKE) $(PASSTHRU) -C $$x clean TOPDIR=$(TOPDIR); \
	done
