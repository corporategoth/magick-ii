# Microsoft Developer Studio Project File - Name="xml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

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
!MESSAGE "xml - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "xml - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xml - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lib/xml/Release"
# PROP Intermediate_Dir "lib/xml/Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /Ob2 /I "..\support\zlib" /I "..\support\ACE_wrappers" /I "..\support\ACE_wrappers\TAO" /I "include" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "HAVE_CONFIG_H" /FR /YX"pch.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "xml - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib/xml/Debug"
# PROP Intermediate_Dir "lib/xml/Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\support\zlib" /I "..\support\ACE_wrappers" /I "..\support\ACE_wrappers\TAO" /I "include" /D "_DEBUG" /D "_LIB" /D "HAVE_CONFIG_H" /D "WIN32" /D "_MBCS" /FR /YX"pch.h" /FD /GZ /c
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

# Name "xml - Win32 Release"
# Name "xml - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\xml\sxp.cpp
# End Source File
# Begin Source File

SOURCE=.\src\xml\xmlparse.c
# End Source File
# Begin Source File

SOURCE=.\src\xml\xmlrole.c
# End Source File
# Begin Source File

SOURCE=.\src\xml\xmltok.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\xml\ascii.h
# End Source File
# Begin Source File

SOURCE=.\include\xml\asciitab.h
# End Source File
# Begin Source File

SOURCE=.\include\xml\expat.h
# End Source File
# Begin Source File

SOURCE=.\include\xml\iasciitab.h
# End Source File
# Begin Source File

SOURCE=.\include\xml\latin1tab.h
# End Source File
# Begin Source File

SOURCE=.\include\xml\nametab.h
# End Source File
# Begin Source File

SOURCE=.\include\xml\sxp.h
# End Source File
# Begin Source File

SOURCE=.\include\xml\sxp_data.h
# End Source File
# Begin Source File

SOURCE=.\include\xml\utf8tab.h
# End Source File
# Begin Source File

SOURCE=.\include\xml\xmlconfig.h
# End Source File
# Begin Source File

SOURCE=.\include\xml\xmlrole.h
# End Source File
# Begin Source File

SOURCE=.\include\xml\xmltok.h
# End Source File
# Begin Source File

SOURCE=.\include\xml\xmltok_impl.h
# End Source File
# End Group
# End Target
# End Project
