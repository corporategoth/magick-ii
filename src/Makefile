# RCS-ID:      $Id$

include $(TOPDIR)/Makefile.inc

# UNIX compilers will be o, windows obj
LIBNAME=magick.a
OBJS = EscLexer.$(OBJ) EscParser.$(OBJ) bob.$(OBJ) chanserv.$(OBJ) confbase.$(OBJ) \
       datetime.$(OBJ) fileconf.$(OBJ) ircsocket.$(OBJ) lockable.$(OBJ) log.$(OBJ) \
       magick.$(OBJ) main.$(OBJ) mstream.$(OBJ) mstring.$(OBJ) textfile.$(OBJ) \
       trace.$(OBJ) utils.$(OBJ) variant.$(OBJ) dccengine.$(OBJ) nickserv.$(OBJ) \
       base.$(OBJ)

SUBDIRS=antlr bob des

all: SUBS RMLIB $(LIBNAME)

clean:
	@for x in $(SUBDIRS); \
	do	cd $$x; \
		if [ -f ./configure ]; \
		then	./configure; \
		fi; \
		$(MAKE) clean $(PASSTHRU); \
		cd $${OLDPWD}; \
	done
	rm -f *.$(OBJ) $(LIBNAME)

SUBS:
	@for x in $(SUBDIRS); \
	do	cd $$x; \
		if [ -f ./configure ]; \
		then	./configure; \
		fi; \
		cd $${OLDPWD}; \
		$(MAKE) -C $$x $(PASSTHRU); \
	done

RMLIB:
	rm -f $(LIBNAME)

$(LIBNAME): $(OBJS)
	ar rcs $@ $(OBJS) || \
	( ar rc $@ $(OBJS) && \
	  ranlib $@ )
	

.cpp.$(OBJ):
	$(CC) $(CFLAGS) -c $<

EscLexer.$(OBJ): EscLexer.cpp
EscParser.$(OBJ): EscParser.cpp
base.$(OBJ): base.cpp
bob.$(OBJ): bob.cpp
chanserv.$(OBJ): chanserv.cpp
confbase.$(OBJ): confbase.cpp
datetime.$(OBJ): datetime.cpp
dccengine.$(OBJ): dccengine.cpp
fileconf.$(OBJ): fileconf.cpp
ircsocket.$(OBJ): ircsocket.cpp
lockable.$(OBJ): lockable.cpp
log.$(OBJ): log.cpp
magick.$(OBJ): magick.cpp
main.$(OBJ): main.cpp
mstream.$(OBJ): mstream.cpp
mstring.$(OBJ): mstring.cpp
nickserv.$(OBJ): nickserv.cpp
textfile.$(OBJ): textfile.cpp
trace.$(OBJ): trace.cpp
utils.$(OBJ): utils.cpp
variant.$(OBJ): variant.cpp
