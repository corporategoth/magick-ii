####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = magick
magick_SOURCES = mconfig.cpp variant.cpp utils.cpp trace.cpp textfile.cpp servmsg.cpp server.cpp operserv.cpp nickserv.cpp mstring.cpp mstream.cpp memoserv.cpp main.cpp magick.cpp lockable.cpp ircsocket.cpp filesys.cpp fileconf.cpp dccengine.cpp datetime.cpp cryptstream.cpp convert.cpp confbase.cpp commserv.cpp chanserv.cpp base.cpp EscParser.cpp EscLexer.cpp 
magick_LDADD   = ./antlr/libantlr.a ./des/libdes.a ./xml/libxml.a 

SUBDIRS = antlr des xml 

EXTRA_DIST = EscLexer.cpp EscLexer.hpp EscLexerTokenTypes.hpp EscParser.cpp EscParser.hpp base.cpp chanserv.cpp commserv.cpp confbase.cpp convert.cpp cryptstream.cpp datetime.cpp dccengine.cpp fileconf.cpp filesys.cpp ircsocket.cpp lockable.cpp magick.cpp main.cpp memoserv.cpp mstream.cpp mstring.cpp nickserv.cpp operserv.cpp server.cpp servmsg.cpp textfile.cpp trace.cpp utils.cpp variant.cpp mconfig.cpp 

####### kdevelop will overwrite this part!!! (end)############

mostlyclean-compile:
	-rm -f *.o core *.core *.debug

helper:
	@echo ""
	@echo "======================================================="
	@echo "This will probably take a while ..."
	@echo "May as well go grab your favourite caffiene supplement."
	@echo "======================================================="
	#$(top_srcdir)/helper/build-ver $(top_srcdir)

magick: helper $(magick_OBJECTS) $(magick_DEPENDENCIES)
	@rm -f magick
	$(CXXLINK) $(magick_LDFLAGS) $(magick_OBJECTS) $(magick_LDADD) $(LIBS)
	cp magick magick.debug
	strip magick
