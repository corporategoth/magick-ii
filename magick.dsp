# Microsoft Developer Studio Project File - Name="magick" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=magick - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "magick.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "magick.mak" CFG="magick - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "magick - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "magick - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "magick - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /Ob2 /I "..\support\zlib-1.1.3" /I "..\support\ACE_wrappers" /I "include" /D "NDEBUG" /D "_CONSOLE" /D "WIN32" /D "_MBCS" /D "HAVE_CONFIG_H" /YX"pch.h" /FD /Zm200 /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 zlib.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"LIBC.LIB" /libpath:"Release" /libpath:"..\support\zlib-1.1.3\Release" /libpath:"..\support\ACE_wrappers\ace" /libpath:"..\support\ACE_wrappers\TAO\tao"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\support\zlib-1.1.3" /I "..\support\ACE_wrappers" /I "include" /D "_DEBUG" /D "_CONSOLE" /D "HAVE_CONFIG_H" /D "WIN32" /D "_MBCS" /FR /YX"pch.h" /FD /Zm200 /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zlibd.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD.LIB" /pdbtype:sept /libpath:"Debug" /libpath:"..\support\zlib-1.1.3\Debug" /libpath:"..\support\ACE_wrappers\ace" /libpath:"..\support\ACE_wrappers\TAO\tao"

!ENDIF 

# Begin Target

# Name "magick - Win32 Release"
# Name "magick - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\base.cpp
# End Source File
# Begin Source File

SOURCE=.\src\chanserv.cpp
# End Source File
# Begin Source File

SOURCE=.\src\commserv.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert_epona.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert_esper.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert_magick.cpp
# End Source File
# Begin Source File

SOURCE=.\src\datetime.cpp
# End Source File
# Begin Source File

SOURCE=.\src\dccengine.cpp
# End Source File
# Begin Source File

SOURCE=.\src\filesys.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ircsocket.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lockable.cpp
# End Source File
# Begin Source File

SOURCE=.\src\magick.cpp
# End Source File
# Begin Source File

SOURCE=.\src\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mconfig.cpp
# End Source File
# Begin Source File

SOURCE=.\src\memoserv.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mstring.cpp
# End Source File
# Begin Source File

SOURCE=.\src\nickserv.cpp
# End Source File
# Begin Source File

SOURCE=.\src\operserv.cpp
# End Source File
# Begin Source File

SOURCE=.\src\server.cpp
# End Source File
# Begin Source File

SOURCE=.\src\servmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\stages.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace.cpp
# End Source File
# Begin Source File

SOURCE=.\src\utils.cpp
# End Source File
# Begin Source File

SOURCE=.\src\variant.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\ace_memory.h
# End Source File
# Begin Source File

SOURCE=.\include\base.h
# End Source File
# Begin Source File

SOURCE=.\include\chanserv.h
# End Source File
# Begin Source File

SOURCE=.\include\commserv.h
# End Source File
# Begin Source File

SOURCE=.\include\config.h
# End Source File
# Begin Source File

SOURCE=.\include\convert_epona.h
# End Source File
# Begin Source File

SOURCE=.\include\convert_esper.h
# End Source File
# Begin Source File

SOURCE=.\include\convert_magick.h
# End Source File
# Begin Source File

SOURCE=.\include\datetime.h
# End Source File
# Begin Source File

SOURCE=.\include\dccengine.h
# End Source File
# Begin Source File

SOURCE=.\include\filesys.h
# End Source File
# Begin Source File

SOURCE=.\include\ircsocket.h
# End Source File
# Begin Source File

SOURCE=.\include\language.h
# End Source File
# Begin Source File

SOURCE=.\include\lockable.h
# End Source File
# Begin Source File

SOURCE=.\include\logfile.h
# End Source File
# Begin Source File

SOURCE=.\include\magick.h
# End Source File
# Begin Source File

SOURCE=.\include\mconfig.h
# End Source File
# Begin Source File

SOURCE=.\include\memoserv.h
# End Source File
# Begin Source File

SOURCE=.\include\mexceptions.h
# End Source File
# Begin Source File

SOURCE=.\include\mstring.h
# End Source File
# Begin Source File

SOURCE=.\include\nickserv.h
# End Source File
# Begin Source File

SOURCE=.\include\operserv.h
# End Source File
# Begin Source File

SOURCE=.\include\pch.h
# End Source File
# Begin Source File

SOURCE=.\include\server.h
# End Source File
# Begin Source File

SOURCE=.\include\servmsg.h
# End Source File
# Begin Source File

SOURCE=.\include\stages.h
# End Source File
# Begin Source File

SOURCE=.\include\trace.h
# End Source File
# Begin Source File

SOURCE=.\include\utils.h
# End Source File
# Begin Source File

SOURCE=.\include\variant.h
# End Source File
# Begin Source File

SOURCE=.\include\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\magick.rc
# End Source File
# Begin Source File

SOURCE=".\include\version.h-release"
# End Source File
# End Group
# End Target
# End Project
