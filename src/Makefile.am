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
