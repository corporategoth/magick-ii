# RCS-ID:      $Id$

include $(TOPDIR)/Makefile.inc

# UNIX compilers will be o, windows obj
LIBNAME=magick.a
OBJS = EscLexer.$(OBJ) EscParser.$(OBJ) chanserv.$(OBJ) confbase.$(OBJ) cryptstream.$(OBJ) \
       datetime.$(OBJ) fileconf.$(OBJ) helpserv.$(OBJ) ircsocket.$(OBJ) lockable.$(OBJ) \
       log.$(OBJ) magick.$(OBJ) main.$(OBJ) memoserv.$(OBJ) mstream.$(OBJ) mstring.$(OBJ) \
       operserv.$(OBJ) textfile.$(OBJ) trace.$(OBJ) utils.$(OBJ) variant.$(OBJ) dccengine.$(OBJ) \
       nickserv.$(OBJ) base.$(OBJ)

SUBDIRS=antlr des rx-1.5

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
confbase.$(OBJ): confbase.cpp
cryptstream.$(OBJ): cryptstream.cpp
datetime.$(OBJ): datetime.cpp
dccengine.$(OBJ): dccengine.cpp
fileconf.$(OBJ): fileconf.cpp
helpserv.$(OBJ): helpserv.cpp
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
textfile.$(OBJ): textfile.cpp
trace.$(OBJ): trace.cpp
utils.$(OBJ): utils.cpp
variant.$(OBJ): variant.cpp
