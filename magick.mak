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

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "magick - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\magick.exe"

!ELSE 

ALL : "$(OUTDIR)\magick.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AParser.obj"
	-@erase "$(INTDIR)\ATokenBuffer.obj"
	-@erase "$(INTDIR)\bob.obj"
	-@erase "$(INTDIR)\confbase.obj"
	-@erase "$(INTDIR)\DLexerBase.obj"
	-@erase "$(INTDIR)\EscapeParser.obj"
	-@erase "$(INTDIR)\escDlgLexer.obj"
	-@erase "$(INTDIR)\escparse.obj"
	-@erase "$(INTDIR)\fileconf.obj"
	-@erase "$(INTDIR)\lockable.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\magick.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\mstream.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\textfile.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\magick.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\magick.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib ace.lib ..\bob\Release\bob.lib ..\cryptlib21\Release\cl32.lib\
 zlib.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\magick.pdb"\
 /machine:I386 /out:"$(OUTDIR)\magick.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AParser.obj" \
	"$(INTDIR)\ATokenBuffer.obj" \
	"$(INTDIR)\bob.obj" \
	"$(INTDIR)\confbase.obj" \
	"$(INTDIR)\DLexerBase.obj" \
	"$(INTDIR)\EscapeParser.obj" \
	"$(INTDIR)\escDlgLexer.obj" \
	"$(INTDIR)\escparse.obj" \
	"$(INTDIR)\fileconf.obj" \
	"$(INTDIR)\lockable.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\magick.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\mstream.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\textfile.obj" \
	"$(INTDIR)\utils.obj"

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

ALL : "$(OUTDIR)\magick.exe"

!ELSE 

ALL : "$(OUTDIR)\magick.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AParser.obj"
	-@erase "$(INTDIR)\ATokenBuffer.obj"
	-@erase "$(INTDIR)\bob.obj"
	-@erase "$(INTDIR)\confbase.obj"
	-@erase "$(INTDIR)\DLexerBase.obj"
	-@erase "$(INTDIR)\EscapeParser.obj"
	-@erase "$(INTDIR)\escDlgLexer.obj"
	-@erase "$(INTDIR)\escparse.obj"
	-@erase "$(INTDIR)\fileconf.obj"
	-@erase "$(INTDIR)\lockable.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\magick.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\mstream.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\textfile.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\magick.exe"
	-@erase "$(OUTDIR)\magick.ilk"
	-@erase "$(OUTDIR)\magick.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Gf /I "..\bob" /I\
 "..\cryptlib21" /I "include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\magick.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib aced.lib ..\bob\Debug\bobd.lib ..\cryptlib21\Debug\cl32d.lib\
 zlib.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\magick.pdb" /debug /machine:I386 /out:"$(OUTDIR)\magick.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AParser.obj" \
	"$(INTDIR)\ATokenBuffer.obj" \
	"$(INTDIR)\bob.obj" \
	"$(INTDIR)\confbase.obj" \
	"$(INTDIR)\DLexerBase.obj" \
	"$(INTDIR)\EscapeParser.obj" \
	"$(INTDIR)\escDlgLexer.obj" \
	"$(INTDIR)\escparse.obj" \
	"$(INTDIR)\fileconf.obj" \
	"$(INTDIR)\lockable.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\magick.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\mstream.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\textfile.obj" \
	"$(INTDIR)\utils.obj"

"$(OUTDIR)\magick.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "magick - Win32 Release" || "$(CFG)" == "magick - Win32 Debug"
SOURCE=.\src\AParser.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_APARS=\
	".\src\pcctscfg.h"\
	

"$(INTDIR)\AParser.obj" : $(SOURCE) $(DEP_CPP_APARS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_APARS=\
	".\src\AParser.h"\
	".\src\AToken.h"\
	".\src\ATokenBuffer.h"\
	".\src\ATokenStream.h"\
	".\src\pcctscfg.h"\
	

"$(INTDIR)\AParser.obj" : $(SOURCE) $(DEP_CPP_APARS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\ATokenBuffer.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_ATOKE=\
	".\src\pcctscfg.h"\
	

"$(INTDIR)\ATokenBuffer.obj" : $(SOURCE) $(DEP_CPP_ATOKE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_ATOKE=\
	".\src\AToken.h"\
	".\src\ATokPtr.cpp"\
	

"$(INTDIR)\ATokenBuffer.obj" : $(SOURCE) $(DEP_CPP_ATOKE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\bob.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

NODEP_CPP_BOB_C=\
	".\src\bob.hpp"\
	".\src\magick.h"\
	

"$(INTDIR)\bob.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_BOB_C=\
	"..\..\program files\devstudio\vc\include\ace\atomic_op.i"\
	"..\..\program files\devstudio\vc\include\ace\containers.h"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.h"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.i"\
	"..\..\program files\devstudio\vc\include\ace\event_handler.h"\
	"..\..\program files\devstudio\vc\include\ace\free_list.cpp"\
	"..\..\program files\devstudio\vc\include\ace\free_list.h"\
	"..\..\program files\devstudio\vc\include\ace\free_list.i"\
	"..\..\program files\devstudio\vc\include\ace\handle_set.h"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.cpp"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\io_cntl_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.i"\
	"..\..\program files\devstudio\vc\include\ace\mem_map.h"\
	"..\..\program files\devstudio\vc\include\ace\memory_pool.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.i"\
	"..\..\program files\devstudio\vc\include\ace\message_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\module.cpp"\
	"..\..\program files\devstudio\vc\include\ace\module.h"\
	"..\..\program files\devstudio\vc\include\ace\module.i"\
	"..\..\program files\devstudio\vc\include\ace\reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\reactor_impl.h"\
	"..\..\program files\devstudio\vc\include\ace\service_config.h"\
	"..\..\program files\devstudio\vc\include\ace\service_object.h"\
	"..\..\program files\devstudio\vc\include\ace\service_repository.h"\
	"..\..\program files\devstudio\vc\include\ace\service_types.h"\
	"..\..\program files\devstudio\vc\include\ace\shared_object.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.i"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.cpp"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.h"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.i"\
	"..\..\program files\devstudio\vc\include\ace\svc_conf_tokens.h"\
	"..\..\program files\devstudio\vc\include\ace\synch.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_options.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.i"\
	"..\..\program files\devstudio\vc\include\ace\task.h"\
	"..\..\program files\devstudio\vc\include\ace\task_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\task_t.h"\
	"..\..\program files\devstudio\vc\include\ace\task_t.i"\
	"..\..\program files\devstudio\vc\include\ace\thread.h"\
	"..\..\program files\devstudio\vc\include\ace\thread_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.h"\
	"..\bob\bob.h"\
	"..\bob\bobexp.h"\
	"..\bob\compiler.h"\
	"..\bob\eval.h"\
	"..\bob\execute.h"\
	"..\bob\function.h"\
	"..\bob\objects.h"\
	"..\bob\streams.h"\
	".\include\bob.hpp"\
	".\include\lockable.h"\
	".\include\magick.h"\
	".\include\mstring.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\config-win32-borland.h"\
	{$(INCLUDE)}"ace\config-win32-common.h"\
	{$(INCLUDE)}"ace\config-win32.h"\
	{$(INCLUDE)}"ace\config.h"\
	{$(INCLUDE)}"ace\inc_user_config.h"\
	{$(INCLUDE)}"ace\log_msg.h"\
	{$(INCLUDE)}"ace\log_priority.h"\
	{$(INCLUDE)}"ace\log_record.h"\
	{$(INCLUDE)}"ace\log_record.i"\
	{$(INCLUDE)}"ace\malloc_base.h"\
	{$(INCLUDE)}"ace\managed_object.cpp"\
	{$(INCLUDE)}"ace\managed_object.h"\
	{$(INCLUDE)}"ace\managed_object.i"\
	{$(INCLUDE)}"ace\object_manager.h"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\sstring.h"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\bob.obj" : $(SOURCE) $(DEP_CPP_BOB_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\confbase.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

NODEP_CPP_CONFB=\
	".\src\confbase.h"\
	".\src\fileconf.h"\
	".\src\log.h"\
	

"$(INTDIR)\confbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_CONFB=\
	".\include\confbase.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstring.h"\
	

"$(INTDIR)\confbase.obj" : $(SOURCE) $(DEP_CPP_CONFB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\DLexerBase.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_DLEXE=\
	".\src\pcctscfg.h"\
	

"$(INTDIR)\DLexerBase.obj" : $(SOURCE) $(DEP_CPP_DLEXE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_DLEXE=\
	".\src\AToken.h"\
	".\src\ATokenStream.h"\
	".\src\DLexerBase.h"\
	".\src\pcctscfg.h"\
	

"$(INTDIR)\DLexerBase.obj" : $(SOURCE) $(DEP_CPP_DLEXE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\EscapeParser.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_ESCAP=\
	".\src\AParser.h"\
	".\src\EscapeParser.h"\
	".\src\esctokens.h"\
	".\src\pcctscfg.h"\
	

"$(INTDIR)\EscapeParser.obj" : $(SOURCE) $(DEP_CPP_ESCAP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_ESCAP=\
	".\src\AParser.h"\
	".\src\AToken.h"\
	".\src\ATokenBuffer.h"\
	".\src\ATokenStream.h"\
	".\src\EscapeParser.h"\
	".\src\esctokens.h"\
	".\src\pcctscfg.h"\
	

"$(INTDIR)\EscapeParser.obj" : $(SOURCE) $(DEP_CPP_ESCAP) "$(INTDIR)"\
 ".\src\EscapeParser.h" ".\src\esctokens.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\escDlgLexer.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_ESCDL=\
	".\src\AParser.h"\
	".\src\AToken.h"\
	".\src\DLexer.cpp"\
	".\src\DLexerBase.h"\
	".\src\escDlgLexer.h"\
	".\src\esctokens.h"\
	".\src\pcctscfg.h"\
	

"$(INTDIR)\escDlgLexer.obj" : $(SOURCE) $(DEP_CPP_ESCDL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_ESCDL=\
	".\src\AParser.h"\
	".\src\AToken.h"\
	".\src\DLexer.cpp"\
	".\src\escDlgLexer.h"\
	

"$(INTDIR)\escDlgLexer.obj" : $(SOURCE) $(DEP_CPP_ESCDL) "$(INTDIR)"\
 ".\src\escDlgLexer.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\escparse.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_ESCPA=\
	".\src\AParser.h"\
	".\src\ATokPtr.h"\
	".\src\DLexerBase.h"\
	".\src\EscapeParser.h"\
	".\src\escDlgLexer.h"\
	".\src\esctokens.h"\
	".\src\pcctscfg.h"\
	
NODEP_CPP_ESCPA=\
	".\src\mstring.h"\
	

"$(INTDIR)\escparse.obj" : $(SOURCE) $(DEP_CPP_ESCPA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\escparse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\escparse.dlg

!IF  "$(CFG)" == "magick - Win32 Release"

!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

InputPath=.\src\escparse.dlg

"src\escDlgLexer.cpp"	"src\escDlgLexer.h"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	dlg -CC -o src src\escparse.dlg -cl escDlgLexer

!ENDIF 

SOURCE=.\src\escparse.g

!IF  "$(CFG)" == "magick - Win32 Release"

!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

InputPath=.\src\escparse.g

"src\escparse.dlg"	"src\esctokens.h"	"src\EscapeParser.h"\
	"src\EscapeParser.cpp"	"src\escparse.cpp"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	antlr -CC src\escparse.g -o src -ft esctokens.h -fl escparse.dlg

!ENDIF 

SOURCE=.\src\fileconf.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

NODEP_CPP_FILEC=\
	".\src\fileconf.h"\
	".\src\utils.h"\
	

"$(INTDIR)\fileconf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_FILEC=\
	".\include\confbase.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\utils.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\auto_ptr.cpp"\
	{$(INCLUDE)}"ace\auto_ptr.h"\
	{$(INCLUDE)}"ace\auto_ptr.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\config-win32-borland.h"\
	{$(INCLUDE)}"ace\config-win32-common.h"\
	{$(INCLUDE)}"ace\config-win32.h"\
	{$(INCLUDE)}"ace\config.h"\
	{$(INCLUDE)}"ace\inc_user_config.h"\
	{$(INCLUDE)}"ace\log_msg.h"\
	{$(INCLUDE)}"ace\log_priority.h"\
	{$(INCLUDE)}"ace\log_record.h"\
	{$(INCLUDE)}"ace\log_record.i"\
	{$(INCLUDE)}"ace\managed_object.cpp"\
	{$(INCLUDE)}"ace\managed_object.h"\
	{$(INCLUDE)}"ace\managed_object.i"\
	{$(INCLUDE)}"ace\object_manager.h"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\fileconf.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\lockable.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

NODEP_CPP_LOCKA=\
	".\src\lockable.h"\
	

"$(INTDIR)\lockable.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_LOCKA=\
	"..\..\program files\devstudio\vc\include\ace\atomic_op.i"\
	"..\..\program files\devstudio\vc\include\ace\containers.h"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.h"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.i"\
	"..\..\program files\devstudio\vc\include\ace\event_handler.h"\
	"..\..\program files\devstudio\vc\include\ace\free_list.cpp"\
	"..\..\program files\devstudio\vc\include\ace\free_list.h"\
	"..\..\program files\devstudio\vc\include\ace\free_list.i"\
	"..\..\program files\devstudio\vc\include\ace\handle_set.h"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.cpp"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\io_cntl_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.i"\
	"..\..\program files\devstudio\vc\include\ace\mem_map.h"\
	"..\..\program files\devstudio\vc\include\ace\memory_pool.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.i"\
	"..\..\program files\devstudio\vc\include\ace\message_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\module.cpp"\
	"..\..\program files\devstudio\vc\include\ace\module.h"\
	"..\..\program files\devstudio\vc\include\ace\module.i"\
	"..\..\program files\devstudio\vc\include\ace\reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\reactor_impl.h"\
	"..\..\program files\devstudio\vc\include\ace\service_config.h"\
	"..\..\program files\devstudio\vc\include\ace\service_object.h"\
	"..\..\program files\devstudio\vc\include\ace\service_repository.h"\
	"..\..\program files\devstudio\vc\include\ace\service_types.h"\
	"..\..\program files\devstudio\vc\include\ace\shared_object.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.i"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.cpp"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.h"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.i"\
	"..\..\program files\devstudio\vc\include\ace\svc_conf_tokens.h"\
	"..\..\program files\devstudio\vc\include\ace\synch.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_options.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.i"\
	"..\..\program files\devstudio\vc\include\ace\task.h"\
	"..\..\program files\devstudio\vc\include\ace\task_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\task_t.h"\
	"..\..\program files\devstudio\vc\include\ace\task_t.i"\
	"..\..\program files\devstudio\vc\include\ace\thread.h"\
	"..\..\program files\devstudio\vc\include\ace\thread_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.h"\
	".\include\lockable.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\config-win32-borland.h"\
	{$(INCLUDE)}"ace\config-win32-common.h"\
	{$(INCLUDE)}"ace\config-win32.h"\
	{$(INCLUDE)}"ace\config.h"\
	{$(INCLUDE)}"ace\inc_user_config.h"\
	{$(INCLUDE)}"ace\log_msg.h"\
	{$(INCLUDE)}"ace\log_priority.h"\
	{$(INCLUDE)}"ace\log_record.h"\
	{$(INCLUDE)}"ace\log_record.i"\
	{$(INCLUDE)}"ace\malloc_base.h"\
	{$(INCLUDE)}"ace\managed_object.cpp"\
	{$(INCLUDE)}"ace\managed_object.h"\
	{$(INCLUDE)}"ace\managed_object.i"\
	{$(INCLUDE)}"ace\object_manager.h"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\sstring.h"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\lockable.obj" : $(SOURCE) $(DEP_CPP_LOCKA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\log.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

NODEP_CPP_LOG_C=\
	".\src\log.h"\
	

"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_LOG_C=\
	".\include\log.h"\
	".\include\mstring.h"\
	

"$(INTDIR)\log.obj" : $(SOURCE) $(DEP_CPP_LOG_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\magick.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

NODEP_CPP_MAGIC=\
	".\src\log.h"\
	".\src\magick.h"\
	

"$(INTDIR)\magick.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MAGIC=\
	"..\..\program files\devstudio\vc\include\ace\atomic_op.i"\
	"..\..\program files\devstudio\vc\include\ace\containers.h"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.h"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.i"\
	"..\..\program files\devstudio\vc\include\ace\event_handler.h"\
	"..\..\program files\devstudio\vc\include\ace\free_list.cpp"\
	"..\..\program files\devstudio\vc\include\ace\free_list.h"\
	"..\..\program files\devstudio\vc\include\ace\free_list.i"\
	"..\..\program files\devstudio\vc\include\ace\handle_set.h"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.cpp"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\io_cntl_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.i"\
	"..\..\program files\devstudio\vc\include\ace\mem_map.h"\
	"..\..\program files\devstudio\vc\include\ace\memory_pool.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.i"\
	"..\..\program files\devstudio\vc\include\ace\message_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\module.cpp"\
	"..\..\program files\devstudio\vc\include\ace\module.h"\
	"..\..\program files\devstudio\vc\include\ace\module.i"\
	"..\..\program files\devstudio\vc\include\ace\reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\reactor_impl.h"\
	"..\..\program files\devstudio\vc\include\ace\service_config.h"\
	"..\..\program files\devstudio\vc\include\ace\service_object.h"\
	"..\..\program files\devstudio\vc\include\ace\service_repository.h"\
	"..\..\program files\devstudio\vc\include\ace\service_types.h"\
	"..\..\program files\devstudio\vc\include\ace\shared_object.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.i"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.cpp"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.h"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.i"\
	"..\..\program files\devstudio\vc\include\ace\svc_conf_tokens.h"\
	"..\..\program files\devstudio\vc\include\ace\synch.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_options.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.i"\
	"..\..\program files\devstudio\vc\include\ace\task.h"\
	"..\..\program files\devstudio\vc\include\ace\task_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\task_t.h"\
	"..\..\program files\devstudio\vc\include\ace\task_t.i"\
	"..\..\program files\devstudio\vc\include\ace\thread.h"\
	"..\..\program files\devstudio\vc\include\ace\thread_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.h"\
	"..\bob\bob.h"\
	"..\bob\bobexp.h"\
	"..\bob\compiler.h"\
	"..\bob\eval.h"\
	"..\bob\execute.h"\
	"..\bob\function.h"\
	"..\bob\objects.h"\
	"..\bob\streams.h"\
	".\include\bob.hpp"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstring.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\config-win32-borland.h"\
	{$(INCLUDE)}"ace\config-win32-common.h"\
	{$(INCLUDE)}"ace\config-win32.h"\
	{$(INCLUDE)}"ace\config.h"\
	{$(INCLUDE)}"ace\inc_user_config.h"\
	{$(INCLUDE)}"ace\log_msg.h"\
	{$(INCLUDE)}"ace\log_priority.h"\
	{$(INCLUDE)}"ace\log_record.h"\
	{$(INCLUDE)}"ace\log_record.i"\
	{$(INCLUDE)}"ace\malloc_base.h"\
	{$(INCLUDE)}"ace\managed_object.cpp"\
	{$(INCLUDE)}"ace\managed_object.h"\
	{$(INCLUDE)}"ace\managed_object.i"\
	{$(INCLUDE)}"ace\object_manager.h"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\sstring.h"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\magick.obj" : $(SOURCE) $(DEP_CPP_MAGIC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\main.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

NODEP_CPP_MAIN_=\
	".\src\magick.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MAIN_=\
	"..\..\program files\devstudio\vc\include\ace\atomic_op.i"\
	"..\..\program files\devstudio\vc\include\ace\containers.h"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.h"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.i"\
	"..\..\program files\devstudio\vc\include\ace\event_handler.h"\
	"..\..\program files\devstudio\vc\include\ace\free_list.cpp"\
	"..\..\program files\devstudio\vc\include\ace\free_list.h"\
	"..\..\program files\devstudio\vc\include\ace\free_list.i"\
	"..\..\program files\devstudio\vc\include\ace\handle_set.h"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.cpp"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\io_cntl_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.i"\
	"..\..\program files\devstudio\vc\include\ace\mem_map.h"\
	"..\..\program files\devstudio\vc\include\ace\memory_pool.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.i"\
	"..\..\program files\devstudio\vc\include\ace\message_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\module.cpp"\
	"..\..\program files\devstudio\vc\include\ace\module.h"\
	"..\..\program files\devstudio\vc\include\ace\module.i"\
	"..\..\program files\devstudio\vc\include\ace\reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\reactor_impl.h"\
	"..\..\program files\devstudio\vc\include\ace\service_config.h"\
	"..\..\program files\devstudio\vc\include\ace\service_object.h"\
	"..\..\program files\devstudio\vc\include\ace\service_repository.h"\
	"..\..\program files\devstudio\vc\include\ace\service_types.h"\
	"..\..\program files\devstudio\vc\include\ace\shared_object.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.i"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.cpp"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.h"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.i"\
	"..\..\program files\devstudio\vc\include\ace\svc_conf_tokens.h"\
	"..\..\program files\devstudio\vc\include\ace\synch.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_options.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.i"\
	"..\..\program files\devstudio\vc\include\ace\task.h"\
	"..\..\program files\devstudio\vc\include\ace\task_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\task_t.h"\
	"..\..\program files\devstudio\vc\include\ace\task_t.i"\
	"..\..\program files\devstudio\vc\include\ace\thread.h"\
	"..\..\program files\devstudio\vc\include\ace\thread_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.h"\
	"..\bob\bob.h"\
	"..\bob\bobexp.h"\
	"..\bob\compiler.h"\
	"..\bob\eval.h"\
	"..\bob\execute.h"\
	"..\bob\function.h"\
	"..\bob\objects.h"\
	"..\bob\streams.h"\
	".\include\bob.hpp"\
	".\include\lockable.h"\
	".\include\magick.h"\
	".\include\mstring.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\config-win32-borland.h"\
	{$(INCLUDE)}"ace\config-win32-common.h"\
	{$(INCLUDE)}"ace\config-win32.h"\
	{$(INCLUDE)}"ace\config.h"\
	{$(INCLUDE)}"ace\inc_user_config.h"\
	{$(INCLUDE)}"ace\log_msg.h"\
	{$(INCLUDE)}"ace\log_priority.h"\
	{$(INCLUDE)}"ace\log_record.h"\
	{$(INCLUDE)}"ace\log_record.i"\
	{$(INCLUDE)}"ace\malloc_base.h"\
	{$(INCLUDE)}"ace\managed_object.cpp"\
	{$(INCLUDE)}"ace\managed_object.h"\
	{$(INCLUDE)}"ace\managed_object.i"\
	{$(INCLUDE)}"ace\object_manager.h"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\sstring.h"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\mstream.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_MSTRE=\
	{$(INCLUDE)}"sys\types.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
NODEP_CPP_MSTRE=\
	".\src\log.h"\
	".\src\mstream.h"\
	".\src\utils.h"\
	

"$(INTDIR)\mstream.obj" : $(SOURCE) $(DEP_CPP_MSTRE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MSTRE=\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\utils.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\config-win32-borland.h"\
	{$(INCLUDE)}"ace\config-win32-common.h"\
	{$(INCLUDE)}"ace\config-win32.h"\
	{$(INCLUDE)}"ace\config.h"\
	{$(INCLUDE)}"ace\inc_user_config.h"\
	{$(INCLUDE)}"ace\log_msg.h"\
	{$(INCLUDE)}"ace\log_priority.h"\
	{$(INCLUDE)}"ace\log_record.h"\
	{$(INCLUDE)}"ace\log_record.i"\
	{$(INCLUDE)}"ace\managed_object.cpp"\
	{$(INCLUDE)}"ace\managed_object.h"\
	{$(INCLUDE)}"ace\managed_object.i"\
	{$(INCLUDE)}"ace\object_manager.h"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

"$(INTDIR)\mstream.obj" : $(SOURCE) $(DEP_CPP_MSTRE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\mstring.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_MSTRI=\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\auto_ptr.cpp"\
	{$(INCLUDE)}"ace\auto_ptr.h"\
	{$(INCLUDE)}"ace\auto_ptr.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\Basic_Types.i"\
	{$(INCLUDE)}"ace\config-win32-borland.h"\
	{$(INCLUDE)}"ace\config-win32-common.h"\
	{$(INCLUDE)}"ace\config-win32.h"\
	{$(INCLUDE)}"ace\config-WinCE.h"\
	{$(INCLUDE)}"ace\config.h"\
	{$(INCLUDE)}"ace\inc_user_config.h"\
	{$(INCLUDE)}"ace\iosfwd.h"\
	{$(INCLUDE)}"ace\log_msg.h"\
	{$(INCLUDE)}"ace\log_priority.h"\
	{$(INCLUDE)}"ace\log_record.h"\
	{$(INCLUDE)}"ace\log_record.i"\
	{$(INCLUDE)}"ace\malloc_base.h"\
	{$(INCLUDE)}"ace\managed_object.cpp"\
	{$(INCLUDE)}"ace\managed_object.h"\
	{$(INCLUDE)}"ace\managed_object.i"\
	{$(INCLUDE)}"ace\object_manager.h"\
	{$(INCLUDE)}"ace\Object_Manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\OS.i"\
	{$(INCLUDE)}"ace\sstring.h"\
	{$(INCLUDE)}"ace\SString.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	{$(INCLUDE)}"ace\ws2tcpip.h"\
	
NODEP_CPP_MSTRI=\
	".\src\mstring.h"\
	

"$(INTDIR)\mstring.obj" : $(SOURCE) $(DEP_CPP_MSTRI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MSTRI=\
	".\include\mstring.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\config-win32-borland.h"\
	{$(INCLUDE)}"ace\config-win32-common.h"\
	{$(INCLUDE)}"ace\config-win32.h"\
	{$(INCLUDE)}"ace\config.h"\
	{$(INCLUDE)}"ace\inc_user_config.h"\
	{$(INCLUDE)}"ace\log_msg.h"\
	{$(INCLUDE)}"ace\log_priority.h"\
	{$(INCLUDE)}"ace\log_record.h"\
	{$(INCLUDE)}"ace\log_record.i"\
	{$(INCLUDE)}"ace\managed_object.cpp"\
	{$(INCLUDE)}"ace\managed_object.h"\
	{$(INCLUDE)}"ace\managed_object.i"\
	{$(INCLUDE)}"ace\object_manager.h"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\mstring.obj" : $(SOURCE) $(DEP_CPP_MSTRI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\textfile.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

NODEP_CPP_TEXTF=\
	".\src\textfile.h"\
	

"$(INTDIR)\textfile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_TEXTF=\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\config-win32-borland.h"\
	{$(INCLUDE)}"ace\config-win32-common.h"\
	{$(INCLUDE)}"ace\config-win32.h"\
	{$(INCLUDE)}"ace\config.h"\
	{$(INCLUDE)}"ace\inc_user_config.h"\
	{$(INCLUDE)}"ace\log_msg.h"\
	{$(INCLUDE)}"ace\log_priority.h"\
	{$(INCLUDE)}"ace\log_record.h"\
	{$(INCLUDE)}"ace\log_record.i"\
	{$(INCLUDE)}"ace\managed_object.cpp"\
	{$(INCLUDE)}"ace\managed_object.h"\
	{$(INCLUDE)}"ace\managed_object.i"\
	{$(INCLUDE)}"ace\object_manager.h"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\textfile.obj" : $(SOURCE) $(DEP_CPP_TEXTF) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\utils.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

NODEP_CPP_UTILS=\
	".\src\log.h"\
	".\src\utils.h"\
	

"$(INTDIR)\utils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_UTILS=\
	".\include\log.h"\
	".\include\mstring.h"\
	".\include\utils.h"\
	

"$(INTDIR)\utils.obj" : $(SOURCE) $(DEP_CPP_UTILS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

