#!/usr/bin/make
#
# Temporary UNIX makefile for Magick 2.0
#

##########################################################################
# You must first select where the pre-compiled modules
# are for each of the Magick dependancies. 
##########################################################################

# Adaptive Communication Environment (ACE)
ACEDIR=../support/ACE_wrappers
LIBACE=$(ACEDIR)/libace.a

# Cryptographic Library
CRYPTDIR=../support/cryptlib21
LIBCRYPT=$(CRYPTDIR)/libcrypt.a

# Z (compression) Library
#ZDIR=../support/zlib
#LIBZ=$(ZDIR)/libz.a

##########################################################################
# Second, you must specify some other system-specific
# flags and veriables magick needs.
##########################################################################

# What compiler to use
CC=gcc

# What directories to enter and compile.
SUBDIRS=$(ACEDIR) $(CRYPTDIR)

# Compilation flags to give the compiler
CFLAGS=-O2 -I$${PWD}/./include -I$${PWD}/../support/bob -I$${PWD}/../support/ACE_wrappers/include

# Linking flags to give the compiler
LFLAGS=

# What third-party libraries to include in link
LIBS=$(LIBACE) $(LIBCRYPT) $(LIBZ) ./src/magick.a

# What are source and object files
.SUFFIXES:	.o .obj .cpp .c

# What extension do objects have
OBJ=o

# --[ Dont edit below this line ]-----------------------------------------

PASSTHRU=CC=$(CC) CFLAG=$(CFLAG) LFLAG=$(LFLAG) .SUFFIXES=$(.SUFFIXES) OBJ=$(OBJ)

all: magick

magick:
	rm -f ./include/bob ./include/ace
	ln -s $$PWD/src/bob ./include/bob
	ln -s $${PWD}/$(ACEDIR)/ace ./include/ace
	@for x in $(SUBDIRS) ./src; \
	do	cd $$x; \
		if [ -f ./configure ]; \
		then	./configure; \
		fi; \
		cd $${OLDPWD}; \
		$(MAKE) $(PASSTHRU) -C $$x; \
	done
	$(CC) $(LFLAG) $(LIBS) -o magick

clean:
	@for x in $(SUBDIRS) ./src; \
	do	$(MAKE) $(PASSTHRU) -C $$x clean; \
	done
