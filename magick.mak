# Microsoft Developer Studio Generated NMAKE File, Based on magick.dsp
!IF "$(CFG)" == ""
CFG=magick - Win32 Debug
!MESSAGE No configuration specified. Defaulting to magick - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "magick - Win32 Release" && "$(CFG)" != "magick - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "magick - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\magick.exe"

!ELSE 

ALL : "xml - Win32 Release" "crypt - Win32 Release" "$(OUTDIR)\magick.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"crypt - Win32 ReleaseCLEAN" "xml - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\base.obj"
	-@erase "$(INTDIR)\chandata.obj"
	-@erase "$(INTDIR)\chanserv.obj"
	-@erase "$(INTDIR)\commserv.obj"
	-@erase "$(INTDIR)\convert_epona.obj"
	-@erase "$(INTDIR)\convert_esper.obj"
	-@erase "$(INTDIR)\convert_magick.obj"
	-@erase "$(INTDIR)\datetime.obj"
	-@erase "$(INTDIR)\dccengine.obj"
	-@erase "$(INTDIR)\filesys.obj"
	-@erase "$(INTDIR)\ircsocket.obj"
	-@erase "$(INTDIR)\lockable.obj"
	-@erase "$(INTDIR)\magick.obj"
	-@erase "$(INTDIR)\magick.res"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\mconfig.obj"
	-@erase "$(INTDIR)\memoserv.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\nickdata.obj"
	-@erase "$(INTDIR)\nickserv.obj"
	-@erase "$(INTDIR)\operserv.obj"
	-@erase "$(INTDIR)\server.obj"
	-@erase "$(INTDIR)\servmsg.obj"
	-@erase "$(INTDIR)\stages.obj"
	-@erase "$(INTDIR)\trace.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\variant.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\magick.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GR /GX /O2 /Ob2 /I "..\support\zlib-1.1.3" /I "..\support\ACE_wrappers" /I "include" /D "NDEBUG" /D "_CONSOLE" /D "WIN32" /D "_MBCS" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\magick.pch" /YX"pch.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /Zm200 /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\magick.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=zlib.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\magick.pdb" /machine:I386 /nodefaultlib:"LIBC.LIB" /out:"$(OUTDIR)\magick.exe" /libpath:"Release" /libpath:"..\support\zlib-1.1.3\Release" /libpath:"..\support\ACE_wrappers\ace" /libpath:"..\support\ACE_wrappers\TAO\tao" 
LINK32_OBJS= \
	"$(INTDIR)\base.obj" \
	"$(INTDIR)\chanserv.obj" \
	"$(INTDIR)\chandata.obj" \
	"$(INTDIR)\commserv.obj" \
	"$(INTDIR)\convert_epona.obj" \
	"$(INTDIR)\convert_esper.obj" \
	"$(INTDIR)\convert_magick.obj" \
	"$(INTDIR)\datetime.obj" \
	"$(INTDIR)\dccengine.obj" \
	"$(INTDIR)\filesys.obj" \
	"$(INTDIR)\ircsocket.obj" \
	"$(INTDIR)\lockable.obj" \
	"$(INTDIR)\magick.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\mconfig.obj" \
	"$(INTDIR)\memoserv.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\nickserv.obj" \
	"$(INTDIR)\nickdata.obj" \
	"$(INTDIR)\operserv.obj" \
	"$(INTDIR)\server.obj" \
	"$(INTDIR)\servmsg.obj" \
	"$(INTDIR)\stages.obj" \
	"$(INTDIR)\trace.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\variant.obj" \
	"$(INTDIR)\magick.res" \
	"$(OUTDIR)\crypt.lib" \
	"$(OUTDIR)\xml.lib"

"$(OUTDIR)\magick.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\magick.exe" "$(OUTDIR)\magick.bsc"

!ELSE 

ALL : "xml - Win32 Debug" "crypt - Win32 Debug" "$(OUTDIR)\magick.exe" "$(OUTDIR)\magick.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"crypt - Win32 DebugCLEAN" "xml - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\base.obj"
	-@erase "$(INTDIR)\base.sbr"
	-@erase "$(INTDIR)\chandata.obj"
	-@erase "$(INTDIR)\chandata.sbr"
	-@erase "$(INTDIR)\chanserv.obj"
	-@erase "$(INTDIR)\chanserv.sbr"
	-@erase "$(INTDIR)\commserv.obj"
	-@erase "$(INTDIR)\commserv.sbr"
	-@erase "$(INTDIR)\convert_epona.obj"
	-@erase "$(INTDIR)\convert_epona.sbr"
	-@erase "$(INTDIR)\convert_esper.obj"
	-@erase "$(INTDIR)\convert_esper.sbr"
	-@erase "$(INTDIR)\convert_magick.obj"
	-@erase "$(INTDIR)\convert_magick.sbr"
	-@erase "$(INTDIR)\datetime.obj"
	-@erase "$(INTDIR)\datetime.sbr"
	-@erase "$(INTDIR)\dccengine.obj"
	-@erase "$(INTDIR)\dccengine.sbr"
	-@erase "$(INTDIR)\filesys.obj"
	-@erase "$(INTDIR)\filesys.sbr"
	-@erase "$(INTDIR)\ircsocket.obj"
	-@erase "$(INTDIR)\ircsocket.sbr"
	-@erase "$(INTDIR)\lockable.obj"
	-@erase "$(INTDIR)\lockable.sbr"
	-@erase "$(INTDIR)\magick.obj"
	-@erase "$(INTDIR)\magick.res"
	-@erase "$(INTDIR)\magick.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\mconfig.obj"
	-@erase "$(INTDIR)\mconfig.sbr"
	-@erase "$(INTDIR)\memoserv.obj"
	-@erase "$(INTDIR)\memoserv.sbr"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mstring.sbr"
	-@erase "$(INTDIR)\nickdata.obj"
	-@erase "$(INTDIR)\nickdata.sbr"
	-@erase "$(INTDIR)\nickserv.obj"
	-@erase "$(INTDIR)\nickserv.sbr"
	-@erase "$(INTDIR)\operserv.obj"
	-@erase "$(INTDIR)\operserv.sbr"
	-@erase "$(INTDIR)\server.obj"
	-@erase "$(INTDIR)\server.sbr"
	-@erase "$(INTDIR)\servmsg.obj"
	-@erase "$(INTDIR)\servmsg.sbr"
	-@erase "$(INTDIR)\stages.obj"
	-@erase "$(INTDIR)\stages.sbr"
	-@erase "$(INTDIR)\trace.obj"
	-@erase "$(INTDIR)\trace.sbr"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\utils.sbr"
	-@erase "$(INTDIR)\variant.obj"
	-@erase "$(INTDIR)\variant.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\magick.bsc"
	-@erase "$(OUTDIR)\magick.exe"
	-@erase "$(OUTDIR)\magick.ilk"
	-@erase "$(OUTDIR)\magick.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\support\zlib-1.1.3" /I "..\support\ACE_wrappers" /I "include" /D "_DEBUG" /D "_CONSOLE" /D "HAVE_CONFIG_H" /D "WIN32" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\magick.pch" /YX"pch.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /Zm200 /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\magick.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\base.sbr" \
	"$(INTDIR)\chanserv.sbr" \
	"$(INTDIR)\chandata.sbr" \
	"$(INTDIR)\commserv.sbr" \
	"$(INTDIR)\convert_epona.sbr" \
	"$(INTDIR)\convert_esper.sbr" \
	"$(INTDIR)\convert_magick.sbr" \
	"$(INTDIR)\datetime.sbr" \
	"$(INTDIR)\dccengine.sbr" \
	"$(INTDIR)\filesys.sbr" \
	"$(INTDIR)\ircsocket.sbr" \
	"$(INTDIR)\lockable.sbr" \
	"$(INTDIR)\magick.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\mconfig.sbr" \
	"$(INTDIR)\memoserv.sbr" \
	"$(INTDIR)\mstring.sbr" \
	"$(INTDIR)\nickserv.sbr" \
	"$(INTDIR)\nickdata.sbr" \
	"$(INTDIR)\operserv.sbr" \
	"$(INTDIR)\server.sbr" \
	"$(INTDIR)\servmsg.sbr" \
	"$(INTDIR)\stages.sbr" \
	"$(INTDIR)\trace.sbr" \
	"$(INTDIR)\utils.sbr" \
	"$(INTDIR)\variant.sbr"

"$(OUTDIR)\magick.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=zlibd.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\magick.pdb" /debug /machine:I386 /nodefaultlib:"LIBCD.LIB" /out:"$(OUTDIR)\magick.exe" /pdbtype:sept /libpath:"Debug" /libpath:"..\support\zlib-1.1.3\Debug" /libpath:"..\support\ACE_wrappers\ace" /libpath:"..\support\ACE_wrappers\TAO\tao" 
LINK32_OBJS= \
	"$(INTDIR)\base.obj" \
	"$(INTDIR)\chanserv.obj" \
	"$(INTDIR)\chandata.obj" \
	"$(INTDIR)\commserv.obj" \
	"$(INTDIR)\convert_epona.obj" \
	"$(INTDIR)\convert_esper.obj" \
	"$(INTDIR)\convert_magick.obj" \
	"$(INTDIR)\datetime.obj" \
	"$(INTDIR)\dccengine.obj" \
	"$(INTDIR)\filesys.obj" \
	"$(INTDIR)\ircsocket.obj" \
	"$(INTDIR)\lockable.obj" \
	"$(INTDIR)\magick.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\mconfig.obj" \
	"$(INTDIR)\memoserv.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\nickserv.obj" \
	"$(INTDIR)\nickdata.obj" \
	"$(INTDIR)\operserv.obj" \
	"$(INTDIR)\server.obj" \
	"$(INTDIR)\servmsg.obj" \
	"$(INTDIR)\stages.obj" \
	"$(INTDIR)\trace.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\variant.obj" \
	"$(INTDIR)\magick.res" \
	"$(OUTDIR)\crypt.lib" \
	"$(OUTDIR)\xml.lib"

"$(OUTDIR)\magick.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("magick.dep")
!INCLUDE "magick.dep"
!ELSE 
!MESSAGE Warning: cannot find "magick.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "magick - Win32 Release" || "$(CFG)" == "magick - Win32 Debug"
SOURCE=.\src\base.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\base.obj"	"$(INTDIR)\base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\chandata.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\chandata.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\chandata.obj"	"$(INTDIR)\chandata.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\chanserv.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\chanserv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\chanserv.obj"	"$(INTDIR)\chanserv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\commserv.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\commserv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\commserv.obj"	"$(INTDIR)\commserv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\convert_epona.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\convert_epona.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\convert_epona.obj"	"$(INTDIR)\convert_epona.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\convert_esper.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\convert_esper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\convert_esper.obj"	"$(INTDIR)\convert_esper.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\convert_magick.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\convert_magick.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\convert_magick.obj"	"$(INTDIR)\convert_magick.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\datetime.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\datetime.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\datetime.obj"	"$(INTDIR)\datetime.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\dccengine.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\dccengine.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\dccengine.obj"	"$(INTDIR)\dccengine.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\filesys.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\filesys.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\filesys.obj"	"$(INTDIR)\filesys.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\ircsocket.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\ircsocket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\ircsocket.obj"	"$(INTDIR)\ircsocket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\lockable.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\lockable.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\lockable.obj"	"$(INTDIR)\lockable.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\magick.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\magick.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\magick.obj"	"$(INTDIR)\magick.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\main.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\mconfig.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\mconfig.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\mconfig.obj"	"$(INTDIR)\mconfig.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\memoserv.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\memoserv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\memoserv.obj"	"$(INTDIR)\memoserv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\mstring.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\mstring.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\mstring.obj"	"$(INTDIR)\mstring.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\nickdata.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\nickdata.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\nickdata.obj"	"$(INTDIR)\nickdata.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\nickserv.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\nickserv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\nickserv.obj"	"$(INTDIR)\nickserv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\operserv.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\operserv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\operserv.obj"	"$(INTDIR)\operserv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\server.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\server.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\server.obj"	"$(INTDIR)\server.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\servmsg.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\servmsg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\servmsg.obj"	"$(INTDIR)\servmsg.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\stages.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\stages.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\stages.obj"	"$(INTDIR)\stages.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\trace.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\trace.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\trace.obj"	"$(INTDIR)\trace.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\utils.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\utils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\utils.obj"	"$(INTDIR)\utils.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\variant.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\variant.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\variant.obj"	"$(INTDIR)\variant.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\magick.rc

"$(INTDIR)\magick.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


!IF  "$(CFG)" == "magick - Win32 Release"

"crypt - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\crypt.mak" CFG="crypt - Win32 Release" 
   cd "."

"crypt - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\crypt.mak" CFG="crypt - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

"crypt - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\crypt.mak" CFG="crypt - Win32 Debug" 
   cd "."

"crypt - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\crypt.mak" CFG="crypt - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "magick - Win32 Release"

"xml - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\xml.mak" CFG="xml - Win32 Release" 
   cd "."

"xml - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\xml.mak" CFG="xml - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

"xml - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\xml.mak" CFG="xml - Win32 Debug" 
   cd "."

"xml - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\xml.mak" CFG="xml - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 


!ENDIF 

