# Microsoft Developer Studio Generated NMAKE File, Based on magick_stampkeys.dsp
!IF "$(CFG)" == ""
CFG=magick_stampkeys - Win32 Debug
!MESSAGE No configuration specified. Defaulting to magick_stampkeys - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "magick_stampkeys - Win32 Release" && "$(CFG)" != "magick_stampkeys - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "magick_stampkeys.mak" CFG="magick_stampkeys - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "magick_stampkeys - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "magick_stampkeys - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "magick_stampkeys - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\magick_stampkeys.exe"


CLEAN :
	-@erase "$(INTDIR)\magick.res"
	-@erase "$(INTDIR)\magick_stampkeys.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\magick_stampkeys.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /Ob2 /I "..\support\zlib-1.1.3" /I "..\support\ACE_wrappers" /I "include" /D "NDEBUG" /D "_CONSOLE" /D "WIN32" /D "_MBCS" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\magick_stampkeys.pch" /YX"pch.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\magick.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\magick_stampkeys.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\magick_stampkeys.pdb" /machine:I386 /out:"$(OUTDIR)\magick_stampkeys.exe" /libpath:"Release" /libpath:"..\support\zlib-1.1.3\Release" /libpath:"..\support\ACE_wrappers\ace" /libpath:"..\support\ACE_wrappers\TAO\tao" 
LINK32_OBJS= \
	"$(INTDIR)\magick_stampkeys.obj" \
	"$(INTDIR)\magick.res"

"$(OUTDIR)\magick_stampkeys.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "magick_stampkeys - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\magick_stampkeys.exe"


CLEAN :
	-@erase "$(INTDIR)\magick.res"
	-@erase "$(INTDIR)\magick_stampkeys.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\magick_stampkeys.exe"
	-@erase "$(OUTDIR)\magick_stampkeys.ilk"
	-@erase "$(OUTDIR)\magick_stampkeys.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "..\support\zlib-1.1.3" /I "..\support\ACE_wrappers" /I "include" /D "_DEBUG" /D "_CONSOLE" /D "HAVE_CONFIG_H" /D "WIN32" /D "_MBCS" /Fp"$(INTDIR)\magick_stampkeys.pch" /YX"pch.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\magick.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\magick_stampkeys.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\magick_stampkeys.pdb" /debug /machine:I386 /out:"$(OUTDIR)\magick_stampkeys.exe" /pdbtype:sept /libpath:"Debug" /libpath:"..\support\zlib-1.1.3\Debug" /libpath:"..\support\ACE_wrappers\ace" /libpath:"..\support\ACE_wrappers\TAO\tao" 
LINK32_OBJS= \
	"$(INTDIR)\magick_stampkeys.obj" \
	"$(INTDIR)\magick.res"

"$(OUTDIR)\magick_stampkeys.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("magick_stampkeys.dep")
!INCLUDE "magick_stampkeys.dep"
!ELSE 
!MESSAGE Warning: cannot find "magick_stampkeys.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "magick_stampkeys - Win32 Release" || "$(CFG)" == "magick_stampkeys - Win32 Debug"
SOURCE=.\src\magick_stampkeys.c

"$(INTDIR)\magick_stampkeys.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\magick.rc

"$(INTDIR)\magick.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

