# Temp MakeFile (for unix users)
# RCS-ID:      $Id$

.SUFFIXES:	.o .obj .c .cpp

# UNIX compilers will be o, windows obj
LIBNAME=magick.a
OBJS = bob.$(OBJ) confbase.$(OBJ) fileconf.$(OBJ) lockable.$(OBJ) log.$(OBJ) \
       magick.$(OBJ) main.$(OBJ) mstream.$(OBJ) mstring.$(OBJ) textfile.$(OBJ) \
       trace.$(OBJ) utils.$(OBJ)

SUBDIRS=antlr bob
LIBS=antlr/antlr.a bob/bob.a

CFLAGS=-O2 -I.././include
PASSTHRU=CC=$(CC) CFLAG=$(CFLAG) LFLAG=$(LFLAG) .SUFFIXES=$(.SUFFIXES) OBJ=$(OBJ)

all: $(LIBNAME)

clean:
	rm -f *.$(OBJ) $(LIBNAME)

$(LIBNAME): $(OBJS)
	@for x in $(SUBDIRS); \
	do	cd $$x; \
		if [ -f ./configure ]; \
		then	./configure; \
		fi; \
		cd $${OLDPWD}; \
		$(MAKE) $(PASSTHRU) -C $$x; \
	done

	@ar rcs $@ $(OBJS) $(LIBS) || \
	( ar rc $@ $(OBJS) $(LIBS) && \
	  ranlib $@ )

.cpp.$(OBJ):
	$(CC) $(CFLAGS) -c $<

bob.$(OBJ): bob.cpp
confbase.$(OBJ): confbase.cpp
fileconf.$(OBJ): fileconf.cpp
lockable.$(OBJ): lockable.cpp
log.$(OBJ): log.cpp
magick.$(OBJ): magick.cpp
main.$(OBJ): main.cpp
mstream.$(OBJ): mstream.cpp
mstring.$(OBJ): mstring.cpp
textfile.$(OBJ): textfile.cpp
trace.$(OBJ): trace.cpp
utils.$(OBJ): utils.cpp
