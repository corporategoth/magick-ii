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
#ZDIR=../zlib-1.1.3
#LIBZ=$(ZDIR)/libz.a

##########################################################################
# Second, you must specify some other system-specific
# flags and veriables magick needs.
##########################################################################

include Makefile.inc

TOPDIR=/home/admin/prez/projects/Magick/Magick-II

# What directories to enter and compile.
SUBDIRS=$(ZDIR)

# all libraries
LIBS=$(SUBLIBS) $(LIBZ)

# --[ Dont edit below this line ]-----------------------------------------
OLDPWD=$(TOPDIR)

all: rmexec magick

rmexec:
	rm -f magick magick.debug

magick: subdirs link

subdirs:
	helper/build-ver
	@for x in $(SUBDIRS) ./src; \
	do	cd $$x; \
		if [ -f ./configure ]; \
		then	./configure; \
		fi; \
		$(MAKE) TOPDIR=$(TOPDIR); \
		cd $(OLDPWD); \
	done

link:
	gcc $(LFLAGS) $(LIBS) -o magick
	cp magick magick.debug
	strip magick

clean:
	@for x in $(SUBDIRS) ./src; \
	do	cd $$x; \
		$(MAKE) clean TOPDIR=$(TOPDIR); \
		cd $(OLDPWD); \
	done
