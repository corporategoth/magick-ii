# Microsoft Developer Studio Generated NMAKE File, Based on crypt.dsp
!IF "$(CFG)" == ""
CFG=crypt - Win32 Debug
!MESSAGE No configuration specified. Defaulting to crypt - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "crypt - Win32 Release" && "$(CFG)" != "crypt - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "crypt.mak" CFG="crypt - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "crypt - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "crypt - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "crypt - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\crypt.lib"


CLEAN :
	-@erase "$(INTDIR)\bf_enc.obj"
	-@erase "$(INTDIR)\bf_skey.obj"
	-@erase "$(INTDIR)\md5_dgst.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\crypt.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GR /GX /O2 /Ob2 /I "..\support\zlib-1.1.3" /I "..\support\ACE_wrappers" /I "include" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\crypt.pch" /YX"pch.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\crypt.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\crypt.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bf_enc.obj" \
	"$(INTDIR)\bf_skey.obj" \
	"$(INTDIR)\md5_dgst.obj"

"$(OUTDIR)\crypt.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "crypt - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\crypt.lib"


CLEAN :
	-@erase "$(INTDIR)\bf_enc.obj"
	-@erase "$(INTDIR)\bf_skey.obj"
	-@erase "$(INTDIR)\md5_dgst.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\crypt.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\support\zlib-1.1.3" /I "..\support\ACE_wrappers" /I "include" /D "_DEBUG" /D "_LIB" /D "HAVE_CONFIG_H" /D "WIN32" /D "_MBCS" /Fp"$(INTDIR)\crypt.pch" /YX"pch.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\crypt.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\crypt.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bf_enc.obj" \
	"$(INTDIR)\bf_skey.obj" \
	"$(INTDIR)\md5_dgst.obj"

"$(OUTDIR)\crypt.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("crypt.dep")
!INCLUDE "crypt.dep"
!ELSE 
!MESSAGE Warning: cannot find "crypt.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "crypt - Win32 Release" || "$(CFG)" == "crypt - Win32 Debug"
SOURCE=.\src\crypt\bf_enc.c

"$(INTDIR)\bf_enc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\crypt\bf_skey.c

"$(INTDIR)\bf_skey.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\crypt\md5_dgst.c

"$(INTDIR)\md5_dgst.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

