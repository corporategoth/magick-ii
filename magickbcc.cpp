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
USEUNIT("src\convert.cpp");
USEUNIT("src\filesys.cpp");
USELIB("src\des\crypto.lib");
USEFILE("include\base.h");
USEFILE("include\version.h-release");
USEFILE("include\commserv.h");
USEFILE("include\convert.h");
USEFILE("include\datetime.h");
USEFILE("include\dccengine.h");
USEFILE("include\filesys.h");
USEFILE("include\ircsocket.h");
USEFILE("include\language.h");
USEFILE("include\lockable.h");
USEFILE("include\logfile.h");
USEFILE("include\magick.h");
USEFILE("include\memoserv.h");
USEFILE("include\mstring.h");
USEFILE("include\nickserv.h");
USEFILE("include\operserv.h");
USEFILE("include\pch.h");
USEFILE("include\server.h");
USEFILE("include\servmsg.h");
USEFILE("include\trace.h");
USEFILE("include\utils.h");
USEFILE("include\variant.h");
USEFILE("include\version.h");
USEFILE("include\chanserv.h");
USELIB("src\xml\xml.lib");
USEUNIT("src\mconfig.cpp");
USEFILE("include\mconfig.h");
//---------------------------------------------------------------------------
#endif // defined(__BORLANDC__)

#define WinMain
