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
USE("include\base.h", File);
USE("include\version.h-release", File);
USE("include\commserv.h", File);
USE("include\datetime.h", File);
USE("include\dccengine.h", File);
USE("include\filesys.h", File);
USE("include\ircsocket.h", File);
USE("include\language.h", File);
USE("include\lockable.h", File);
USE("include\logfile.h", File);
USE("include\magick.h", File);
USE("include\memoserv.h", File);
USE("include\mstring.h", File);
USE("include\nickserv.h", File);
USE("include\operserv.h", File);
USE("include\pch.h", File);
USE("include\server.h", File);
USE("include\servmsg.h", File);
USE("include\trace.h", File);
USE("include\utils.h", File);
USE("include\variant.h", File);
USE("include\version.h", File);
USE("include\chanserv.h", File);
USELIB("src\xml\xml.lib");
USEUNIT("src\mconfig.cpp");
USE("include\mconfig.h", File);
USEUNIT("src\convert_magick.cpp");
USEUNIT("src\convert_esper.cpp");
USE("include\convert_magick.h", File);
USE("include\convert_esper.h", File);
//---------------------------------------------------------------------------
#endif // defined(__BORLANDC__)

#define WinMain
