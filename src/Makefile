# Temp MakeFile (for unix users)
# RCS-ID:      $Id$

.SUFFIXES:	.o .obj .c .cpp

# UNIX compilers will be o, windows obj
LIBNAME=magick.a
OBJS = EscLexer.$(OBJ) EscParser.$(OBJ) bob.$(OBJ) chanserv.$(OBJ) confbase.$(OBJ) \
       datetime.$(OBJ) fileconf.$(OBJ) ircsocket.$(OBJ) lockable.$(OBJ) log.$(OBJ) \
       magick.$(OBJ) main.$(OBJ) mstream.$(OBJ) mstring.$(OBJ) textfile.$(OBJ) \
       trace.$(OBJ) utils.$(OBJ) variant.$(OBJ)

SUBDIRS=antlr bob
LIBS=antlr/antlr.a bob/bob.a

CFLAGS=-O2 -I.././include
PASSTHRU=CC=$(CC) CFLAG=$(CFLAG) LFLAG=$(LFLAG) .SUFFIXES=$(.SUFFIXES) OBJ=$(OBJ)

all: SUBS $(LIBNAME)

clean:
	rm -f *.$(OBJ) $(LIBNAME)

SUBS:
	@for x in $(SUBDIRS); \
	do	cd $$x; \
		if [ -f ./configure ]; \
		then	./configure; \
		fi; \
		$(MAKE) $(PASSTHRU); \
		cd $${OLDPWD}; \
	done

$(LIBNAME): $(OBJS)
	@ar rcs $@ $(OBJS) || \
	( ar rc $@ $(OBJS) && \
	  ranlib $@ )
	

.cpp.$(OBJ):
	$(CC) $(CFLAGS) -c $<

EscLexer.$(OBJ): EscLexer.cpp
EscParser.$(OBJ): EscParser.cpp
bob.$(OBJ): bob.cpp
chanserv.$(OBJ): chanserv.cpp
confbase.$(OBJ): confbase.cpp
datetime.$(OBJ): datetime.cpp
fileconf.$(OBJ): fileconf.cpp
ircsocket.$(OBJ): ircsocket.cpp
lockable.$(OBJ): lockable.cpp
log.$(OBJ): log.cpp
magick.$(OBJ): magick.cpp
main.$(OBJ): main.cpp
mstream.$(OBJ): mstream.cpp
mstring.$(OBJ): mstring.cpp
textfile.$(OBJ): textfile.cpp
trace.$(OBJ): trace.cpp
utils.$(OBJ): utils.cpp
variant.$(OBJ): variant.cpp

