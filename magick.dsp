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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\bob" /I "..\cryptlib21" /I "include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ace.lib ..\bob\Release\bob.lib ..\cryptlib21\Release\cl32.lib zlib.lib /nologo /subsystem:console /machine:I386

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
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Gf /I "..\bob" /I "..\cryptlib21" /I "include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib aced.lib ..\bob\Debug\bobd.lib ..\cryptlib21\Debug\cl32d.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "magick - Win32 Release"
# Name "magick - Win32 Debug"
# Begin Source File

SOURCE=.\src\antlr\ANTLRException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\antlr\AST.cpp
# End Source File
# Begin Source File

SOURCE=.\src\antlr\ASTFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\src\antlr\BitSet.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bob.cpp
# End Source File
# Begin Source File

SOURCE=.\include\bob.hpp
# End Source File
# Begin Source File

SOURCE=.\src\antlr\CharBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\antlr\CharScanner.cpp
# End Source File
# Begin Source File

SOURCE=.\src\antlr\CommonToken.cpp
# End Source File
# Begin Source File

SOURCE=.\src\confbase.cpp
# End Source File
# Begin Source File

SOURCE=.\include\confbase.h
# End Source File
# Begin Source File

SOURCE=.\src\ConfigLexer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\configparse.g
# End Source File
# Begin Source File

SOURCE=.\src\ConfigParser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\EscLexer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\EscLexer.hpp
# End Source File
# Begin Source File

SOURCE=.\src\escparse.g
# End Source File
# Begin Source File

SOURCE=.\src\EscParser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\EscParser.hpp
# End Source File
# Begin Source File

SOURCE=.\src\fileconf.cpp
# End Source File
# Begin Source File

SOURCE=.\include\fileconf.h
# End Source File
# Begin Source File

SOURCE=.\src\antlr\LLkParser.cpp
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

SOURCE=.\src\antlr\MismatchedTokenException.cpp
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

SOURCE=.\src\antlr\NoViableAltException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\antlr\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\antlr\ParserException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\antlr\ScannerException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\antlr\String.cpp
# End Source File
# Begin Source File

SOURCE=.\src\textfile.cpp
# End Source File
# Begin Source File

SOURCE=.\include\textfile.h
# End Source File
# Begin Source File

SOURCE=.\docs\todo.txt
# End Source File
# Begin Source File

SOURCE=.\src\antlr\Token.cpp
# End Source File
# Begin Source File

SOURCE=.\src\antlr\TokenBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\antlr\TreeParser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\utils.cpp
# End Source File
# Begin Source File

SOURCE=.\include\utils.h
# End Source File
# End Target
# End Project
