# Microsoft Developer Studio Project File - Name="xml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=xml - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xml.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xml.mak" CFG="xml - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xml - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "xml - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xml - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "xml - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Gf /I "..\..\include" /I "..\..\..\support\ace_wrappers" /I "..\..\..\support\zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "HAVE_CONFIG_H" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "xml - Win32 Release"
# Name "xml - Win32 Debug"
# Begin Source File

SOURCE=..\..\include\xml\asciitab.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\codepage.h
# End Source File
# Begin Source File

SOURCE=.\dllmain.c
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\filemap.h
# End Source File
# Begin Source File

SOURCE=.\gennmtab.c
# End Source File
# Begin Source File

SOURCE=.\hashtable.c
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\hashtable.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\iasciitab.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\latin1tab.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\nametab.h
# End Source File
# Begin Source File

SOURCE=.\sxp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\sxp.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\sxp_data.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\utf8tab.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\xmldef.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\xmlfile.h
# End Source File
# Begin Source File

SOURCE=.\xmlparse.c
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\xmlparse.h
# End Source File
# Begin Source File

SOURCE=.\xmlrole.c
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\xmlrole.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\xmltchar.h
# End Source File
# Begin Source File

SOURCE=.\xmltok.c
# End Source File
# Begin Source File

SOURCE=..\..\include\xml\xmltok.h
# End Source File
# Begin Source File

SOURCE=.\xmltok_impl.c

!IF  "$(CFG)" == "xml - Win32 Release"

!ELSEIF  "$(CFG)" == "xml - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\include\xml\xmltok_impl.h
# End Source File
# Begin Source File

SOURCE=.\xmltok_ns.c

!IF  "$(CFG)" == "xml - Win32 Release"

!ELSEIF  "$(CFG)" == "xml - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
