#if defined(__BORLANDC__)
#include <condefs.h>
USEUNIT("src\base.cpp");
USEUNIT("src\chanserv.cpp");
USEUNIT("src\commserv.cpp");
USEUNIT("src\confbase.cpp");
USEUNIT("src\cryptstream.cpp");
USEUNIT("src\datetime.cpp");
USEUNIT("src\dccengine.cpp");
USEUNIT("src\EscLexer.cpp");
USEUNIT("src\EscParser.cpp");
USEUNIT("src\fileconf.cpp");
USEUNIT("src\ircsocket.cpp");
USEUNIT("src\lockable.cpp");
USEUNIT("src\log.cpp");
USEUNIT("src\magick.cpp");
USEUNIT("src\main.cpp");
USEUNIT("src\memoserv.cpp");
USEUNIT("src\mstream.cpp");
USEUNIT("src\mstring.cpp");
USEUNIT("src\nickserv.cpp");
USEUNIT("src\operserv.cpp");
USEUNIT("src\server.cpp");
USEUNIT("src\servmsg.cpp");
USEUNIT("src\textfile.cpp");
USEUNIT("src\trace.cpp");
USEUNIT("src\utils.cpp");
USEUNIT("src\variant.cpp");
USELIB("src\des\Debug\crypto.lib");
USELIB("src\rx-1.5\rx\rx\Debug\rxbcc.lib");
USELIB("src\antlr\Debug\antlr.lib");
USELIB("..\support\zlib\zlib.LIB");
USELIB("..\support\ACE_wrappers\bin\Dynamic\Debug\Pascal\ace_bpd.lib");
USEUNIT("src\convert.cpp");
USEUNIT("src\filesys.cpp");
//---------------------------------------------------------------------------
#endif // defined(__BORLANDC__)

#define WinMain
