# Microsoft Developer Studio Project File - Name="convert" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=convert - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "convert.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "convert.mak" CFG="convert - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "convert - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "convert - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "convert - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lib/convert/Release"
# PROP Intermediate_Dir "lib/convert/Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /Ob2 /I "..\support\zlib" /I "..\support\ACE_wrappers" /I "include" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "HAVE_CONFIG_H" /FR /YX"pch.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "convert - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "convert___Win32_Debug"
# PROP BASE Intermediate_Dir "convert___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib/convert/Debug"
# PROP Intermediate_Dir "lib/convert/Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\support\zlib" /I "..\support\ACE_wrappers" /I "include" /D "_DEBUG" /D "_LIB" /D "HAVE_CONFIG_H" /D "WIN32" /D "_MBCS" /FR /YX"pch.h" /FD /GZ  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "convert - Win32 Release"
# Name "convert - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\convert\auspice.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert\bolivia.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert\cygnus.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert\epona.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert\hybserv.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert\interface.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert\ircservices.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert\magick.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert\ptlink.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert\sirv.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert\trircd.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert\wrecked.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\convert\auspice.h
# End Source File
# Begin Source File

SOURCE=.\include\convert\bolivia.h
# End Source File
# Begin Source File

SOURCE=.\include\convert\cygnus.h
# End Source File
# Begin Source File

SOURCE=.\include\convert\epona.h
# End Source File
# Begin Source File

SOURCE=.\include\convert\hybserv.h
# End Source File
# Begin Source File

SOURCE=.\include\convert\interface.h
# End Source File
# Begin Source File

SOURCE=.\include\convert\ircservices.h
# End Source File
# Begin Source File

SOURCE=.\include\convert\magick.h
# End Source File
# Begin Source File

SOURCE=.\include\convert\ptlink.h
# End Source File
# Begin Source File

SOURCE=.\include\convert\sirv.h
# End Source File
# Begin Source File

SOURCE=.\include\convert\trircd.h
# End Source File
# Begin Source File

SOURCE=.\include\convert\wrecked.h
# End Source File
# End Group
# End Target
# End Project
