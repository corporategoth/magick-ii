# RCS-ID:      $Id$

include $(TOPDIR)/Makefile.inc

# UNIX compilers will be o, windows obj
LIBNAME=magick.a
OBJS = EscLexer.$(OBJ) EscParser.$(OBJ) base.$(OBJ) chanserv.$(OBJ) \
       commserv.$(OBJ) confbase.$(OBJ) convert.$(OBJ) cryptstream.$(OBJ) \
       datetime.$(OBJ) dccengine.$(OBJ) fileconf.$(OBJ) filesys.$(OBJ) \
       ircsocket.$(OBJ) lockable.$(OBJ) log.$(OBJ) magick.$(OBJ) main.$(OBJ) \
       memoserv.$(OBJ) mstream.$(OBJ) mstring.$(OBJ) nickserv.$(OBJ) \
       operserv.$(OBJ) server.$(OBJ) servmsg.$(OBJ) textfile.$(OBJ) \
       trace.$(OBJ) utils.$(OBJ) variant.$(OBJ)

SUBDIRS=antlr des expat sxp

OLDPWD=$(TOPDIR)/src
all: SUBS RMLIB $(LIBNAME)

clean:
	@for x in $(SUBDIRS); \
	do	cd $$x; \
		if [ -f ./configure ]; \
		then	./configure; \
		fi; \
		$(MAKE) clean $(PASSTHRU); \
		cd $(OLDPWD); \
	done
	rm -f *.$(OBJ) $(LIBNAME)

SUBS:
	@for x in $(SUBDIRS); \
	do	cd $$x; \
		if [ -f ./configure ]; \
		then	./configure; \
		fi; \
		$(MAKE) $(PASSTHRU); \
		cd $(OLDPWD); \
	done

RMLIB:
	rm -f $(LIBNAME)

$(LIBNAME): $(OBJS)
	@echo "Linking $@ ..."
	@ar rcs $@ $(OBJS) || \
	( ar rc $@ $(OBJS) && \
	  ranlib $@ )
	

.cpp.$(OBJ):
	$(CC) $(CFLAGS) -c $<

EscLexer.$(OBJ): EscLexer.cpp
EscParser.$(OBJ): EscParser.cpp
base.$(OBJ): base.cpp
chanserv.$(OBJ): chanserv.cpp
commserv.$(OBJ): commserv.cpp
confbase.$(OBJ): confbase.cpp
convert.$(OBJ): convert.cpp
cryptstream.$(OBJ): cryptstream.cpp
datetime.$(OBJ): datetime.cpp
dccengine.$(OBJ): dccengine.cpp
fileconf.$(OBJ): fileconf.cpp
filesys.$(OBJ): filesys.cpp
ircsocket.$(OBJ): ircsocket.cpp
lockable.$(OBJ): lockable.cpp
log.$(OBJ): log.cpp
magick.$(OBJ): magick.cpp
main.$(OBJ): main.cpp
memoserv.$(OBJ): memoserv.cpp
mstream.$(OBJ): mstream.cpp
mstring.$(OBJ): mstring.cpp
nickserv.$(OBJ): nickserv.cpp
operserv.$(OBJ): operserv.cpp
server.$(OBJ): server.cpp
servmsg.$(OBJ): servmsg.cpp
textfile.$(OBJ): textfile.cpp
trace.$(OBJ): trace.cpp
utils.$(OBJ): utils.cpp
variant.$(OBJ): variant.cpp
