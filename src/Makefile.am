####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = magick
magick_SOURCES = variant.cpp utils.cpp trace.cpp textfile.cpp servmsg.cpp server.cpp operserv.cpp nickserv.cpp mstring.cpp mstream.cpp memoserv.cpp main.cpp magick.cpp log.cpp lockable.cpp ircsocket.cpp filesys.cpp fileconf.cpp dccengine.cpp datetime.cpp cryptstream.cpp convert.cpp confbase.cpp commserv.cpp chanserv.cpp base.cpp EscParser.cpp EscLexer.cpp 
magick_LDADD   = ./antlr/libantlr.a ./des/libdes.a ./xml/libxml.a 

SUBDIRS = antlr des xml 

EXTRA_DIST = EscLexer.cpp EscLexer.hpp EscLexerTokenTypes.hpp EscParser.cpp EscParser.hpp base.cpp chanserv.cpp commserv.cpp confbase.cpp convert.cpp cryptstream.cpp datetime.cpp dccengine.cpp fileconf.cpp filesys.cpp ircsocket.cpp lockable.cpp log.cpp magick.cpp main.cpp memoserv.cpp mstream.cpp mstring.cpp nickserv.cpp operserv.cpp server.cpp servmsg.cpp textfile.cpp trace.cpp utils.cpp variant.cpp 

####### kdevelop will overwrite this part!!! (end)############
SUBDIRS = antlr des xml
LDADD = ./antlr/libantlr.a ./des/libdes.a ./xml/libxml.a
INCLUDES = -I$(top_srcdir)/include
bin_PROGRAMS = magick
magick_SOURCES = \
base.cpp         chanserv.cpp     commserv.cpp     confbase.cpp \
convert.cpp      cryptstream.cpp  datetime.cpp     dccengine.cpp \
EscLexer.cpp     EscParser.cpp    fileconf.cpp     filesys.cpp \
ircsocket.cpp    lockable.cpp     log.cpp          magick.cpp \
main.cpp         memoserv.cpp     mstream.cpp      mstring.cpp \
nickserv.cpp     operserv.cpp     server.cpp       servmsg.cpp \
textfile.cpp     trace.cpp        utils.cpp        variant.cpp
