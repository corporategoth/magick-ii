#if defined(__BORLANDC__)
#include <condefs.h>
USEUNIT("src\base.cpp");
USEUNIT("src\chanserv.cpp");
USEUNIT("src\commserv.cpp");
USEUNIT("src\datetime.cpp");
USEUNIT("src\dccengine.cpp");
USEUNIT("src\ircsocket.cpp");
USEUNIT("src\lockable.cpp");
USEUNIT("src\magick.cpp");
USEUNIT("src\main.cpp");
USEUNIT("src\memoserv.cpp");
USEUNIT("src\mstring.cpp");
USEUNIT("src\nickserv.cpp");
USEUNIT("src\operserv.cpp");
USEUNIT("src\server.cpp");
USEUNIT("src\servmsg.cpp");
USEUNIT("src\trace.cpp");
USEUNIT("src\utils.cpp");
USEUNIT("src\variant.cpp");
USELIB("..\support\zlib\zlib.LIB");
USELIB("..\support\ACE_wrappers\bin\Dynamic\Debug\Pascal\ace_bpd.lib");
USEUNIT("src\filesys.cpp");
USELIB("src\des\crypto.lib");
USELIB("src\xml\xml.lib");
USEUNIT("src\mconfig.cpp");
USEUNIT("src\convert_magick.cpp");
USEUNIT("src\convert_esper.cpp");
//---------------------------------------------------------------------------
#endif // defined(__BORLANDC__)

#define WinMain
