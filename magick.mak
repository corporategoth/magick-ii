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

ALL : "$(OUTDIR)\magick.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ANTLRException.obj"
	-@erase "$(INTDIR)\AST.obj"
	-@erase "$(INTDIR)\ASTFactory.obj"
	-@erase "$(INTDIR)\BitSet.obj"
	-@erase "$(INTDIR)\bob.obj"
	-@erase "$(INTDIR)\CharBuffer.obj"
	-@erase "$(INTDIR)\CharScanner.obj"
	-@erase "$(INTDIR)\CommonToken.obj"
	-@erase "$(INTDIR)\confbase.obj"
	-@erase "$(INTDIR)\datetime.obj"
	-@erase "$(INTDIR)\EscLexer.obj"
	-@erase "$(INTDIR)\EscParser.obj"
	-@erase "$(INTDIR)\fileconf.obj"
	-@erase "$(INTDIR)\LLkParser.obj"
	-@erase "$(INTDIR)\lockable.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\magick.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\MismatchedTokenException.obj"
	-@erase "$(INTDIR)\mstream.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\NoViableAltException.obj"
	-@erase "$(INTDIR)\Parser.obj"
	-@erase "$(INTDIR)\ParserException.obj"
	-@erase "$(INTDIR)\ScannerException.obj"
	-@erase "$(INTDIR)\String.obj"
	-@erase "$(INTDIR)\textfile.obj"
	-@erase "$(INTDIR)\Token.obj"
	-@erase "$(INTDIR)\TokenBuffer.obj"
	-@erase "$(INTDIR)\trace.obj"
	-@erase "$(INTDIR)\TreeParser.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\variant.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\magick.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\bob" /I "..\cryptlib21" /I "include" /D\
 "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

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

RSC=rc.exe
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
	"$(INTDIR)\ANTLRException.obj" \
	"$(INTDIR)\AST.obj" \
	"$(INTDIR)\ASTFactory.obj" \
	"$(INTDIR)\BitSet.obj" \
	"$(INTDIR)\bob.obj" \
	"$(INTDIR)\CharBuffer.obj" \
	"$(INTDIR)\CharScanner.obj" \
	"$(INTDIR)\CommonToken.obj" \
	"$(INTDIR)\confbase.obj" \
	"$(INTDIR)\datetime.obj" \
	"$(INTDIR)\EscLexer.obj" \
	"$(INTDIR)\EscParser.obj" \
	"$(INTDIR)\fileconf.obj" \
	"$(INTDIR)\LLkParser.obj" \
	"$(INTDIR)\lockable.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\magick.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\MismatchedTokenException.obj" \
	"$(INTDIR)\mstream.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\NoViableAltException.obj" \
	"$(INTDIR)\Parser.obj" \
	"$(INTDIR)\ParserException.obj" \
	"$(INTDIR)\ScannerException.obj" \
	"$(INTDIR)\String.obj" \
	"$(INTDIR)\textfile.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\TokenBuffer.obj" \
	"$(INTDIR)\trace.obj" \
	"$(INTDIR)\TreeParser.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\variant.obj"

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
	-@erase "$(INTDIR)\ANTLRException.obj"
	-@erase "$(INTDIR)\AST.obj"
	-@erase "$(INTDIR)\ASTFactory.obj"
	-@erase "$(INTDIR)\BitSet.obj"
	-@erase "$(INTDIR)\bob.obj"
	-@erase "$(INTDIR)\CharBuffer.obj"
	-@erase "$(INTDIR)\CharScanner.obj"
	-@erase "$(INTDIR)\CommonToken.obj"
	-@erase "$(INTDIR)\confbase.obj"
	-@erase "$(INTDIR)\datetime.obj"
	-@erase "$(INTDIR)\EscLexer.obj"
	-@erase "$(INTDIR)\EscParser.obj"
	-@erase "$(INTDIR)\fileconf.obj"
	-@erase "$(INTDIR)\LLkParser.obj"
	-@erase "$(INTDIR)\lockable.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\magick.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\MismatchedTokenException.obj"
	-@erase "$(INTDIR)\mstream.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\NoViableAltException.obj"
	-@erase "$(INTDIR)\Parser.obj"
	-@erase "$(INTDIR)\ParserException.obj"
	-@erase "$(INTDIR)\ScannerException.obj"
	-@erase "$(INTDIR)\String.obj"
	-@erase "$(INTDIR)\textfile.obj"
	-@erase "$(INTDIR)\Token.obj"
	-@erase "$(INTDIR)\TokenBuffer.obj"
	-@erase "$(INTDIR)\trace.obj"
	-@erase "$(INTDIR)\TreeParser.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\variant.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\magick.exe"
	-@erase "$(OUTDIR)\magick.ilk"
	-@erase "$(OUTDIR)\magick.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Gf /I "..\bob" /I\
 "..\cryptlib21" /I "include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

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

RSC=rc.exe
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
	"$(INTDIR)\ANTLRException.obj" \
	"$(INTDIR)\AST.obj" \
	"$(INTDIR)\ASTFactory.obj" \
	"$(INTDIR)\BitSet.obj" \
	"$(INTDIR)\bob.obj" \
	"$(INTDIR)\CharBuffer.obj" \
	"$(INTDIR)\CharScanner.obj" \
	"$(INTDIR)\CommonToken.obj" \
	"$(INTDIR)\confbase.obj" \
	"$(INTDIR)\datetime.obj" \
	"$(INTDIR)\EscLexer.obj" \
	"$(INTDIR)\EscParser.obj" \
	"$(INTDIR)\fileconf.obj" \
	"$(INTDIR)\LLkParser.obj" \
	"$(INTDIR)\lockable.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\magick.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\MismatchedTokenException.obj" \
	"$(INTDIR)\mstream.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\NoViableAltException.obj" \
	"$(INTDIR)\Parser.obj" \
	"$(INTDIR)\ParserException.obj" \
	"$(INTDIR)\ScannerException.obj" \
	"$(INTDIR)\String.obj" \
	"$(INTDIR)\textfile.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\TokenBuffer.obj" \
	"$(INTDIR)\trace.obj" \
	"$(INTDIR)\TreeParser.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\variant.obj"

"$(OUTDIR)\magick.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "magick - Win32 Release" || "$(CFG)" == "magick - Win32 Debug"
SOURCE=.\src\antlr\ANTLRException.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_ANTLR=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\config.hpp"\
	

"$(INTDIR)\ANTLRException.obj" : $(SOURCE) $(DEP_CPP_ANTLR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_ANTLR=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\config.hpp"\
	

"$(INTDIR)\ANTLRException.obj" : $(SOURCE) $(DEP_CPP_ANTLR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\AST.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_AST_C=\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\token.hpp"\
	

"$(INTDIR)\AST.obj" : $(SOURCE) $(DEP_CPP_AST_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_AST_C=\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\token.hpp"\
	

"$(INTDIR)\AST.obj" : $(SOURCE) $(DEP_CPP_AST_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\ASTFactory.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_ASTFA=\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astarray.hpp"\
	".\include\antlr\astfactory.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\astpair.hpp"\
	".\include\antlr\commonastnode.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\token.hpp"\
	

"$(INTDIR)\ASTFactory.obj" : $(SOURCE) $(DEP_CPP_ASTFA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_ASTFA=\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astarray.hpp"\
	".\include\antlr\astfactory.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\astpair.hpp"\
	".\include\antlr\commonastnode.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\token.hpp"\
	

"$(INTDIR)\ASTFactory.obj" : $(SOURCE) $(DEP_CPP_ASTFA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\BitSet.cpp
DEP_CPP_BITSE=\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\config.hpp"\
	

"$(INTDIR)\BitSet.obj" : $(SOURCE) $(DEP_CPP_BITSE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\bob.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_BOB_C=\
	"..\..\program files\devstudio\vc\include\ace\atomic_op.i"\
	"..\..\program files\devstudio\vc\include\ace\containers.h"\
	"..\..\program files\devstudio\vc\include\ace\containers.i"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.h"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.i"\
	"..\..\program files\devstudio\vc\include\ace\event_handler.h"\
	"..\..\program files\devstudio\vc\include\ace\event_handler.i"\
	"..\..\program files\devstudio\vc\include\ace\free_list.cpp"\
	"..\..\program files\devstudio\vc\include\ace\free_list.h"\
	"..\..\program files\devstudio\vc\include\ace\free_list.i"\
	"..\..\program files\devstudio\vc\include\ace\handle_set.h"\
	"..\..\program files\devstudio\vc\include\ace\handle_set.i"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.cpp"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\io_cntl_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc.i"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.i"\
	"..\..\program files\devstudio\vc\include\ace\mem_map.h"\
	"..\..\program files\devstudio\vc\include\ace\mem_map.i"\
	"..\..\program files\devstudio\vc\include\ace\memory_pool.h"\
	"..\..\program files\devstudio\vc\include\ace\memory_pool.i"\
	"..\..\program files\devstudio\vc\include\ace\message_block.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block.i"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.i"\
	"..\..\program files\devstudio\vc\include\ace\message_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue.i"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\module.cpp"\
	"..\..\program files\devstudio\vc\include\ace\module.h"\
	"..\..\program files\devstudio\vc\include\ace\module.i"\
	"..\..\program files\devstudio\vc\include\ace\reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\reactor.i"\
	"..\..\program files\devstudio\vc\include\ace\reactor_impl.h"\
	"..\..\program files\devstudio\vc\include\ace\service_config.h"\
	"..\..\program files\devstudio\vc\include\ace\service_config.i"\
	"..\..\program files\devstudio\vc\include\ace\service_object.h"\
	"..\..\program files\devstudio\vc\include\ace\service_object.i"\
	"..\..\program files\devstudio\vc\include\ace\service_repository.h"\
	"..\..\program files\devstudio\vc\include\ace\service_repository.i"\
	"..\..\program files\devstudio\vc\include\ace\service_types.h"\
	"..\..\program files\devstudio\vc\include\ace\service_types.i"\
	"..\..\program files\devstudio\vc\include\ace\shared_object.h"\
	"..\..\program files\devstudio\vc\include\ace\shared_object.i"\
	"..\..\program files\devstudio\vc\include\ace\signal.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.i"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.cpp"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.h"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.i"\
	"..\..\program files\devstudio\vc\include\ace\svc_conf_tokens.h"\
	"..\..\program files\devstudio\vc\include\ace\synch.h"\
	"..\..\program files\devstudio\vc\include\ace\synch.i"\
	"..\..\program files\devstudio\vc\include\ace\synch_options.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_options.i"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.i"\
	"..\..\program files\devstudio\vc\include\ace\task.h"\
	"..\..\program files\devstudio\vc\include\ace\task.i"\
	"..\..\program files\devstudio\vc\include\ace\task_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\task_t.h"\
	"..\..\program files\devstudio\vc\include\ace\task_t.i"\
	"..\..\program files\devstudio\vc\include\ace\thread.h"\
	"..\..\program files\devstudio\vc\include\ace\thread.i"\
	"..\..\program files\devstudio\vc\include\ace\thread_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\thread_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.i"\
	"..\bob\bob.h"\
	"..\bob\bobexp.h"\
	"..\bob\compiler.h"\
	"..\bob\eval.h"\
	"..\bob\execute.h"\
	"..\bob\function.h"\
	"..\bob\objects.h"\
	"..\bob\streams.h"\
	".\include\bob.hpp"\
	".\include\confbase.h"\
	".\include\fileconf.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\basic_types.i"\
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
	{$(INCLUDE)}"ace\object_manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\os.i"\
	{$(INCLUDE)}"ace\sstring.h"\
	{$(INCLUDE)}"ace\sstring.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\bob.obj" : $(SOURCE) $(DEP_CPP_BOB_C) "$(INTDIR)"
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
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
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

SOURCE=.\src\antlr\CharBuffer.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_CHARB=\
	".\include\antlr\charbuffer.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\config.hpp"\
	

"$(INTDIR)\CharBuffer.obj" : $(SOURCE) $(DEP_CPP_CHARB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_CHARB=\
	".\include\antlr\charbuffer.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\config.hpp"\
	

"$(INTDIR)\CharBuffer.obj" : $(SOURCE) $(DEP_CPP_CHARB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\CharScanner.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_CHARS=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\charbuffer.hpp"\
	".\include\antlr\charscanner.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\commontoken.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\scannerexception.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenizer.hpp"\
	

"$(INTDIR)\CharScanner.obj" : $(SOURCE) $(DEP_CPP_CHARS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_CHARS=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\charbuffer.hpp"\
	".\include\antlr\charscanner.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\commontoken.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\scannerexception.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenizer.hpp"\
	

"$(INTDIR)\CharScanner.obj" : $(SOURCE) $(DEP_CPP_CHARS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\CommonToken.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_COMMO=\
	".\include\antlr\commontoken.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\string.hpp"\
	".\include\antlr\token.hpp"\
	

"$(INTDIR)\CommonToken.obj" : $(SOURCE) $(DEP_CPP_COMMO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_COMMO=\
	".\include\antlr\commontoken.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\string.hpp"\
	".\include\antlr\token.hpp"\
	

"$(INTDIR)\CommonToken.obj" : $(SOURCE) $(DEP_CPP_COMMO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\confbase.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_CONFB=\
	".\include\confbase.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\basic_types.i"\
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
	{$(INCLUDE)}"ace\object_manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\os.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\confbase.obj" : $(SOURCE) $(DEP_CPP_CONFB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_CONFB=\
	".\include\confbase.h"\
	".\include\fileconf.h"\
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
	

"$(INTDIR)\confbase.obj" : $(SOURCE) $(DEP_CPP_CONFB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\datetime.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_DATET=\
	".\include\datetime.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\Auto_Ptr.cpp"\
	{$(INCLUDE)}"ace\Auto_Ptr.h"\
	{$(INCLUDE)}"ace\Auto_Ptr.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\basic_types.i"\
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
	{$(INCLUDE)}"ace\object_manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\os.i"\
	{$(INCLUDE)}"ace\sstring.h"\
	{$(INCLUDE)}"ace\sstring.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	{$(INCLUDE)}"ace\ws2tcpip.h"\
	

"$(INTDIR)\datetime.obj" : $(SOURCE) $(DEP_CPP_DATET) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_DATET=\
	".\include\datetime.h"\
	".\include\log.h"\
	".\include\mstream.h"\
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
	

"$(INTDIR)\datetime.obj" : $(SOURCE) $(DEP_CPP_DATET) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\EscLexer.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_ESCLE=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\charbuffer.hpp"\
	".\include\antlr\charscanner.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\commontoken.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\scannerexception.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenizer.hpp"\
	".\include\mstring.h"\
	".\src\EscLexer.hpp"\
	".\src\esclexertokentypes.hpp"\
	

"$(INTDIR)\EscLexer.obj" : $(SOURCE) $(DEP_CPP_ESCLE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_ESCLE=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\charbuffer.hpp"\
	".\include\antlr\charscanner.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\commontoken.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\scannerexception.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenizer.hpp"\
	".\include\mstring.h"\
	".\src\EscLexer.hpp"\
	".\src\esclexertokentypes.hpp"\
	

"$(INTDIR)\EscLexer.obj" : $(SOURCE) $(DEP_CPP_ESCLE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\EscParser.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_ESCPA=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astarray.hpp"\
	".\include\antlr\astfactory.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\astpair.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\llkparser.hpp"\
	".\include\antlr\noviablealtexception.hpp"\
	".\include\antlr\parser.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\semanticexception.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenbuffer.hpp"\
	".\include\antlr\tokenizer.hpp"\
	".\include\mstring.h"\
	".\src\esclexertokentypes.hpp"\
	".\src\EscParser.hpp"\
	

"$(INTDIR)\EscParser.obj" : $(SOURCE) $(DEP_CPP_ESCPA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_ESCPA=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astarray.hpp"\
	".\include\antlr\astfactory.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\astpair.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\llkparser.hpp"\
	".\include\antlr\noviablealtexception.hpp"\
	".\include\antlr\parser.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\semanticexception.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenbuffer.hpp"\
	".\include\antlr\tokenizer.hpp"\
	".\include\mstring.h"\
	".\src\esclexertokentypes.hpp"\
	".\src\EscParser.hpp"\
	

"$(INTDIR)\EscParser.obj" : $(SOURCE) $(DEP_CPP_ESCPA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\fileconf.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

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
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\basic_types.i"\
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
	{$(INCLUDE)}"ace\object_manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\os.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\fileconf.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"
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

SOURCE=.\src\antlr\LLkParser.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_LLKPA=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astarray.hpp"\
	".\include\antlr\astfactory.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\astpair.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\llkparser.hpp"\
	".\include\antlr\parser.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenbuffer.hpp"\
	".\include\antlr\tokenizer.hpp"\
	

"$(INTDIR)\LLkParser.obj" : $(SOURCE) $(DEP_CPP_LLKPA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_LLKPA=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astarray.hpp"\
	".\include\antlr\astfactory.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\astpair.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\llkparser.hpp"\
	".\include\antlr\parser.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenbuffer.hpp"\
	".\include\antlr\tokenizer.hpp"\
	

"$(INTDIR)\LLkParser.obj" : $(SOURCE) $(DEP_CPP_LLKPA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\lockable.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_LOCKA=\
	"..\..\program files\devstudio\vc\include\ace\atomic_op.i"\
	"..\..\program files\devstudio\vc\include\ace\containers.h"\
	"..\..\program files\devstudio\vc\include\ace\containers.i"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.h"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.i"\
	"..\..\program files\devstudio\vc\include\ace\event_handler.h"\
	"..\..\program files\devstudio\vc\include\ace\event_handler.i"\
	"..\..\program files\devstudio\vc\include\ace\free_list.cpp"\
	"..\..\program files\devstudio\vc\include\ace\free_list.h"\
	"..\..\program files\devstudio\vc\include\ace\free_list.i"\
	"..\..\program files\devstudio\vc\include\ace\handle_set.h"\
	"..\..\program files\devstudio\vc\include\ace\handle_set.i"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.cpp"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\io_cntl_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc.i"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.i"\
	"..\..\program files\devstudio\vc\include\ace\mem_map.h"\
	"..\..\program files\devstudio\vc\include\ace\mem_map.i"\
	"..\..\program files\devstudio\vc\include\ace\memory_pool.h"\
	"..\..\program files\devstudio\vc\include\ace\memory_pool.i"\
	"..\..\program files\devstudio\vc\include\ace\message_block.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block.i"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.i"\
	"..\..\program files\devstudio\vc\include\ace\message_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue.i"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\module.cpp"\
	"..\..\program files\devstudio\vc\include\ace\module.h"\
	"..\..\program files\devstudio\vc\include\ace\module.i"\
	"..\..\program files\devstudio\vc\include\ace\reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\reactor.i"\
	"..\..\program files\devstudio\vc\include\ace\reactor_impl.h"\
	"..\..\program files\devstudio\vc\include\ace\service_config.h"\
	"..\..\program files\devstudio\vc\include\ace\service_config.i"\
	"..\..\program files\devstudio\vc\include\ace\service_object.h"\
	"..\..\program files\devstudio\vc\include\ace\service_object.i"\
	"..\..\program files\devstudio\vc\include\ace\service_repository.h"\
	"..\..\program files\devstudio\vc\include\ace\service_repository.i"\
	"..\..\program files\devstudio\vc\include\ace\service_types.h"\
	"..\..\program files\devstudio\vc\include\ace\service_types.i"\
	"..\..\program files\devstudio\vc\include\ace\shared_object.h"\
	"..\..\program files\devstudio\vc\include\ace\shared_object.i"\
	"..\..\program files\devstudio\vc\include\ace\signal.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.i"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.cpp"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.h"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.i"\
	"..\..\program files\devstudio\vc\include\ace\svc_conf_tokens.h"\
	"..\..\program files\devstudio\vc\include\ace\synch.h"\
	"..\..\program files\devstudio\vc\include\ace\synch.i"\
	"..\..\program files\devstudio\vc\include\ace\synch_options.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_options.i"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.i"\
	"..\..\program files\devstudio\vc\include\ace\task.h"\
	"..\..\program files\devstudio\vc\include\ace\task.i"\
	"..\..\program files\devstudio\vc\include\ace\task_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\task_t.h"\
	"..\..\program files\devstudio\vc\include\ace\task_t.i"\
	"..\..\program files\devstudio\vc\include\ace\thread.h"\
	"..\..\program files\devstudio\vc\include\ace\thread.i"\
	"..\..\program files\devstudio\vc\include\ace\thread_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\thread_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.i"\
	".\include\lockable.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\basic_types.i"\
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
	{$(INCLUDE)}"ace\object_manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\os.i"\
	{$(INCLUDE)}"ace\sstring.h"\
	{$(INCLUDE)}"ace\sstring.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\lockable.obj" : $(SOURCE) $(DEP_CPP_LOCKA) "$(INTDIR)"
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
DEP_CPP_LOG_C=\
	".\include\log.h"\
	".\include\mstring.h"\
	

"$(INTDIR)\log.obj" : $(SOURCE) $(DEP_CPP_LOG_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\magick.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_MAGIC=\
	"..\..\program files\devstudio\vc\include\ace\atomic_op.i"\
	"..\..\program files\devstudio\vc\include\ace\containers.h"\
	"..\..\program files\devstudio\vc\include\ace\containers.i"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.h"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.i"\
	"..\..\program files\devstudio\vc\include\ace\event_handler.h"\
	"..\..\program files\devstudio\vc\include\ace\event_handler.i"\
	"..\..\program files\devstudio\vc\include\ace\free_list.cpp"\
	"..\..\program files\devstudio\vc\include\ace\free_list.h"\
	"..\..\program files\devstudio\vc\include\ace\free_list.i"\
	"..\..\program files\devstudio\vc\include\ace\handle_set.h"\
	"..\..\program files\devstudio\vc\include\ace\handle_set.i"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.cpp"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\io_cntl_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc.i"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.i"\
	"..\..\program files\devstudio\vc\include\ace\mem_map.h"\
	"..\..\program files\devstudio\vc\include\ace\mem_map.i"\
	"..\..\program files\devstudio\vc\include\ace\memory_pool.h"\
	"..\..\program files\devstudio\vc\include\ace\memory_pool.i"\
	"..\..\program files\devstudio\vc\include\ace\message_block.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block.i"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.i"\
	"..\..\program files\devstudio\vc\include\ace\message_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue.i"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\module.cpp"\
	"..\..\program files\devstudio\vc\include\ace\module.h"\
	"..\..\program files\devstudio\vc\include\ace\module.i"\
	"..\..\program files\devstudio\vc\include\ace\reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\reactor.i"\
	"..\..\program files\devstudio\vc\include\ace\reactor_impl.h"\
	"..\..\program files\devstudio\vc\include\ace\service_config.h"\
	"..\..\program files\devstudio\vc\include\ace\service_config.i"\
	"..\..\program files\devstudio\vc\include\ace\service_object.h"\
	"..\..\program files\devstudio\vc\include\ace\service_object.i"\
	"..\..\program files\devstudio\vc\include\ace\service_repository.h"\
	"..\..\program files\devstudio\vc\include\ace\service_repository.i"\
	"..\..\program files\devstudio\vc\include\ace\service_types.h"\
	"..\..\program files\devstudio\vc\include\ace\service_types.i"\
	"..\..\program files\devstudio\vc\include\ace\shared_object.h"\
	"..\..\program files\devstudio\vc\include\ace\shared_object.i"\
	"..\..\program files\devstudio\vc\include\ace\signal.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.i"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.cpp"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.h"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.i"\
	"..\..\program files\devstudio\vc\include\ace\svc_conf_tokens.h"\
	"..\..\program files\devstudio\vc\include\ace\synch.h"\
	"..\..\program files\devstudio\vc\include\ace\synch.i"\
	"..\..\program files\devstudio\vc\include\ace\synch_options.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_options.i"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.i"\
	"..\..\program files\devstudio\vc\include\ace\task.h"\
	"..\..\program files\devstudio\vc\include\ace\task.i"\
	"..\..\program files\devstudio\vc\include\ace\task_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\task_t.h"\
	"..\..\program files\devstudio\vc\include\ace\task_t.i"\
	"..\..\program files\devstudio\vc\include\ace\thread.h"\
	"..\..\program files\devstudio\vc\include\ace\thread.i"\
	"..\..\program files\devstudio\vc\include\ace\thread_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\thread_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.i"\
	"..\bob\bob.h"\
	"..\bob\bobexp.h"\
	"..\bob\compiler.h"\
	"..\bob\eval.h"\
	"..\bob\execute.h"\
	"..\bob\function.h"\
	"..\bob\objects.h"\
	"..\bob\streams.h"\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astarray.hpp"\
	".\include\antlr\astfactory.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\astpair.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\charbuffer.hpp"\
	".\include\antlr\charscanner.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\commontoken.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\llkparser.hpp"\
	".\include\antlr\parser.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\scannerexception.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenbuffer.hpp"\
	".\include\antlr\tokenizer.hpp"\
	".\include\bob.hpp"\
	".\include\confbase.h"\
	".\include\fileconf.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\src\EscLexer.hpp"\
	".\src\EscParser.hpp"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\basic_types.i"\
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
	{$(INCLUDE)}"ace\object_manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\os.i"\
	{$(INCLUDE)}"ace\sstring.h"\
	{$(INCLUDE)}"ace\sstring.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\magick.obj" : $(SOURCE) $(DEP_CPP_MAGIC) "$(INTDIR)"
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
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astarray.hpp"\
	".\include\antlr\astfactory.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\astpair.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\charbuffer.hpp"\
	".\include\antlr\charscanner.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\commontoken.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\llkparser.hpp"\
	".\include\antlr\parser.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\scannerexception.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenbuffer.hpp"\
	".\include\antlr\tokenizer.hpp"\
	".\include\bob.hpp"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\language.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\src\EscLexer.hpp"\
	".\src\EscParser.hpp"\
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

DEP_CPP_MAIN_=\
	"..\..\program files\devstudio\vc\include\ace\atomic_op.i"\
	"..\..\program files\devstudio\vc\include\ace\containers.h"\
	"..\..\program files\devstudio\vc\include\ace\containers.i"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.h"\
	"..\..\program files\devstudio\vc\include\ace\containers_t.i"\
	"..\..\program files\devstudio\vc\include\ace\event_handler.h"\
	"..\..\program files\devstudio\vc\include\ace\event_handler.i"\
	"..\..\program files\devstudio\vc\include\ace\free_list.cpp"\
	"..\..\program files\devstudio\vc\include\ace\free_list.h"\
	"..\..\program files\devstudio\vc\include\ace\free_list.i"\
	"..\..\program files\devstudio\vc\include\ace\handle_set.h"\
	"..\..\program files\devstudio\vc\include\ace\handle_set.i"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.cpp"\
	"..\..\program files\devstudio\vc\include\ace\hash_map_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\io_cntl_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc.i"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.i"\
	"..\..\program files\devstudio\vc\include\ace\mem_map.h"\
	"..\..\program files\devstudio\vc\include\ace\mem_map.i"\
	"..\..\program files\devstudio\vc\include\ace\memory_pool.h"\
	"..\..\program files\devstudio\vc\include\ace\memory_pool.i"\
	"..\..\program files\devstudio\vc\include\ace\message_block.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block.i"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_block_t.i"\
	"..\..\program files\devstudio\vc\include\ace\message_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue.i"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\message_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\module.cpp"\
	"..\..\program files\devstudio\vc\include\ace\module.h"\
	"..\..\program files\devstudio\vc\include\ace\module.i"\
	"..\..\program files\devstudio\vc\include\ace\reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\reactor.i"\
	"..\..\program files\devstudio\vc\include\ace\reactor_impl.h"\
	"..\..\program files\devstudio\vc\include\ace\service_config.h"\
	"..\..\program files\devstudio\vc\include\ace\service_config.i"\
	"..\..\program files\devstudio\vc\include\ace\service_object.h"\
	"..\..\program files\devstudio\vc\include\ace\service_object.i"\
	"..\..\program files\devstudio\vc\include\ace\service_repository.h"\
	"..\..\program files\devstudio\vc\include\ace\service_repository.i"\
	"..\..\program files\devstudio\vc\include\ace\service_types.h"\
	"..\..\program files\devstudio\vc\include\ace\service_types.i"\
	"..\..\program files\devstudio\vc\include\ace\shared_object.h"\
	"..\..\program files\devstudio\vc\include\ace\shared_object.i"\
	"..\..\program files\devstudio\vc\include\ace\signal.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.i"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.cpp"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.h"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.i"\
	"..\..\program files\devstudio\vc\include\ace\svc_conf_tokens.h"\
	"..\..\program files\devstudio\vc\include\ace\synch.h"\
	"..\..\program files\devstudio\vc\include\ace\synch.i"\
	"..\..\program files\devstudio\vc\include\ace\synch_options.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_options.i"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.h"\
	"..\..\program files\devstudio\vc\include\ace\synch_t.i"\
	"..\..\program files\devstudio\vc\include\ace\task.h"\
	"..\..\program files\devstudio\vc\include\ace\task.i"\
	"..\..\program files\devstudio\vc\include\ace\task_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\task_t.h"\
	"..\..\program files\devstudio\vc\include\ace\task_t.i"\
	"..\..\program files\devstudio\vc\include\ace\thread.h"\
	"..\..\program files\devstudio\vc\include\ace\thread.i"\
	"..\..\program files\devstudio\vc\include\ace\thread_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\thread_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.h"\
	"..\..\program files\devstudio\vc\include\ace\timer_queue_t.i"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.i"\
	"..\bob\bob.h"\
	"..\bob\bobexp.h"\
	"..\bob\compiler.h"\
	"..\bob\eval.h"\
	"..\bob\execute.h"\
	"..\bob\function.h"\
	"..\bob\objects.h"\
	"..\bob\streams.h"\
	".\include\bob.hpp"\
	".\include\confbase.h"\
	".\include\fileconf.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\basic_types.i"\
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
	{$(INCLUDE)}"ace\object_manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\os.i"\
	{$(INCLUDE)}"ace\sstring.h"\
	{$(INCLUDE)}"ace\sstring.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"
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
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
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

SOURCE=.\src\antlr\MismatchedTokenException.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_MISMA=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\mismatchedtokenexception.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\string.hpp"\
	".\include\antlr\token.hpp"\
	

"$(INTDIR)\MismatchedTokenException.obj" : $(SOURCE) $(DEP_CPP_MISMA)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MISMA=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\mismatchedtokenexception.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\string.hpp"\
	".\include\antlr\token.hpp"\
	

"$(INTDIR)\MismatchedTokenException.obj" : $(SOURCE) $(DEP_CPP_MISMA)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\mstream.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_MSTRE=\
	"..\..\program files\devstudio\vc\include\zconf.h"\
	"..\..\program files\devstudio\vc\include\zlib.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\utils.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\basic_types.i"\
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
	{$(INCLUDE)}"ace\object_manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\os.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\mstream.obj" : $(SOURCE) $(DEP_CPP_MSTRE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MSTRE=\
	"..\..\program files\devstudio\vc\include\zconf.h"\
	"..\..\program files\devstudio\vc\include\zlib.h"\
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
	

"$(INTDIR)\mstream.obj" : $(SOURCE) $(DEP_CPP_MSTRE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\mstring.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_MSTRI=\
	".\include\mstring.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\basic_types.i"\
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
	{$(INCLUDE)}"ace\object_manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\os.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

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

SOURCE=.\src\antlr\NoViableAltException.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_NOVIA=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\noviablealtexception.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\string.hpp"\
	".\include\antlr\token.hpp"\
	

"$(INTDIR)\NoViableAltException.obj" : $(SOURCE) $(DEP_CPP_NOVIA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_NOVIA=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\noviablealtexception.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\string.hpp"\
	".\include\antlr\token.hpp"\
	

"$(INTDIR)\NoViableAltException.obj" : $(SOURCE) $(DEP_CPP_NOVIA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\Parser.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_PARSE=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astarray.hpp"\
	".\include\antlr\astfactory.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\astpair.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\mismatchedtokenexception.hpp"\
	".\include\antlr\parser.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenbuffer.hpp"\
	".\include\antlr\tokenizer.hpp"\
	

"$(INTDIR)\Parser.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_PARSE=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astarray.hpp"\
	".\include\antlr\astfactory.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\astpair.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\mismatchedtokenexception.hpp"\
	".\include\antlr\parser.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenbuffer.hpp"\
	".\include\antlr\tokenizer.hpp"\
	

"$(INTDIR)\Parser.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\ParserException.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_PARSER=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\parserexception.hpp"\
	

"$(INTDIR)\ParserException.obj" : $(SOURCE) $(DEP_CPP_PARSER) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_PARSER=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\parserexception.hpp"\
	

"$(INTDIR)\ParserException.obj" : $(SOURCE) $(DEP_CPP_PARSER) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\ScannerException.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_SCANN=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\scannerexception.hpp"\
	".\include\antlr\string.hpp"\
	

"$(INTDIR)\ScannerException.obj" : $(SOURCE) $(DEP_CPP_SCANN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_SCANN=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\scannerexception.hpp"\
	".\include\antlr\string.hpp"\
	

"$(INTDIR)\ScannerException.obj" : $(SOURCE) $(DEP_CPP_SCANN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\String.cpp
DEP_CPP_STRIN=\
	".\include\antlr\config.hpp"\
	".\include\antlr\string.hpp"\
	

"$(INTDIR)\String.obj" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\textfile.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_TEXTF=\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\basic_types.i"\
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
	{$(INCLUDE)}"ace\object_manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\os.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	

"$(INTDIR)\textfile.obj" : $(SOURCE) $(DEP_CPP_TEXTF) "$(INTDIR)"
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

SOURCE=.\src\antlr\Token.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_TOKEN=\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\string.hpp"\
	".\include\antlr\token.hpp"\
	

"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_TOKEN=\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\string.hpp"\
	".\include\antlr\token.hpp"\
	

"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\TokenBuffer.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_TOKENB=\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenbuffer.hpp"\
	".\include\antlr\tokenizer.hpp"\
	

"$(INTDIR)\TokenBuffer.obj" : $(SOURCE) $(DEP_CPP_TOKENB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_TOKENB=\
	".\include\antlr\circularqueue.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\tokenbuffer.hpp"\
	".\include\antlr\tokenizer.hpp"\
	

"$(INTDIR)\TokenBuffer.obj" : $(SOURCE) $(DEP_CPP_TOKENB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\trace.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_TRACE=\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\trace.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\Auto_Ptr.cpp"\
	{$(INCLUDE)}"ace\Auto_Ptr.h"\
	{$(INCLUDE)}"ace\Auto_Ptr.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\basic_types.i"\
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
	{$(INCLUDE)}"ace\object_manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\os.i"\
	{$(INCLUDE)}"ace\sstring.h"\
	{$(INCLUDE)}"ace\sstring.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	{$(INCLUDE)}"ace\ws2tcpip.h"\
	

"$(INTDIR)\trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_TRACE=\
	".\include\datetime.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\trace.h"\
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
	

"$(INTDIR)\trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\TreeParser.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_TREEP=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astarray.hpp"\
	".\include\antlr\astfactory.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\astnulltype.hpp"\
	".\include\antlr\astpair.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\mismatchedtokenexception.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\treeparser.hpp"\
	

"$(INTDIR)\TreeParser.obj" : $(SOURCE) $(DEP_CPP_TREEP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_TREEP=\
	".\include\antlr\antlrexception.hpp"\
	".\include\antlr\ast.hpp"\
	".\include\antlr\astarray.hpp"\
	".\include\antlr\astfactory.hpp"\
	".\include\antlr\astnode.hpp"\
	".\include\antlr\astnulltype.hpp"\
	".\include\antlr\astpair.hpp"\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\config.hpp"\
	".\include\antlr\mismatchedtokenexception.hpp"\
	".\include\antlr\parserexception.hpp"\
	".\include\antlr\refcount.hpp"\
	".\include\antlr\token.hpp"\
	".\include\antlr\treeparser.hpp"\
	

"$(INTDIR)\TreeParser.obj" : $(SOURCE) $(DEP_CPP_TREEP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\utils.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_UTILS=\
	".\include\log.h"\
	".\include\mstring.h"\
	".\include\utils.h"\
	

"$(INTDIR)\utils.obj" : $(SOURCE) $(DEP_CPP_UTILS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_UTILS=\
	".\include\log.h"\
	".\include\mstring.h"\
	".\include\utils.h"\
	

"$(INTDIR)\utils.obj" : $(SOURCE) $(DEP_CPP_UTILS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\variant.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_VARIA=\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\trace.h"\
	{$(INCLUDE)}"ace\ace.h"\
	{$(INCLUDE)}"ace\ace.i"\
	{$(INCLUDE)}"ace\Auto_Ptr.cpp"\
	{$(INCLUDE)}"ace\Auto_Ptr.h"\
	{$(INCLUDE)}"ace\Auto_Ptr.i"\
	{$(INCLUDE)}"ace\basic_types.h"\
	{$(INCLUDE)}"ace\basic_types.i"\
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
	{$(INCLUDE)}"ace\object_manager.i"\
	{$(INCLUDE)}"ace\os.h"\
	{$(INCLUDE)}"ace\os.i"\
	{$(INCLUDE)}"ace\sstring.h"\
	{$(INCLUDE)}"ace\sstring.i"\
	{$(INCLUDE)}"ace\streams.h"\
	{$(INCLUDE)}"ace\trace.h"\
	{$(INCLUDE)}"ace\ws2tcpip.h"\
	

"$(INTDIR)\variant.obj" : $(SOURCE) $(DEP_CPP_VARIA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_VARIA=\
	".\include\datetime.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\trace.h"\
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
	

"$(INTDIR)\variant.obj" : $(SOURCE) $(DEP_CPP_VARIA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

