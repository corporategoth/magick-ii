# Microsoft Developer Studio Project File - Name="magick" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
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
# ADD CPP /nologo /MD /W3 /Gi /GX /O2 /I "include\bob" /I "include" /I "..\support\ace_wrappers" /I "..\support\zlib-1.1.3" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "DES_UNROLL" /FR /YX"pch.h" /FD /c
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ace.lib zlib.lib /nologo /subsystem:console /machine:I386 /libpath:"..\support\ace_wrappers\ace" /libpath:"..\support\zlib-1.1.3"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Gf /I "include\bob" /I "include" /I "..\support\ace_wrappers" /I "..\support\zlib-1.1.3" /D "_DEBUG" /D "DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "DES_UNROLL" /D ACE_NTRACE=1 /Fr /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib aced.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\support\ace_wrappers\ace" /libpath:"..\support\zlib-1.1.3"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "magick - Win32 Release"
# Name "magick - Win32 Debug"
# Begin Group "docs"

# PROP Default_Filter ""
# Begin Group "rfcs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\docs\rfcs\rfc1459.txt
# End Source File
# Begin Source File

SOURCE=.\docs\rfcs\rfc854.txt
# End Source File
# Begin Source File

SOURCE=.\docs\rfcs\rfc855.txt
# End Source File
# Begin Source File

SOURCE=.\docs\rfcs\rfc856.txt
# End Source File
# Begin Source File

SOURCE=.\docs\rfcs\rfc857.txt
# End Source File
# Begin Source File

SOURCE=.\docs\rfcs\rfc858.txt
# End Source File
# Begin Source File

SOURCE=.\docs\rfcs\rfc859.txt
# End Source File
# Begin Source File

SOURCE=.\docs\rfcs\rfc860.txt
# End Source File
# Begin Source File

SOURCE=.\docs\rfcs\rfc861.txt
# End Source File
# Begin Source File

SOURCE=.\docs\rfcs\rfc959.txt
# End Source File
# Begin Source File

SOURCE=".\docs\rfcs\The Client-To-Client Protocol (CTCP).txt"
# End Source File
# End Group
# Begin Source File

SOURCE=.\docs\communication.txt
# End Source File
# Begin Source File

SOURCE=.\docs\config.h
# End Source File
# Begin Source File

SOURCE=.\docs\dbase
# End Source File
# Begin Source File

SOURCE=.\docs\develop
# End Source File
# Begin Source File

SOURCE=.\docs\magick.ini
# End Source File
# Begin Source File

SOURCE=.\docs\policy
# End Source File
# Begin Source File

SOURCE=.\docs\requirements
# End Source File
# End Group
# Begin Group "lang files"

# PROP Default_Filter ".lng"
# Begin Source File

SOURCE=.\lang\dark.lng
# End Source File
# Begin Source File

SOURCE=.\lang\dutch.lng
# End Source File
# Begin Source File

SOURCE=.\lang\english.lng
# End Source File
# Begin Source File

SOURCE=.\lang\leet.lng
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\base.cpp
# End Source File
# Begin Source File

SOURCE=.\include\base.h
# End Source File
# Begin Source File

SOURCE=.\src\chanserv.cpp
# End Source File
# Begin Source File

SOURCE=.\include\chanserv.h
# End Source File
# Begin Source File

SOURCE=.\src\confbase.cpp
# End Source File
# Begin Source File

SOURCE=.\include\confbase.h
# End Source File
# Begin Source File

SOURCE=.\src\cryptstream.cpp
# End Source File
# Begin Source File

SOURCE=.\include\cryptstream.h
# End Source File
# Begin Source File

SOURCE=.\src\datetime.cpp
# End Source File
# Begin Source File

SOURCE=.\include\datetime.h
# End Source File
# Begin Source File

SOURCE=.\src\dccengine.cpp
# End Source File
# Begin Source File

SOURCE=.\include\dccengine.h
# End Source File
# Begin Source File

SOURCE=.\src\EscLexer.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\src\escparse.g
# End Source File
# Begin Source File

SOURCE=.\src\EscParser.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\src\fileconf.cpp
# End Source File
# Begin Source File

SOURCE=.\include\fileconf.h
# End Source File
# Begin Source File

SOURCE=.\src\servmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\include\servmsg.h
# End Source File
# Begin Source File

SOURCE=.\src\commserv.cpp
# End Source File
# Begin Source File

SOURCE=.\include\commserv.h
# End Source File
# Begin Source File

SOURCE=.\src\ircsocket.cpp
# End Source File
# Begin Source File

SOURCE=.\include\ircsocket.h
# End Source File
# Begin Source File

SOURCE=.\src\lockable.cpp
# End Source File
# Begin Source File

SOURCE=.\include\lockable.h
# End Source File
# Begin Source File

SOURCE=.\src\log.cpp
# End Source File
# Begin Source File

SOURCE=.\include\log.h
# End Source File
# Begin Source File

SOURCE=.\src\magick.cpp
# End Source File
# Begin Source File

SOURCE=.\include\magick.h
# End Source File
# Begin Source File

SOURCE=.\src\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\memoserv.cpp
# End Source File
# Begin Source File

SOURCE=.\include\memoserv.h
# End Source File
# Begin Source File

SOURCE=.\src\mstream.cpp
# End Source File
# Begin Source File

SOURCE=.\include\mstream.h
# End Source File
# Begin Source File

SOURCE=.\src\mstring.cpp
# End Source File
# Begin Source File

SOURCE=.\include\mstring.h
# End Source File
# Begin Source File

SOURCE=.\src\nickserv.cpp
# End Source File
# Begin Source File

SOURCE=.\include\nickserv.h
# End Source File
# Begin Source File

SOURCE=.\src\operserv.cpp
# End Source File
# Begin Source File

SOURCE=.\include\operserv.h
# End Source File
# Begin Source File

SOURCE=.\include\pch.h
# End Source File
# Begin Source File

SOURCE=.\src\server.cpp
# End Source File
# Begin Source File

SOURCE=.\include\server.h
# End Source File
# Begin Source File

SOURCE=.\src\textfile.cpp
# End Source File
# Begin Source File

SOURCE=.\include\textfile.h
# End Source File
# Begin Source File

SOURCE=.\src\trace.cpp
# End Source File
# Begin Source File

SOURCE=.\include\trace.h
# End Source File
# Begin Source File

SOURCE=.\src\utils.cpp
# End Source File
# Begin Source File

SOURCE=.\include\utils.h
# End Source File
# Begin Source File

SOURCE=.\src\variant.cpp
# End Source File
# Begin Source File

SOURCE=.\include\variant.h
# End Source File
# Begin Source File

SOURCE=.\include\version.h
# End Source File
# End Target
# End Project
