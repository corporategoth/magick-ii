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

ALL : "m_bob - Win32 Release" "$(OUTDIR)\magick.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"m_bob - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ANTLRException.obj"
	-@erase "$(INTDIR)\AST.obj"
	-@erase "$(INTDIR)\ASTFactory.obj"
	-@erase "$(INTDIR)\base.obj"
	-@erase "$(INTDIR)\BitSet.obj"
	-@erase "$(INTDIR)\bob.obj"
	-@erase "$(INTDIR)\cbc_cksm.obj"
	-@erase "$(INTDIR)\cfb_enc.obj"
	-@erase "$(INTDIR)\chanserv.obj"
	-@erase "$(INTDIR)\CharBuffer.obj"
	-@erase "$(INTDIR)\CharScanner.obj"
	-@erase "$(INTDIR)\CommonToken.obj"
	-@erase "$(INTDIR)\confbase.obj"
	-@erase "$(INTDIR)\cryptstream.obj"
	-@erase "$(INTDIR)\datetime.obj"
	-@erase "$(INTDIR)\dccengine.obj"
	-@erase "$(INTDIR)\des_enc.obj"
	-@erase "$(INTDIR)\EscLexer.obj"
	-@erase "$(INTDIR)\EscParser.obj"
	-@erase "$(INTDIR)\fileconf.obj"
	-@erase "$(INTDIR)\ircsocket.obj"
	-@erase "$(INTDIR)\LLkParser.obj"
	-@erase "$(INTDIR)\lockable.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\magick.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\MismatchedTokenException.obj"
	-@erase "$(INTDIR)\mstream.obj"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\nickserv.obj"
	-@erase "$(INTDIR)\NoViableAltException.obj"
	-@erase "$(INTDIR)\Parser.obj"
	-@erase "$(INTDIR)\ParserException.obj"
	-@erase "$(INTDIR)\ScannerException.obj"
	-@erase "$(INTDIR)\set_key.obj"
	-@erase "$(INTDIR)\str2key.obj"
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

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "include\bob" /I "include" /I\
 "..\ace_wrappers" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D\
 "DES_UNROLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\magick.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib ace.lib ..\cryptlib21\Release\cl32.lib zlib.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)\magick.pdb" /machine:I386\
 /out:"$(OUTDIR)\magick.exe" /libpath:"..\ace_wrappers\ace" 
LINK32_OBJS= \
	"$(INTDIR)\ANTLRException.obj" \
	"$(INTDIR)\AST.obj" \
	"$(INTDIR)\ASTFactory.obj" \
	"$(INTDIR)\base.obj" \
	"$(INTDIR)\BitSet.obj" \
	"$(INTDIR)\bob.obj" \
	"$(INTDIR)\cbc_cksm.obj" \
	"$(INTDIR)\cfb_enc.obj" \
	"$(INTDIR)\chanserv.obj" \
	"$(INTDIR)\CharBuffer.obj" \
	"$(INTDIR)\CharScanner.obj" \
	"$(INTDIR)\CommonToken.obj" \
	"$(INTDIR)\confbase.obj" \
	"$(INTDIR)\cryptstream.obj" \
	"$(INTDIR)\datetime.obj" \
	"$(INTDIR)\dccengine.obj" \
	"$(INTDIR)\des_enc.obj" \
	"$(INTDIR)\EscLexer.obj" \
	"$(INTDIR)\EscParser.obj" \
	"$(INTDIR)\fileconf.obj" \
	"$(INTDIR)\ircsocket.obj" \
	"$(INTDIR)\LLkParser.obj" \
	"$(INTDIR)\lockable.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\magick.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\MismatchedTokenException.obj" \
	"$(INTDIR)\mstream.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\nickserv.obj" \
	"$(INTDIR)\NoViableAltException.obj" \
	"$(INTDIR)\Parser.obj" \
	"$(INTDIR)\ParserException.obj" \
	"$(INTDIR)\ScannerException.obj" \
	"$(INTDIR)\set_key.obj" \
	"$(INTDIR)\str2key.obj" \
	"$(INTDIR)\String.obj" \
	"$(INTDIR)\textfile.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\TokenBuffer.obj" \
	"$(INTDIR)\trace.obj" \
	"$(INTDIR)\TreeParser.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\variant.obj" \
	".\src\bob\Release\bob.lib"

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

ALL : "m_bob - Win32 Debug" "$(OUTDIR)\magick.exe" "$(OUTDIR)\magick.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"m_bob - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ANTLRException.obj"
	-@erase "$(INTDIR)\ANTLRException.sbr"
	-@erase "$(INTDIR)\AST.obj"
	-@erase "$(INTDIR)\AST.sbr"
	-@erase "$(INTDIR)\ASTFactory.obj"
	-@erase "$(INTDIR)\ASTFactory.sbr"
	-@erase "$(INTDIR)\base.obj"
	-@erase "$(INTDIR)\base.sbr"
	-@erase "$(INTDIR)\BitSet.obj"
	-@erase "$(INTDIR)\BitSet.sbr"
	-@erase "$(INTDIR)\bob.obj"
	-@erase "$(INTDIR)\bob.sbr"
	-@erase "$(INTDIR)\cbc_cksm.obj"
	-@erase "$(INTDIR)\cbc_cksm.sbr"
	-@erase "$(INTDIR)\cfb_enc.obj"
	-@erase "$(INTDIR)\cfb_enc.sbr"
	-@erase "$(INTDIR)\chanserv.obj"
	-@erase "$(INTDIR)\chanserv.sbr"
	-@erase "$(INTDIR)\CharBuffer.obj"
	-@erase "$(INTDIR)\CharBuffer.sbr"
	-@erase "$(INTDIR)\CharScanner.obj"
	-@erase "$(INTDIR)\CharScanner.sbr"
	-@erase "$(INTDIR)\CommonToken.obj"
	-@erase "$(INTDIR)\CommonToken.sbr"
	-@erase "$(INTDIR)\confbase.obj"
	-@erase "$(INTDIR)\confbase.sbr"
	-@erase "$(INTDIR)\cryptstream.obj"
	-@erase "$(INTDIR)\cryptstream.sbr"
	-@erase "$(INTDIR)\datetime.obj"
	-@erase "$(INTDIR)\datetime.sbr"
	-@erase "$(INTDIR)\dccengine.obj"
	-@erase "$(INTDIR)\dccengine.sbr"
	-@erase "$(INTDIR)\des_enc.obj"
	-@erase "$(INTDIR)\des_enc.sbr"
	-@erase "$(INTDIR)\EscLexer.obj"
	-@erase "$(INTDIR)\EscLexer.sbr"
	-@erase "$(INTDIR)\EscParser.obj"
	-@erase "$(INTDIR)\EscParser.sbr"
	-@erase "$(INTDIR)\fileconf.obj"
	-@erase "$(INTDIR)\fileconf.sbr"
	-@erase "$(INTDIR)\ircsocket.obj"
	-@erase "$(INTDIR)\ircsocket.sbr"
	-@erase "$(INTDIR)\LLkParser.obj"
	-@erase "$(INTDIR)\LLkParser.sbr"
	-@erase "$(INTDIR)\lockable.obj"
	-@erase "$(INTDIR)\lockable.sbr"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log.sbr"
	-@erase "$(INTDIR)\magick.obj"
	-@erase "$(INTDIR)\magick.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\MismatchedTokenException.obj"
	-@erase "$(INTDIR)\MismatchedTokenException.sbr"
	-@erase "$(INTDIR)\mstream.obj"
	-@erase "$(INTDIR)\mstream.sbr"
	-@erase "$(INTDIR)\mstring.obj"
	-@erase "$(INTDIR)\mstring.sbr"
	-@erase "$(INTDIR)\nickserv.obj"
	-@erase "$(INTDIR)\nickserv.sbr"
	-@erase "$(INTDIR)\NoViableAltException.obj"
	-@erase "$(INTDIR)\NoViableAltException.sbr"
	-@erase "$(INTDIR)\Parser.obj"
	-@erase "$(INTDIR)\Parser.sbr"
	-@erase "$(INTDIR)\ParserException.obj"
	-@erase "$(INTDIR)\ParserException.sbr"
	-@erase "$(INTDIR)\ScannerException.obj"
	-@erase "$(INTDIR)\ScannerException.sbr"
	-@erase "$(INTDIR)\set_key.obj"
	-@erase "$(INTDIR)\set_key.sbr"
	-@erase "$(INTDIR)\str2key.obj"
	-@erase "$(INTDIR)\str2key.sbr"
	-@erase "$(INTDIR)\String.obj"
	-@erase "$(INTDIR)\String.sbr"
	-@erase "$(INTDIR)\textfile.obj"
	-@erase "$(INTDIR)\textfile.sbr"
	-@erase "$(INTDIR)\Token.obj"
	-@erase "$(INTDIR)\Token.sbr"
	-@erase "$(INTDIR)\TokenBuffer.obj"
	-@erase "$(INTDIR)\TokenBuffer.sbr"
	-@erase "$(INTDIR)\trace.obj"
	-@erase "$(INTDIR)\trace.sbr"
	-@erase "$(INTDIR)\TreeParser.obj"
	-@erase "$(INTDIR)\TreeParser.sbr"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\utils.sbr"
	-@erase "$(INTDIR)\variant.obj"
	-@erase "$(INTDIR)\variant.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\magick.bsc"
	-@erase "$(OUTDIR)\magick.exe"
	-@erase "$(OUTDIR)\magick.ilk"
	-@erase "$(OUTDIR)\magick.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Gf /I "include\bob" /I "include"\
 /I "..\ace_wrappers" /D "_DEBUG" /D "DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS"\
 /D "DES_UNROLL" /Fr"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\magick.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ANTLRException.sbr" \
	"$(INTDIR)\AST.sbr" \
	"$(INTDIR)\ASTFactory.sbr" \
	"$(INTDIR)\base.sbr" \
	"$(INTDIR)\BitSet.sbr" \
	"$(INTDIR)\bob.sbr" \
	"$(INTDIR)\cbc_cksm.sbr" \
	"$(INTDIR)\cfb_enc.sbr" \
	"$(INTDIR)\chanserv.sbr" \
	"$(INTDIR)\CharBuffer.sbr" \
	"$(INTDIR)\CharScanner.sbr" \
	"$(INTDIR)\CommonToken.sbr" \
	"$(INTDIR)\confbase.sbr" \
	"$(INTDIR)\cryptstream.sbr" \
	"$(INTDIR)\datetime.sbr" \
	"$(INTDIR)\dccengine.sbr" \
	"$(INTDIR)\des_enc.sbr" \
	"$(INTDIR)\EscLexer.sbr" \
	"$(INTDIR)\EscParser.sbr" \
	"$(INTDIR)\fileconf.sbr" \
	"$(INTDIR)\ircsocket.sbr" \
	"$(INTDIR)\LLkParser.sbr" \
	"$(INTDIR)\lockable.sbr" \
	"$(INTDIR)\log.sbr" \
	"$(INTDIR)\magick.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\MismatchedTokenException.sbr" \
	"$(INTDIR)\mstream.sbr" \
	"$(INTDIR)\mstring.sbr" \
	"$(INTDIR)\nickserv.sbr" \
	"$(INTDIR)\NoViableAltException.sbr" \
	"$(INTDIR)\Parser.sbr" \
	"$(INTDIR)\ParserException.sbr" \
	"$(INTDIR)\ScannerException.sbr" \
	"$(INTDIR)\set_key.sbr" \
	"$(INTDIR)\str2key.sbr" \
	"$(INTDIR)\String.sbr" \
	"$(INTDIR)\textfile.sbr" \
	"$(INTDIR)\Token.sbr" \
	"$(INTDIR)\TokenBuffer.sbr" \
	"$(INTDIR)\trace.sbr" \
	"$(INTDIR)\TreeParser.sbr" \
	"$(INTDIR)\utils.sbr" \
	"$(INTDIR)\variant.sbr"

"$(OUTDIR)\magick.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib aced.lib ..\cryptlib21\Debug\cl32d.lib zlib.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\magick.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)\magick.exe" /pdbtype:sept\
 /libpath:"..\ace_wrappers\ace" 
LINK32_OBJS= \
	"$(INTDIR)\ANTLRException.obj" \
	"$(INTDIR)\AST.obj" \
	"$(INTDIR)\ASTFactory.obj" \
	"$(INTDIR)\base.obj" \
	"$(INTDIR)\BitSet.obj" \
	"$(INTDIR)\bob.obj" \
	"$(INTDIR)\cbc_cksm.obj" \
	"$(INTDIR)\cfb_enc.obj" \
	"$(INTDIR)\chanserv.obj" \
	"$(INTDIR)\CharBuffer.obj" \
	"$(INTDIR)\CharScanner.obj" \
	"$(INTDIR)\CommonToken.obj" \
	"$(INTDIR)\confbase.obj" \
	"$(INTDIR)\cryptstream.obj" \
	"$(INTDIR)\datetime.obj" \
	"$(INTDIR)\dccengine.obj" \
	"$(INTDIR)\des_enc.obj" \
	"$(INTDIR)\EscLexer.obj" \
	"$(INTDIR)\EscParser.obj" \
	"$(INTDIR)\fileconf.obj" \
	"$(INTDIR)\ircsocket.obj" \
	"$(INTDIR)\LLkParser.obj" \
	"$(INTDIR)\lockable.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\magick.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\MismatchedTokenException.obj" \
	"$(INTDIR)\mstream.obj" \
	"$(INTDIR)\mstring.obj" \
	"$(INTDIR)\nickserv.obj" \
	"$(INTDIR)\NoViableAltException.obj" \
	"$(INTDIR)\Parser.obj" \
	"$(INTDIR)\ParserException.obj" \
	"$(INTDIR)\ScannerException.obj" \
	"$(INTDIR)\set_key.obj" \
	"$(INTDIR)\str2key.obj" \
	"$(INTDIR)\String.obj" \
	"$(INTDIR)\textfile.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\TokenBuffer.obj" \
	"$(INTDIR)\trace.obj" \
	"$(INTDIR)\TreeParser.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\variant.obj" \
	".\src\bob\Debug\bob.lib"

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

!IF  "$(CFG)" == "magick - Win32 Release"

"m_bob - Win32 Release" : 
   cd ".\src\bob"
   $(MAKE) /$(MAKEFLAGS) /F ".\m_bob.mak" CFG="m_bob - Win32 Release" 
   cd "..\.."

"m_bob - Win32 ReleaseCLEAN" : 
   cd ".\src\bob"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F ".\m_bob.mak" CFG="m_bob - Win32 Release"\
 RECURSE=1 
   cd "..\.."

!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

"m_bob - Win32 Debug" : 
   cd ".\src\bob"
   $(MAKE) /$(MAKEFLAGS) /F ".\m_bob.mak" CFG="m_bob - Win32 Debug" 
   cd "..\.."

"m_bob - Win32 DebugCLEAN" : 
   cd ".\src\bob"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F ".\m_bob.mak" CFG="m_bob - Win32 Debug"\
 RECURSE=1 
   cd "..\.."

!ENDIF 

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
	

"$(INTDIR)\ANTLRException.obj"	"$(INTDIR)\ANTLRException.sbr" : $(SOURCE)\
 $(DEP_CPP_ANTLR) "$(INTDIR)"
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
	

"$(INTDIR)\AST.obj"	"$(INTDIR)\AST.sbr" : $(SOURCE) $(DEP_CPP_AST_C)\
 "$(INTDIR)"
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
	

"$(INTDIR)\ASTFactory.obj"	"$(INTDIR)\ASTFactory.sbr" : $(SOURCE)\
 $(DEP_CPP_ASTFA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\base.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_BASE_=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Addr.h"\
	"..\ace_wrappers\ace\Addr.i"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\Basic_Types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config-WinCE.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Connector.cpp"\
	"..\ace_wrappers\ace\Connector.h"\
	"..\ace_wrappers\ace\Connector.i"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers.i"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Dynamic.h"\
	"..\ace_wrappers\ace\Dynamic.i"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Event_Handler.i"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Functor.h"\
	"..\ace_wrappers\ace\Functor.i"\
	"..\ace_wrappers\ace\Functor_T.cpp"\
	"..\ace_wrappers\ace\Functor_T.h"\
	"..\ace_wrappers\ace\Functor_T.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\Handle_Set.i"\
	"..\ace_wrappers\ace\Hash_Map_Manager.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager.i"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.cpp"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\INET_Addr.h"\
	"..\ace_wrappers\ace\INET_Addr.i"\
	"..\ace_wrappers\ace\IO_Cntl_Msg.h"\
	"..\ace_wrappers\ace\iosfwd.h"\
	"..\ace_wrappers\ace\IPC_SAP.h"\
	"..\ace_wrappers\ace\IPC_SAP.i"\
	"..\ace_wrappers\ace\Local_Tokens.h"\
	"..\ace_wrappers\ace\Local_Tokens.i"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc.h"\
	"..\ace_wrappers\ace\Malloc.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Mem_Map.i"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Memory_Pool.i"\
	"..\ace_wrappers\ace\Message_Block.h"\
	"..\ace_wrappers\ace\Message_Block.i"\
	"..\ace_wrappers\ace\Message_Block_T.cpp"\
	"..\ace_wrappers\ace\Message_Block_T.h"\
	"..\ace_wrappers\ace\Message_Block_T.i"\
	"..\ace_wrappers\ace\Message_Queue.h"\
	"..\ace_wrappers\ace\Message_Queue.i"\
	"..\ace_wrappers\ace\Message_Queue_T.cpp"\
	"..\ace_wrappers\ace\Message_Queue_T.h"\
	"..\ace_wrappers\ace\Message_Queue_T.i"\
	"..\ace_wrappers\ace\Module.cpp"\
	"..\ace_wrappers\ace\Module.h"\
	"..\ace_wrappers\ace\Module.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\Object_Manager.i"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\OS.i"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Reactor.i"\
	"..\ace_wrappers\ace\Reactor_Impl.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Config.i"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Service_Object.i"\
	"..\ace_wrappers\ace\Service_Repository.h"\
	"..\ace_wrappers\ace\Service_Repository.i"\
	"..\ace_wrappers\ace\Service_Types.h"\
	"..\ace_wrappers\ace\Service_Types.i"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\Shared_Object.i"\
	"..\ace_wrappers\ace\Signal.h"\
	"..\ace_wrappers\ace\Signal.i"\
	"..\ace_wrappers\ace\Singleton.cpp"\
	"..\ace_wrappers\ace\Singleton.h"\
	"..\ace_wrappers\ace\Singleton.i"\
	"..\ace_wrappers\ace\SOCK.h"\
	"..\ace_wrappers\ace\SOCK.i"\
	"..\ace_wrappers\ace\SOCK_Connector.h"\
	"..\ace_wrappers\ace\SOCK_Connector.i"\
	"..\ace_wrappers\ace\SOCK_IO.h"\
	"..\ace_wrappers\ace\SOCK_IO.i"\
	"..\ace_wrappers\ace\SOCK_Stream.h"\
	"..\ace_wrappers\ace\SOCK_Stream.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\SString.i"\
	"..\ace_wrappers\ace\Strategies.h"\
	"..\ace_wrappers\ace\Strategies.i"\
	"..\ace_wrappers\ace\Strategies_T.cpp"\
	"..\ace_wrappers\ace\Strategies_T.h"\
	"..\ace_wrappers\ace\Strategies_T.i"\
	"..\ace_wrappers\ace\Stream_Modules.cpp"\
	"..\ace_wrappers\ace\Stream_Modules.h"\
	"..\ace_wrappers\ace\Stream_Modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Complex.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Complex.i"\
	"..\ace_wrappers\ace\SV_Semaphore_Simple.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Simple.i"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Svc_Handler.cpp"\
	"..\ace_wrappers\ace\Svc_Handler.h"\
	"..\ace_wrappers\ace\Svc_Handler.i"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch.i"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_Options.i"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Task.h"\
	"..\ace_wrappers\ace\Task.i"\
	"..\ace_wrappers\ace\Task_T.cpp"\
	"..\ace_wrappers\ace\Task_T.h"\
	"..\ace_wrappers\ace\Task_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread.i"\
	"..\ace_wrappers\ace\Thread_Manager.h"\
	"..\ace_wrappers\ace\Thread_Manager.i"\
	"..\ace_wrappers\ace\Time_Value.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.i"\
	"..\ace_wrappers\ace\ws2tcpip.h"\
	".\include\base.h"\
	".\include\bob.hpp"\
	".\include\bob\bob.h"\
	".\include\bob\bobexp.h"\
	".\include\bob\compiler.h"\
	".\include\bob\eval.h"\
	".\include\bob\execute.h"\
	".\include\bob\function.h"\
	".\include\bob\objects.h"\
	".\include\bob\streams.h"\
	".\include\chanserv.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\ircsocket.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\nickserv.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\base.obj" : $(SOURCE) $(DEP_CPP_BASE_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_BASE_=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Addr.h"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Connector.cpp"\
	"..\ace_wrappers\ace\Connector.h"\
	"..\ace_wrappers\ace\Connector.i"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Dynamic.h"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Functor.h"\
	"..\ace_wrappers\ace\Functor_T.cpp"\
	"..\ace_wrappers\ace\Functor_T.h"\
	"..\ace_wrappers\ace\Functor_T.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.cpp"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\INET_Addr.h"\
	"..\ace_wrappers\ace\IO_Cntl_Msg.h"\
	"..\ace_wrappers\ace\IPC_SAP.h"\
	"..\ace_wrappers\ace\IPC_SAP.i"\
	"..\ace_wrappers\ace\Local_Tokens.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Message_Block.h"\
	"..\ace_wrappers\ace\Message_Block_T.cpp"\
	"..\ace_wrappers\ace\Message_Block_T.h"\
	"..\ace_wrappers\ace\Message_Block_T.i"\
	"..\ace_wrappers\ace\Message_Queue.h"\
	"..\ace_wrappers\ace\Message_Queue_T.cpp"\
	"..\ace_wrappers\ace\Message_Queue_T.h"\
	"..\ace_wrappers\ace\Message_Queue_T.i"\
	"..\ace_wrappers\ace\Module.cpp"\
	"..\ace_wrappers\ace\Module.h"\
	"..\ace_wrappers\ace\Module.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Reactor_Impl.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Service_Repository.h"\
	"..\ace_wrappers\ace\Service_Types.h"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\Singleton.cpp"\
	"..\ace_wrappers\ace\Singleton.h"\
	"..\ace_wrappers\ace\Singleton.i"\
	"..\ace_wrappers\ace\SOCK.h"\
	"..\ace_wrappers\ace\SOCK.i"\
	"..\ace_wrappers\ace\SOCK_Connector.h"\
	"..\ace_wrappers\ace\SOCK_Connector.i"\
	"..\ace_wrappers\ace\SOCK_IO.h"\
	"..\ace_wrappers\ace\SOCK_IO.i"\
	"..\ace_wrappers\ace\SOCK_Stream.h"\
	"..\ace_wrappers\ace\SOCK_Stream.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\Strategies.h"\
	"..\ace_wrappers\ace\Strategies_T.cpp"\
	"..\ace_wrappers\ace\Strategies_T.h"\
	"..\ace_wrappers\ace\Strategies_T.i"\
	"..\ace_wrappers\ace\Stream_Modules.cpp"\
	"..\ace_wrappers\ace\Stream_Modules.h"\
	"..\ace_wrappers\ace\Stream_Modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Svc_Handler.cpp"\
	"..\ace_wrappers\ace\Svc_Handler.h"\
	"..\ace_wrappers\ace\Svc_Handler.i"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Task.h"\
	"..\ace_wrappers\ace\Task_T.cpp"\
	"..\ace_wrappers\ace\Task_T.h"\
	"..\ace_wrappers\ace\Task_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread_Manager.h"\
	"..\ace_wrappers\ace\Time_Value.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.h"\
	".\include\base.h"\
	".\include\bob.hpp"\
	".\include\bob\bob.h"\
	".\include\bob\bobexp.h"\
	".\include\bob\compiler.h"\
	".\include\bob\eval.h"\
	".\include\bob\execute.h"\
	".\include\bob\function.h"\
	".\include\bob\objects.h"\
	".\include\bob\streams.h"\
	".\include\chanserv.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\ircsocket.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\nickserv.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\base.obj"	"$(INTDIR)\base.sbr" : $(SOURCE) $(DEP_CPP_BASE_)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\BitSet.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_BITSE=\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\config.hpp"\
	

"$(INTDIR)\BitSet.obj" : $(SOURCE) $(DEP_CPP_BITSE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_BITSE=\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\config.hpp"\
	

"$(INTDIR)\BitSet.obj"	"$(INTDIR)\BitSet.sbr" : $(SOURCE) $(DEP_CPP_BITSE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\bob.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_BOB_C=\
	".\include\bob.hpp"\
	".\include\confbase.h"\
	".\include\fileconf.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	
NODEP_CPP_BOB_C=\
	"..\..\program files\devstudio\vc\include\ace\ace.h"\
	"..\..\program files\devstudio\vc\include\ace\ace.i"\
	"..\..\program files\devstudio\vc\include\ace\atomic_op.i"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.h"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.i"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-borland.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-common.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32.h"\
	"..\..\program files\devstudio\vc\include\ace\config.h"\
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
	"..\..\program files\devstudio\vc\include\ace\inc_user_config.h"\
	"..\..\program files\devstudio\vc\include\ace\io_cntl_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_priority.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.i"\
	"..\..\program files\devstudio\vc\include\ace\malloc.i"\
	"..\..\program files\devstudio\vc\include\ace\malloc_base.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.i"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.cpp"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.h"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.i"\
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
	"..\..\program files\devstudio\vc\include\ace\object_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\os.h"\
	"..\..\program files\devstudio\vc\include\ace\os.i"\
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
	"..\..\program files\devstudio\vc\include\ace\sstring.h"\
	"..\..\program files\devstudio\vc\include\ace\sstring.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.i"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.cpp"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.h"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.i"\
	"..\..\program files\devstudio\vc\include\ace\streams.h"\
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
	"..\..\program files\devstudio\vc\include\ace\trace.h"\
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
	

"$(INTDIR)\bob.obj" : $(SOURCE) $(DEP_CPP_BOB_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_BOB_C=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Addr.h"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Connector.cpp"\
	"..\ace_wrappers\ace\Connector.h"\
	"..\ace_wrappers\ace\Connector.i"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Dynamic.h"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Functor.h"\
	"..\ace_wrappers\ace\Functor_T.cpp"\
	"..\ace_wrappers\ace\Functor_T.h"\
	"..\ace_wrappers\ace\Functor_T.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.cpp"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\INET_Addr.h"\
	"..\ace_wrappers\ace\IO_Cntl_Msg.h"\
	"..\ace_wrappers\ace\IPC_SAP.h"\
	"..\ace_wrappers\ace\IPC_SAP.i"\
	"..\ace_wrappers\ace\Local_Tokens.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Message_Block.h"\
	"..\ace_wrappers\ace\Message_Block_T.cpp"\
	"..\ace_wrappers\ace\Message_Block_T.h"\
	"..\ace_wrappers\ace\Message_Block_T.i"\
	"..\ace_wrappers\ace\Message_Queue.h"\
	"..\ace_wrappers\ace\Message_Queue_T.cpp"\
	"..\ace_wrappers\ace\Message_Queue_T.h"\
	"..\ace_wrappers\ace\Message_Queue_T.i"\
	"..\ace_wrappers\ace\Module.cpp"\
	"..\ace_wrappers\ace\Module.h"\
	"..\ace_wrappers\ace\Module.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Reactor_Impl.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Service_Repository.h"\
	"..\ace_wrappers\ace\Service_Types.h"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\Singleton.cpp"\
	"..\ace_wrappers\ace\Singleton.h"\
	"..\ace_wrappers\ace\Singleton.i"\
	"..\ace_wrappers\ace\SOCK.h"\
	"..\ace_wrappers\ace\SOCK.i"\
	"..\ace_wrappers\ace\SOCK_Connector.h"\
	"..\ace_wrappers\ace\SOCK_Connector.i"\
	"..\ace_wrappers\ace\SOCK_IO.h"\
	"..\ace_wrappers\ace\SOCK_IO.i"\
	"..\ace_wrappers\ace\SOCK_Stream.h"\
	"..\ace_wrappers\ace\SOCK_Stream.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\Strategies.h"\
	"..\ace_wrappers\ace\Strategies_T.cpp"\
	"..\ace_wrappers\ace\Strategies_T.h"\
	"..\ace_wrappers\ace\Strategies_T.i"\
	"..\ace_wrappers\ace\Stream_Modules.cpp"\
	"..\ace_wrappers\ace\Stream_Modules.h"\
	"..\ace_wrappers\ace\Stream_Modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Svc_Handler.cpp"\
	"..\ace_wrappers\ace\Svc_Handler.h"\
	"..\ace_wrappers\ace\Svc_Handler.i"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Task.h"\
	"..\ace_wrappers\ace\Task_T.cpp"\
	"..\ace_wrappers\ace\Task_T.h"\
	"..\ace_wrappers\ace\Task_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread_Manager.h"\
	"..\ace_wrappers\ace\Time_Value.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.h"\
	".\include\base.h"\
	".\include\bob.hpp"\
	".\include\bob\bob.h"\
	".\include\bob\bobexp.h"\
	".\include\bob\compiler.h"\
	".\include\bob\eval.h"\
	".\include\bob\execute.h"\
	".\include\bob\function.h"\
	".\include\bob\objects.h"\
	".\include\bob\streams.h"\
	".\include\chanserv.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\ircsocket.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\nickserv.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\bob.obj"	"$(INTDIR)\bob.sbr" : $(SOURCE) $(DEP_CPP_BOB_C)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\des\cbc_cksm.c
DEP_CPP_CBC_C=\
	".\include\des\des.h"\
	".\include\des\des_locl.h"\
	

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\cbc_cksm.obj" : $(SOURCE) $(DEP_CPP_CBC_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\cbc_cksm.obj"	"$(INTDIR)\cbc_cksm.sbr" : $(SOURCE) $(DEP_CPP_CBC_C)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\des\cfb_enc.c
DEP_CPP_CFB_E=\
	".\include\des\des.h"\
	".\include\des\des_locl.h"\
	

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\cfb_enc.obj" : $(SOURCE) $(DEP_CPP_CFB_E) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\cfb_enc.obj"	"$(INTDIR)\cfb_enc.sbr" : $(SOURCE) $(DEP_CPP_CFB_E)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\chanserv.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_CHANS=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Addr.h"\
	"..\ace_wrappers\ace\Addr.i"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\Basic_Types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config-WinCE.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Connector.cpp"\
	"..\ace_wrappers\ace\Connector.h"\
	"..\ace_wrappers\ace\Connector.i"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers.i"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Dynamic.h"\
	"..\ace_wrappers\ace\Dynamic.i"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Event_Handler.i"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Functor.h"\
	"..\ace_wrappers\ace\Functor.i"\
	"..\ace_wrappers\ace\Functor_T.cpp"\
	"..\ace_wrappers\ace\Functor_T.h"\
	"..\ace_wrappers\ace\Functor_T.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\Handle_Set.i"\
	"..\ace_wrappers\ace\Hash_Map_Manager.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager.i"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.cpp"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\INET_Addr.h"\
	"..\ace_wrappers\ace\INET_Addr.i"\
	"..\ace_wrappers\ace\IO_Cntl_Msg.h"\
	"..\ace_wrappers\ace\iosfwd.h"\
	"..\ace_wrappers\ace\IPC_SAP.h"\
	"..\ace_wrappers\ace\IPC_SAP.i"\
	"..\ace_wrappers\ace\Local_Tokens.h"\
	"..\ace_wrappers\ace\Local_Tokens.i"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc.h"\
	"..\ace_wrappers\ace\Malloc.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Mem_Map.i"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Memory_Pool.i"\
	"..\ace_wrappers\ace\Message_Block.h"\
	"..\ace_wrappers\ace\Message_Block.i"\
	"..\ace_wrappers\ace\Message_Block_T.cpp"\
	"..\ace_wrappers\ace\Message_Block_T.h"\
	"..\ace_wrappers\ace\Message_Block_T.i"\
	"..\ace_wrappers\ace\Message_Queue.h"\
	"..\ace_wrappers\ace\Message_Queue.i"\
	"..\ace_wrappers\ace\Message_Queue_T.cpp"\
	"..\ace_wrappers\ace\Message_Queue_T.h"\
	"..\ace_wrappers\ace\Message_Queue_T.i"\
	"..\ace_wrappers\ace\Module.cpp"\
	"..\ace_wrappers\ace\Module.h"\
	"..\ace_wrappers\ace\Module.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\Object_Manager.i"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\OS.i"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Reactor.i"\
	"..\ace_wrappers\ace\Reactor_Impl.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Config.i"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Service_Object.i"\
	"..\ace_wrappers\ace\Service_Repository.h"\
	"..\ace_wrappers\ace\Service_Repository.i"\
	"..\ace_wrappers\ace\Service_Types.h"\
	"..\ace_wrappers\ace\Service_Types.i"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\Shared_Object.i"\
	"..\ace_wrappers\ace\Signal.h"\
	"..\ace_wrappers\ace\Signal.i"\
	"..\ace_wrappers\ace\Singleton.cpp"\
	"..\ace_wrappers\ace\Singleton.h"\
	"..\ace_wrappers\ace\Singleton.i"\
	"..\ace_wrappers\ace\SOCK.h"\
	"..\ace_wrappers\ace\SOCK.i"\
	"..\ace_wrappers\ace\SOCK_Connector.h"\
	"..\ace_wrappers\ace\SOCK_Connector.i"\
	"..\ace_wrappers\ace\SOCK_IO.h"\
	"..\ace_wrappers\ace\SOCK_IO.i"\
	"..\ace_wrappers\ace\SOCK_Stream.h"\
	"..\ace_wrappers\ace\SOCK_Stream.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\SString.i"\
	"..\ace_wrappers\ace\Strategies.h"\
	"..\ace_wrappers\ace\Strategies.i"\
	"..\ace_wrappers\ace\Strategies_T.cpp"\
	"..\ace_wrappers\ace\Strategies_T.h"\
	"..\ace_wrappers\ace\Strategies_T.i"\
	"..\ace_wrappers\ace\Stream_Modules.cpp"\
	"..\ace_wrappers\ace\Stream_Modules.h"\
	"..\ace_wrappers\ace\Stream_Modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Complex.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Complex.i"\
	"..\ace_wrappers\ace\SV_Semaphore_Simple.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Simple.i"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Svc_Handler.cpp"\
	"..\ace_wrappers\ace\Svc_Handler.h"\
	"..\ace_wrappers\ace\Svc_Handler.i"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch.i"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_Options.i"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Task.h"\
	"..\ace_wrappers\ace\Task.i"\
	"..\ace_wrappers\ace\Task_T.cpp"\
	"..\ace_wrappers\ace\Task_T.h"\
	"..\ace_wrappers\ace\Task_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread.i"\
	"..\ace_wrappers\ace\Thread_Manager.h"\
	"..\ace_wrappers\ace\Thread_Manager.i"\
	"..\ace_wrappers\ace\Time_Value.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.i"\
	"..\ace_wrappers\ace\ws2tcpip.h"\
	".\include\base.h"\
	".\include\bob.hpp"\
	".\include\bob\bob.h"\
	".\include\bob\bobexp.h"\
	".\include\bob\compiler.h"\
	".\include\bob\eval.h"\
	".\include\bob\execute.h"\
	".\include\bob\function.h"\
	".\include\bob\objects.h"\
	".\include\bob\streams.h"\
	".\include\chanserv.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\ircsocket.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\nickserv.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\chanserv.obj" : $(SOURCE) $(DEP_CPP_CHANS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_CHANS=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Addr.h"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Connector.cpp"\
	"..\ace_wrappers\ace\Connector.h"\
	"..\ace_wrappers\ace\Connector.i"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Dynamic.h"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Functor.h"\
	"..\ace_wrappers\ace\Functor_T.cpp"\
	"..\ace_wrappers\ace\Functor_T.h"\
	"..\ace_wrappers\ace\Functor_T.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.cpp"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\INET_Addr.h"\
	"..\ace_wrappers\ace\IO_Cntl_Msg.h"\
	"..\ace_wrappers\ace\IPC_SAP.h"\
	"..\ace_wrappers\ace\IPC_SAP.i"\
	"..\ace_wrappers\ace\Local_Tokens.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Message_Block.h"\
	"..\ace_wrappers\ace\Message_Block_T.cpp"\
	"..\ace_wrappers\ace\Message_Block_T.h"\
	"..\ace_wrappers\ace\Message_Block_T.i"\
	"..\ace_wrappers\ace\Message_Queue.h"\
	"..\ace_wrappers\ace\Message_Queue_T.cpp"\
	"..\ace_wrappers\ace\Message_Queue_T.h"\
	"..\ace_wrappers\ace\Message_Queue_T.i"\
	"..\ace_wrappers\ace\Module.cpp"\
	"..\ace_wrappers\ace\Module.h"\
	"..\ace_wrappers\ace\Module.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Reactor_Impl.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Service_Repository.h"\
	"..\ace_wrappers\ace\Service_Types.h"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\Singleton.cpp"\
	"..\ace_wrappers\ace\Singleton.h"\
	"..\ace_wrappers\ace\Singleton.i"\
	"..\ace_wrappers\ace\SOCK.h"\
	"..\ace_wrappers\ace\SOCK.i"\
	"..\ace_wrappers\ace\SOCK_Connector.h"\
	"..\ace_wrappers\ace\SOCK_Connector.i"\
	"..\ace_wrappers\ace\SOCK_IO.h"\
	"..\ace_wrappers\ace\SOCK_IO.i"\
	"..\ace_wrappers\ace\SOCK_Stream.h"\
	"..\ace_wrappers\ace\SOCK_Stream.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\Strategies.h"\
	"..\ace_wrappers\ace\Strategies_T.cpp"\
	"..\ace_wrappers\ace\Strategies_T.h"\
	"..\ace_wrappers\ace\Strategies_T.i"\
	"..\ace_wrappers\ace\Stream_Modules.cpp"\
	"..\ace_wrappers\ace\Stream_Modules.h"\
	"..\ace_wrappers\ace\Stream_Modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Svc_Handler.cpp"\
	"..\ace_wrappers\ace\Svc_Handler.h"\
	"..\ace_wrappers\ace\Svc_Handler.i"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Task.h"\
	"..\ace_wrappers\ace\Task_T.cpp"\
	"..\ace_wrappers\ace\Task_T.h"\
	"..\ace_wrappers\ace\Task_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread_Manager.h"\
	"..\ace_wrappers\ace\Time_Value.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.h"\
	".\include\base.h"\
	".\include\bob.hpp"\
	".\include\bob\bob.h"\
	".\include\bob\bobexp.h"\
	".\include\bob\compiler.h"\
	".\include\bob\eval.h"\
	".\include\bob\execute.h"\
	".\include\bob\function.h"\
	".\include\bob\objects.h"\
	".\include\bob\streams.h"\
	".\include\chanserv.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\ircsocket.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\nickserv.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\chanserv.obj"	"$(INTDIR)\chanserv.sbr" : $(SOURCE) $(DEP_CPP_CHANS)\
 "$(INTDIR)"
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
	

"$(INTDIR)\CharBuffer.obj"	"$(INTDIR)\CharBuffer.sbr" : $(SOURCE)\
 $(DEP_CPP_CHARB) "$(INTDIR)"
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
	

"$(INTDIR)\CharScanner.obj"	"$(INTDIR)\CharScanner.sbr" : $(SOURCE)\
 $(DEP_CPP_CHARS) "$(INTDIR)"
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
	

"$(INTDIR)\CommonToken.obj"	"$(INTDIR)\CommonToken.sbr" : $(SOURCE)\
 $(DEP_CPP_COMMO) "$(INTDIR)"
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
	
NODEP_CPP_CONFB=\
	"..\..\program files\devstudio\vc\include\ace\ace.h"\
	"..\..\program files\devstudio\vc\include\ace\ace.i"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.h"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.i"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-borland.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-common.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32.h"\
	"..\..\program files\devstudio\vc\include\ace\config.h"\
	"..\..\program files\devstudio\vc\include\ace\inc_user_config.h"\
	"..\..\program files\devstudio\vc\include\ace\log_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_priority.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.i"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.cpp"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.h"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.i"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\os.h"\
	"..\..\program files\devstudio\vc\include\ace\os.i"\
	"..\..\program files\devstudio\vc\include\ace\streams.h"\
	"..\..\program files\devstudio\vc\include\ace\trace.h"\
	

"$(INTDIR)\confbase.obj" : $(SOURCE) $(DEP_CPP_CONFB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_CONFB=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Trace.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\confbase.obj"	"$(INTDIR)\confbase.sbr" : $(SOURCE) $(DEP_CPP_CONFB)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\cryptstream.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_CRYPT=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\Basic_Types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config-WinCE.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\iosfwd.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\Object_Manager.i"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\OS.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\SString.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\ws2tcpip.h"\
	".\include\cryptstream.h"\
	".\include\des\des.h"\
	".\include\des\des_locl.h"\
	".\include\des\spr.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	

"$(INTDIR)\cryptstream.obj" : $(SOURCE) $(DEP_CPP_CRYPT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_CRYPT=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Trace.h"\
	".\include\cryptstream.h"\
	".\include\des\des.h"\
	".\include\des\des_locl.h"\
	".\include\des\spr.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	

"$(INTDIR)\cryptstream.obj"	"$(INTDIR)\cryptstream.sbr" : $(SOURCE)\
 $(DEP_CPP_CRYPT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\datetime.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_DATET=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\Basic_Types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config-WinCE.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\iosfwd.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\Object_Manager.i"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\OS.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\SString.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\ws2tcpip.h"\
	".\include\datetime.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	

"$(INTDIR)\datetime.obj" : $(SOURCE) $(DEP_CPP_DATET) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_DATET=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Trace.h"\
	".\include\datetime.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	

"$(INTDIR)\datetime.obj"	"$(INTDIR)\datetime.sbr" : $(SOURCE) $(DEP_CPP_DATET)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\dccengine.cpp
DEP_CPP_DCCEN=\
	".\include\dccengine.h"\
	

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\dccengine.obj" : $(SOURCE) $(DEP_CPP_DCCEN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\dccengine.obj"	"$(INTDIR)\dccengine.sbr" : $(SOURCE)\
 $(DEP_CPP_DCCEN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\des\des_enc.c
DEP_CPP_DES_E=\
	".\include\des\des.h"\
	".\include\des\des_locl.h"\
	

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\des_enc.obj" : $(SOURCE) $(DEP_CPP_DES_E) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\des_enc.obj"	"$(INTDIR)\des_enc.sbr" : $(SOURCE) $(DEP_CPP_DES_E)\
 "$(INTDIR)"
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
	
NODEP_CPP_ESCLE=\
	".\src\esclexer.hpp"\
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
	".\include\EscLexer.hpp"\
	".\include\esclexertokentypes.hpp"\
	".\include\mstring.h"\
	

"$(INTDIR)\EscLexer.obj"	"$(INTDIR)\EscLexer.sbr" : $(SOURCE) $(DEP_CPP_ESCLE)\
 "$(INTDIR)"
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
	
NODEP_CPP_ESCPA=\
	".\src\esclexertokentypes.hpp"\
	".\src\escparser.hpp"\
	

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
	".\include\esclexertokentypes.hpp"\
	".\include\EscParser.hpp"\
	".\include\mstring.h"\
	

"$(INTDIR)\EscParser.obj"	"$(INTDIR)\EscParser.sbr" : $(SOURCE)\
 $(DEP_CPP_ESCPA) "$(INTDIR)"
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
	
NODEP_CPP_FILEC=\
	"..\..\program files\devstudio\vc\include\ace\ace.h"\
	"..\..\program files\devstudio\vc\include\ace\ace.i"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.h"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.i"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-borland.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-common.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32.h"\
	"..\..\program files\devstudio\vc\include\ace\config.h"\
	"..\..\program files\devstudio\vc\include\ace\inc_user_config.h"\
	"..\..\program files\devstudio\vc\include\ace\log_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_priority.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.i"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.cpp"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.h"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.i"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\os.h"\
	"..\..\program files\devstudio\vc\include\ace\os.i"\
	"..\..\program files\devstudio\vc\include\ace\streams.h"\
	"..\..\program files\devstudio\vc\include\ace\trace.h"\
	

"$(INTDIR)\fileconf.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_FILEC=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Trace.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	".\include\utils.h"\
	

"$(INTDIR)\fileconf.obj"	"$(INTDIR)\fileconf.sbr" : $(SOURCE) $(DEP_CPP_FILEC)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\ircsocket.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_IRCSO=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Addr.h"\
	"..\ace_wrappers\ace\Addr.i"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\Basic_Types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config-WinCE.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Connector.cpp"\
	"..\ace_wrappers\ace\Connector.h"\
	"..\ace_wrappers\ace\Connector.i"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers.i"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Dynamic.h"\
	"..\ace_wrappers\ace\Dynamic.i"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Event_Handler.i"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Functor.h"\
	"..\ace_wrappers\ace\Functor.i"\
	"..\ace_wrappers\ace\Functor_T.cpp"\
	"..\ace_wrappers\ace\Functor_T.h"\
	"..\ace_wrappers\ace\Functor_T.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\Handle_Set.i"\
	"..\ace_wrappers\ace\Hash_Map_Manager.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager.i"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.cpp"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\INET_Addr.h"\
	"..\ace_wrappers\ace\INET_Addr.i"\
	"..\ace_wrappers\ace\IO_Cntl_Msg.h"\
	"..\ace_wrappers\ace\iosfwd.h"\
	"..\ace_wrappers\ace\IPC_SAP.h"\
	"..\ace_wrappers\ace\IPC_SAP.i"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc.h"\
	"..\ace_wrappers\ace\Malloc.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Mem_Map.i"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Memory_Pool.i"\
	"..\ace_wrappers\ace\Message_Block.h"\
	"..\ace_wrappers\ace\Message_Block.i"\
	"..\ace_wrappers\ace\Message_Block_T.cpp"\
	"..\ace_wrappers\ace\Message_Block_T.h"\
	"..\ace_wrappers\ace\Message_Block_T.i"\
	"..\ace_wrappers\ace\Message_Queue.h"\
	"..\ace_wrappers\ace\Message_Queue.i"\
	"..\ace_wrappers\ace\Message_Queue_T.cpp"\
	"..\ace_wrappers\ace\Message_Queue_T.h"\
	"..\ace_wrappers\ace\Message_Queue_T.i"\
	"..\ace_wrappers\ace\Module.cpp"\
	"..\ace_wrappers\ace\Module.h"\
	"..\ace_wrappers\ace\Module.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\Object_Manager.i"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\OS.i"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Reactor.i"\
	"..\ace_wrappers\ace\Reactor_Impl.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Config.i"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Service_Object.i"\
	"..\ace_wrappers\ace\Service_Repository.h"\
	"..\ace_wrappers\ace\Service_Repository.i"\
	"..\ace_wrappers\ace\Service_Types.h"\
	"..\ace_wrappers\ace\Service_Types.i"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\Shared_Object.i"\
	"..\ace_wrappers\ace\Signal.h"\
	"..\ace_wrappers\ace\Signal.i"\
	"..\ace_wrappers\ace\Singleton.cpp"\
	"..\ace_wrappers\ace\Singleton.h"\
	"..\ace_wrappers\ace\Singleton.i"\
	"..\ace_wrappers\ace\SOCK.h"\
	"..\ace_wrappers\ace\SOCK.i"\
	"..\ace_wrappers\ace\SOCK_Connector.h"\
	"..\ace_wrappers\ace\SOCK_Connector.i"\
	"..\ace_wrappers\ace\SOCK_IO.h"\
	"..\ace_wrappers\ace\SOCK_IO.i"\
	"..\ace_wrappers\ace\SOCK_Stream.h"\
	"..\ace_wrappers\ace\SOCK_Stream.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\SString.i"\
	"..\ace_wrappers\ace\Strategies.h"\
	"..\ace_wrappers\ace\Strategies.i"\
	"..\ace_wrappers\ace\Strategies_T.cpp"\
	"..\ace_wrappers\ace\Strategies_T.h"\
	"..\ace_wrappers\ace\Strategies_T.i"\
	"..\ace_wrappers\ace\Stream_Modules.cpp"\
	"..\ace_wrappers\ace\Stream_Modules.h"\
	"..\ace_wrappers\ace\Stream_Modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Complex.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Complex.i"\
	"..\ace_wrappers\ace\SV_Semaphore_Simple.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Simple.i"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Svc_Handler.cpp"\
	"..\ace_wrappers\ace\Svc_Handler.h"\
	"..\ace_wrappers\ace\Svc_Handler.i"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch.i"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_Options.i"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Task.h"\
	"..\ace_wrappers\ace\Task.i"\
	"..\ace_wrappers\ace\Task_T.cpp"\
	"..\ace_wrappers\ace\Task_T.h"\
	"..\ace_wrappers\ace\Task_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread.i"\
	"..\ace_wrappers\ace\Thread_Manager.h"\
	"..\ace_wrappers\ace\Thread_Manager.i"\
	"..\ace_wrappers\ace\Time_Value.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.i"\
	"..\ace_wrappers\ace\ws2tcpip.h"\
	".\include\ircsocket.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\trace.h"\
	

"$(INTDIR)\ircsocket.obj" : $(SOURCE) $(DEP_CPP_IRCSO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_IRCSO=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Addr.h"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Connector.cpp"\
	"..\ace_wrappers\ace\Connector.h"\
	"..\ace_wrappers\ace\Connector.i"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Dynamic.h"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Functor.h"\
	"..\ace_wrappers\ace\Functor_T.cpp"\
	"..\ace_wrappers\ace\Functor_T.h"\
	"..\ace_wrappers\ace\Functor_T.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.cpp"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\INET_Addr.h"\
	"..\ace_wrappers\ace\IO_Cntl_Msg.h"\
	"..\ace_wrappers\ace\IPC_SAP.h"\
	"..\ace_wrappers\ace\IPC_SAP.i"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Message_Block.h"\
	"..\ace_wrappers\ace\Message_Block_T.cpp"\
	"..\ace_wrappers\ace\Message_Block_T.h"\
	"..\ace_wrappers\ace\Message_Block_T.i"\
	"..\ace_wrappers\ace\Message_Queue.h"\
	"..\ace_wrappers\ace\Message_Queue_T.cpp"\
	"..\ace_wrappers\ace\Message_Queue_T.h"\
	"..\ace_wrappers\ace\Message_Queue_T.i"\
	"..\ace_wrappers\ace\Module.cpp"\
	"..\ace_wrappers\ace\Module.h"\
	"..\ace_wrappers\ace\Module.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Reactor_Impl.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Service_Repository.h"\
	"..\ace_wrappers\ace\Service_Types.h"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\Singleton.cpp"\
	"..\ace_wrappers\ace\Singleton.h"\
	"..\ace_wrappers\ace\Singleton.i"\
	"..\ace_wrappers\ace\SOCK.h"\
	"..\ace_wrappers\ace\SOCK.i"\
	"..\ace_wrappers\ace\SOCK_Connector.h"\
	"..\ace_wrappers\ace\SOCK_Connector.i"\
	"..\ace_wrappers\ace\SOCK_IO.h"\
	"..\ace_wrappers\ace\SOCK_IO.i"\
	"..\ace_wrappers\ace\SOCK_Stream.h"\
	"..\ace_wrappers\ace\SOCK_Stream.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\Strategies.h"\
	"..\ace_wrappers\ace\Strategies_T.cpp"\
	"..\ace_wrappers\ace\Strategies_T.h"\
	"..\ace_wrappers\ace\Strategies_T.i"\
	"..\ace_wrappers\ace\Stream_Modules.cpp"\
	"..\ace_wrappers\ace\Stream_Modules.h"\
	"..\ace_wrappers\ace\Stream_Modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Svc_Handler.cpp"\
	"..\ace_wrappers\ace\Svc_Handler.h"\
	"..\ace_wrappers\ace\Svc_Handler.i"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Task.h"\
	"..\ace_wrappers\ace\Task_T.cpp"\
	"..\ace_wrappers\ace\Task_T.h"\
	"..\ace_wrappers\ace\Task_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread_Manager.h"\
	"..\ace_wrappers\ace\Time_Value.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\ircsocket.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\ircsocket.obj"	"$(INTDIR)\ircsocket.sbr" : $(SOURCE)\
 $(DEP_CPP_IRCSO) "$(INTDIR)"
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
	

"$(INTDIR)\LLkParser.obj"	"$(INTDIR)\LLkParser.sbr" : $(SOURCE)\
 $(DEP_CPP_LLKPA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\lockable.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_LOCKA=\
	".\include\lockable.h"\
	
NODEP_CPP_LOCKA=\
	"..\..\program files\devstudio\vc\include\ace\ace.h"\
	"..\..\program files\devstudio\vc\include\ace\ace.i"\
	"..\..\program files\devstudio\vc\include\ace\atomic_op.i"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.h"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.i"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-borland.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-common.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32.h"\
	"..\..\program files\devstudio\vc\include\ace\config.h"\
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
	"..\..\program files\devstudio\vc\include\ace\inc_user_config.h"\
	"..\..\program files\devstudio\vc\include\ace\io_cntl_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_priority.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.i"\
	"..\..\program files\devstudio\vc\include\ace\malloc.i"\
	"..\..\program files\devstudio\vc\include\ace\malloc_base.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.i"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.cpp"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.h"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.i"\
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
	"..\..\program files\devstudio\vc\include\ace\object_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\os.h"\
	"..\..\program files\devstudio\vc\include\ace\os.i"\
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
	"..\..\program files\devstudio\vc\include\ace\sstring.h"\
	"..\..\program files\devstudio\vc\include\ace\sstring.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.i"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.cpp"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.h"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.i"\
	"..\..\program files\devstudio\vc\include\ace\streams.h"\
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
	"..\..\program files\devstudio\vc\include\ace\trace.h"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.h"\
	"..\..\program files\devstudio\vc\include\ace\wfmo_reactor.i"\
	

"$(INTDIR)\lockable.obj" : $(SOURCE) $(DEP_CPP_LOCKA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_LOCKA=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\Local_Tokens.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread_Manager.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	".\include\utils.h"\
	

"$(INTDIR)\lockable.obj"	"$(INTDIR)\lockable.sbr" : $(SOURCE) $(DEP_CPP_LOCKA)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\log.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_LOG_C=\
	".\include\log.h"\
	".\include\mstring.h"\
	

"$(INTDIR)\log.obj" : $(SOURCE) $(DEP_CPP_LOG_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_LOG_C=\
	".\include\log.h"\
	".\include\mstring.h"\
	

"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) $(DEP_CPP_LOG_C)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\magick.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_MAGIC=\
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
	
NODEP_CPP_MAGIC=\
	"..\..\program files\devstudio\vc\include\ace\ace.h"\
	"..\..\program files\devstudio\vc\include\ace\ace.i"\
	"..\..\program files\devstudio\vc\include\ace\atomic_op.i"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.h"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.i"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-borland.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-common.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32.h"\
	"..\..\program files\devstudio\vc\include\ace\config.h"\
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
	"..\..\program files\devstudio\vc\include\ace\inc_user_config.h"\
	"..\..\program files\devstudio\vc\include\ace\io_cntl_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_priority.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.i"\
	"..\..\program files\devstudio\vc\include\ace\malloc.i"\
	"..\..\program files\devstudio\vc\include\ace\malloc_base.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.i"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.cpp"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.h"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.i"\
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
	"..\..\program files\devstudio\vc\include\ace\object_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\os.h"\
	"..\..\program files\devstudio\vc\include\ace\os.i"\
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
	"..\..\program files\devstudio\vc\include\ace\sstring.h"\
	"..\..\program files\devstudio\vc\include\ace\sstring.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.i"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.cpp"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.h"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.i"\
	"..\..\program files\devstudio\vc\include\ace\streams.h"\
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
	"..\..\program files\devstudio\vc\include\ace\trace.h"\
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
	".\src\esclexer.hpp"\
	".\src\escparser.hpp"\
	

"$(INTDIR)\magick.obj" : $(SOURCE) $(DEP_CPP_MAGIC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MAGIC=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Addr.h"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Connector.cpp"\
	"..\ace_wrappers\ace\Connector.h"\
	"..\ace_wrappers\ace\Connector.i"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Dynamic.h"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Functor.h"\
	"..\ace_wrappers\ace\Functor_T.cpp"\
	"..\ace_wrappers\ace\Functor_T.h"\
	"..\ace_wrappers\ace\Functor_T.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.cpp"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\INET_Addr.h"\
	"..\ace_wrappers\ace\IO_Cntl_Msg.h"\
	"..\ace_wrappers\ace\IPC_SAP.h"\
	"..\ace_wrappers\ace\IPC_SAP.i"\
	"..\ace_wrappers\ace\Local_Tokens.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Message_Block.h"\
	"..\ace_wrappers\ace\Message_Block_T.cpp"\
	"..\ace_wrappers\ace\Message_Block_T.h"\
	"..\ace_wrappers\ace\Message_Block_T.i"\
	"..\ace_wrappers\ace\Message_Queue.h"\
	"..\ace_wrappers\ace\Message_Queue_T.cpp"\
	"..\ace_wrappers\ace\Message_Queue_T.h"\
	"..\ace_wrappers\ace\Message_Queue_T.i"\
	"..\ace_wrappers\ace\Module.cpp"\
	"..\ace_wrappers\ace\Module.h"\
	"..\ace_wrappers\ace\Module.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Reactor_Impl.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Service_Repository.h"\
	"..\ace_wrappers\ace\Service_Types.h"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\Singleton.cpp"\
	"..\ace_wrappers\ace\Singleton.h"\
	"..\ace_wrappers\ace\Singleton.i"\
	"..\ace_wrappers\ace\SOCK.h"\
	"..\ace_wrappers\ace\SOCK.i"\
	"..\ace_wrappers\ace\SOCK_Connector.h"\
	"..\ace_wrappers\ace\SOCK_Connector.i"\
	"..\ace_wrappers\ace\SOCK_IO.h"\
	"..\ace_wrappers\ace\SOCK_IO.i"\
	"..\ace_wrappers\ace\SOCK_Stream.h"\
	"..\ace_wrappers\ace\SOCK_Stream.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\Strategies.h"\
	"..\ace_wrappers\ace\Strategies_T.cpp"\
	"..\ace_wrappers\ace\Strategies_T.h"\
	"..\ace_wrappers\ace\Strategies_T.i"\
	"..\ace_wrappers\ace\Stream_Modules.cpp"\
	"..\ace_wrappers\ace\Stream_Modules.h"\
	"..\ace_wrappers\ace\Stream_Modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Svc_Handler.cpp"\
	"..\ace_wrappers\ace\Svc_Handler.h"\
	"..\ace_wrappers\ace\Svc_Handler.i"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Task.h"\
	"..\ace_wrappers\ace\Task_T.cpp"\
	"..\ace_wrappers\ace\Task_T.h"\
	"..\ace_wrappers\ace\Task_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread_Manager.h"\
	"..\ace_wrappers\ace\Time_Value.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.h"\
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
	".\include\base.h"\
	".\include\bob.hpp"\
	".\include\bob\bob.h"\
	".\include\bob\bobexp.h"\
	".\include\bob\compiler.h"\
	".\include\bob\eval.h"\
	".\include\bob\execute.h"\
	".\include\bob\function.h"\
	".\include\bob\objects.h"\
	".\include\bob\streams.h"\
	".\include\chanserv.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\EscLexer.hpp"\
	".\include\EscParser.hpp"\
	".\include\fileconf.h"\
	".\include\ircsocket.h"\
	".\include\language.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\nickserv.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\magick.obj"	"$(INTDIR)\magick.sbr" : $(SOURCE) $(DEP_CPP_MAGIC)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\main.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_MAIN_=\
	".\include\bob.hpp"\
	".\include\confbase.h"\
	".\include\fileconf.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	
NODEP_CPP_MAIN_=\
	"..\..\program files\devstudio\vc\include\ace\ace.h"\
	"..\..\program files\devstudio\vc\include\ace\ace.i"\
	"..\..\program files\devstudio\vc\include\ace\atomic_op.i"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.h"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.i"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-borland.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-common.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32.h"\
	"..\..\program files\devstudio\vc\include\ace\config.h"\
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
	"..\..\program files\devstudio\vc\include\ace\inc_user_config.h"\
	"..\..\program files\devstudio\vc\include\ace\io_cntl_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_priority.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.i"\
	"..\..\program files\devstudio\vc\include\ace\malloc.i"\
	"..\..\program files\devstudio\vc\include\ace\malloc_base.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.h"\
	"..\..\program files\devstudio\vc\include\ace\malloc_t.i"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.cpp"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.h"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.i"\
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
	"..\..\program files\devstudio\vc\include\ace\object_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\os.h"\
	"..\..\program files\devstudio\vc\include\ace\os.i"\
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
	"..\..\program files\devstudio\vc\include\ace\sstring.h"\
	"..\..\program files\devstudio\vc\include\ace\sstring.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies.i"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.cpp"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.h"\
	"..\..\program files\devstudio\vc\include\ace\strategies_t.i"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.cpp"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.h"\
	"..\..\program files\devstudio\vc\include\ace\stream_modules.i"\
	"..\..\program files\devstudio\vc\include\ace\streams.h"\
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
	"..\..\program files\devstudio\vc\include\ace\trace.h"\
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
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MAIN_=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Addr.h"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Connector.cpp"\
	"..\ace_wrappers\ace\Connector.h"\
	"..\ace_wrappers\ace\Connector.i"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Dynamic.h"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Functor.h"\
	"..\ace_wrappers\ace\Functor_T.cpp"\
	"..\ace_wrappers\ace\Functor_T.h"\
	"..\ace_wrappers\ace\Functor_T.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.cpp"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\INET_Addr.h"\
	"..\ace_wrappers\ace\IO_Cntl_Msg.h"\
	"..\ace_wrappers\ace\IPC_SAP.h"\
	"..\ace_wrappers\ace\IPC_SAP.i"\
	"..\ace_wrappers\ace\Local_Tokens.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Message_Block.h"\
	"..\ace_wrappers\ace\Message_Block_T.cpp"\
	"..\ace_wrappers\ace\Message_Block_T.h"\
	"..\ace_wrappers\ace\Message_Block_T.i"\
	"..\ace_wrappers\ace\Message_Queue.h"\
	"..\ace_wrappers\ace\Message_Queue_T.cpp"\
	"..\ace_wrappers\ace\Message_Queue_T.h"\
	"..\ace_wrappers\ace\Message_Queue_T.i"\
	"..\ace_wrappers\ace\Module.cpp"\
	"..\ace_wrappers\ace\Module.h"\
	"..\ace_wrappers\ace\Module.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Reactor_Impl.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Service_Repository.h"\
	"..\ace_wrappers\ace\Service_Types.h"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\Singleton.cpp"\
	"..\ace_wrappers\ace\Singleton.h"\
	"..\ace_wrappers\ace\Singleton.i"\
	"..\ace_wrappers\ace\SOCK.h"\
	"..\ace_wrappers\ace\SOCK.i"\
	"..\ace_wrappers\ace\SOCK_Connector.h"\
	"..\ace_wrappers\ace\SOCK_Connector.i"\
	"..\ace_wrappers\ace\SOCK_IO.h"\
	"..\ace_wrappers\ace\SOCK_IO.i"\
	"..\ace_wrappers\ace\SOCK_Stream.h"\
	"..\ace_wrappers\ace\SOCK_Stream.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\Strategies.h"\
	"..\ace_wrappers\ace\Strategies_T.cpp"\
	"..\ace_wrappers\ace\Strategies_T.h"\
	"..\ace_wrappers\ace\Strategies_T.i"\
	"..\ace_wrappers\ace\Stream_Modules.cpp"\
	"..\ace_wrappers\ace\Stream_Modules.h"\
	"..\ace_wrappers\ace\Stream_Modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Svc_Handler.cpp"\
	"..\ace_wrappers\ace\Svc_Handler.h"\
	"..\ace_wrappers\ace\Svc_Handler.i"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Task.h"\
	"..\ace_wrappers\ace\Task_T.cpp"\
	"..\ace_wrappers\ace\Task_T.h"\
	"..\ace_wrappers\ace\Task_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread_Manager.h"\
	"..\ace_wrappers\ace\Time_Value.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.h"\
	".\include\base.h"\
	".\include\bob.hpp"\
	".\include\bob\bob.h"\
	".\include\bob\bobexp.h"\
	".\include\bob\compiler.h"\
	".\include\bob\eval.h"\
	".\include\bob\execute.h"\
	".\include\bob\function.h"\
	".\include\bob\objects.h"\
	".\include\bob\streams.h"\
	".\include\chanserv.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\ircsocket.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\nickserv.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) $(DEP_CPP_MAIN_)\
 "$(INTDIR)"
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
	

"$(INTDIR)\MismatchedTokenException.obj"\
	"$(INTDIR)\MismatchedTokenException.sbr" : $(SOURCE) $(DEP_CPP_MISMA)\
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
	
NODEP_CPP_MSTRE=\
	"..\..\program files\devstudio\vc\include\ace\ace.h"\
	"..\..\program files\devstudio\vc\include\ace\ace.i"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.h"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.i"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-borland.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-common.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32.h"\
	"..\..\program files\devstudio\vc\include\ace\config.h"\
	"..\..\program files\devstudio\vc\include\ace\inc_user_config.h"\
	"..\..\program files\devstudio\vc\include\ace\log_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_priority.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.i"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.cpp"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.h"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.i"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\os.h"\
	"..\..\program files\devstudio\vc\include\ace\os.i"\
	"..\..\program files\devstudio\vc\include\ace\streams.h"\
	"..\..\program files\devstudio\vc\include\ace\trace.h"\
	

"$(INTDIR)\mstream.obj" : $(SOURCE) $(DEP_CPP_MSTRE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MSTRE=\
	"..\..\program files\devstudio\vc\include\zconf.h"\
	"..\..\program files\devstudio\vc\include\zlib.h"\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Trace.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\utils.h"\
	

"$(INTDIR)\mstream.obj"	"$(INTDIR)\mstream.sbr" : $(SOURCE) $(DEP_CPP_MSTRE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\mstring.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_MSTRI=\
	".\include\mstring.h"\
	
NODEP_CPP_MSTRI=\
	"..\..\program files\devstudio\vc\include\ace\ace.h"\
	"..\..\program files\devstudio\vc\include\ace\ace.i"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.h"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.i"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-borland.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-common.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32.h"\
	"..\..\program files\devstudio\vc\include\ace\config.h"\
	"..\..\program files\devstudio\vc\include\ace\inc_user_config.h"\
	"..\..\program files\devstudio\vc\include\ace\log_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_priority.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.i"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.cpp"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.h"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.i"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\os.h"\
	"..\..\program files\devstudio\vc\include\ace\os.i"\
	"..\..\program files\devstudio\vc\include\ace\streams.h"\
	"..\..\program files\devstudio\vc\include\ace\trace.h"\
	

"$(INTDIR)\mstring.obj" : $(SOURCE) $(DEP_CPP_MSTRI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MSTRI=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Trace.h"\
	".\include\mstring.h"\
	

"$(INTDIR)\mstring.obj"	"$(INTDIR)\mstring.sbr" : $(SOURCE) $(DEP_CPP_MSTRI)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\nickserv.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_NICKS=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Addr.h"\
	"..\ace_wrappers\ace\Addr.i"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\Basic_Types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config-WinCE.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Connector.cpp"\
	"..\ace_wrappers\ace\Connector.h"\
	"..\ace_wrappers\ace\Connector.i"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers.i"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Dynamic.h"\
	"..\ace_wrappers\ace\Dynamic.i"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Event_Handler.i"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Functor.h"\
	"..\ace_wrappers\ace\Functor.i"\
	"..\ace_wrappers\ace\Functor_T.cpp"\
	"..\ace_wrappers\ace\Functor_T.h"\
	"..\ace_wrappers\ace\Functor_T.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\Handle_Set.i"\
	"..\ace_wrappers\ace\Hash_Map_Manager.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager.i"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.cpp"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\INET_Addr.h"\
	"..\ace_wrappers\ace\INET_Addr.i"\
	"..\ace_wrappers\ace\IO_Cntl_Msg.h"\
	"..\ace_wrappers\ace\iosfwd.h"\
	"..\ace_wrappers\ace\IPC_SAP.h"\
	"..\ace_wrappers\ace\IPC_SAP.i"\
	"..\ace_wrappers\ace\Local_Tokens.h"\
	"..\ace_wrappers\ace\Local_Tokens.i"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc.h"\
	"..\ace_wrappers\ace\Malloc.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Mem_Map.i"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Memory_Pool.i"\
	"..\ace_wrappers\ace\Message_Block.h"\
	"..\ace_wrappers\ace\Message_Block.i"\
	"..\ace_wrappers\ace\Message_Block_T.cpp"\
	"..\ace_wrappers\ace\Message_Block_T.h"\
	"..\ace_wrappers\ace\Message_Block_T.i"\
	"..\ace_wrappers\ace\Message_Queue.h"\
	"..\ace_wrappers\ace\Message_Queue.i"\
	"..\ace_wrappers\ace\Message_Queue_T.cpp"\
	"..\ace_wrappers\ace\Message_Queue_T.h"\
	"..\ace_wrappers\ace\Message_Queue_T.i"\
	"..\ace_wrappers\ace\Module.cpp"\
	"..\ace_wrappers\ace\Module.h"\
	"..\ace_wrappers\ace\Module.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\Object_Manager.i"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\OS.i"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Reactor.i"\
	"..\ace_wrappers\ace\Reactor_Impl.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Config.i"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Service_Object.i"\
	"..\ace_wrappers\ace\Service_Repository.h"\
	"..\ace_wrappers\ace\Service_Repository.i"\
	"..\ace_wrappers\ace\Service_Types.h"\
	"..\ace_wrappers\ace\Service_Types.i"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\Shared_Object.i"\
	"..\ace_wrappers\ace\Signal.h"\
	"..\ace_wrappers\ace\Signal.i"\
	"..\ace_wrappers\ace\Singleton.cpp"\
	"..\ace_wrappers\ace\Singleton.h"\
	"..\ace_wrappers\ace\Singleton.i"\
	"..\ace_wrappers\ace\SOCK.h"\
	"..\ace_wrappers\ace\SOCK.i"\
	"..\ace_wrappers\ace\SOCK_Connector.h"\
	"..\ace_wrappers\ace\SOCK_Connector.i"\
	"..\ace_wrappers\ace\SOCK_IO.h"\
	"..\ace_wrappers\ace\SOCK_IO.i"\
	"..\ace_wrappers\ace\SOCK_Stream.h"\
	"..\ace_wrappers\ace\SOCK_Stream.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\SString.i"\
	"..\ace_wrappers\ace\Strategies.h"\
	"..\ace_wrappers\ace\Strategies.i"\
	"..\ace_wrappers\ace\Strategies_T.cpp"\
	"..\ace_wrappers\ace\Strategies_T.h"\
	"..\ace_wrappers\ace\Strategies_T.i"\
	"..\ace_wrappers\ace\Stream_Modules.cpp"\
	"..\ace_wrappers\ace\Stream_Modules.h"\
	"..\ace_wrappers\ace\Stream_Modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Complex.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Complex.i"\
	"..\ace_wrappers\ace\SV_Semaphore_Simple.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Simple.i"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Svc_Handler.cpp"\
	"..\ace_wrappers\ace\Svc_Handler.h"\
	"..\ace_wrappers\ace\Svc_Handler.i"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch.i"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_Options.i"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Task.h"\
	"..\ace_wrappers\ace\Task.i"\
	"..\ace_wrappers\ace\Task_T.cpp"\
	"..\ace_wrappers\ace\Task_T.h"\
	"..\ace_wrappers\ace\Task_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread.i"\
	"..\ace_wrappers\ace\Thread_Manager.h"\
	"..\ace_wrappers\ace\Thread_Manager.i"\
	"..\ace_wrappers\ace\Time_Value.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.i"\
	"..\ace_wrappers\ace\ws2tcpip.h"\
	".\include\base.h"\
	".\include\bob.hpp"\
	".\include\bob\bob.h"\
	".\include\bob\bobexp.h"\
	".\include\bob\compiler.h"\
	".\include\bob\eval.h"\
	".\include\bob\execute.h"\
	".\include\bob\function.h"\
	".\include\bob\objects.h"\
	".\include\bob\streams.h"\
	".\include\chanserv.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\ircsocket.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\nickserv.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\nickserv.obj" : $(SOURCE) $(DEP_CPP_NICKS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_NICKS=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Addr.h"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Connector.cpp"\
	"..\ace_wrappers\ace\Connector.h"\
	"..\ace_wrappers\ace\Connector.i"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Dynamic.h"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Functor.h"\
	"..\ace_wrappers\ace\Functor_T.cpp"\
	"..\ace_wrappers\ace\Functor_T.h"\
	"..\ace_wrappers\ace\Functor_T.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.cpp"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.h"\
	"..\ace_wrappers\ace\Hash_Map_Manager_T.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\INET_Addr.h"\
	"..\ace_wrappers\ace\IO_Cntl_Msg.h"\
	"..\ace_wrappers\ace\IPC_SAP.h"\
	"..\ace_wrappers\ace\IPC_SAP.i"\
	"..\ace_wrappers\ace\Local_Tokens.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Message_Block.h"\
	"..\ace_wrappers\ace\Message_Block_T.cpp"\
	"..\ace_wrappers\ace\Message_Block_T.h"\
	"..\ace_wrappers\ace\Message_Block_T.i"\
	"..\ace_wrappers\ace\Message_Queue.h"\
	"..\ace_wrappers\ace\Message_Queue_T.cpp"\
	"..\ace_wrappers\ace\Message_Queue_T.h"\
	"..\ace_wrappers\ace\Message_Queue_T.i"\
	"..\ace_wrappers\ace\Module.cpp"\
	"..\ace_wrappers\ace\Module.h"\
	"..\ace_wrappers\ace\Module.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Reactor_Impl.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Service_Repository.h"\
	"..\ace_wrappers\ace\Service_Types.h"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\Singleton.cpp"\
	"..\ace_wrappers\ace\Singleton.h"\
	"..\ace_wrappers\ace\Singleton.i"\
	"..\ace_wrappers\ace\SOCK.h"\
	"..\ace_wrappers\ace\SOCK.i"\
	"..\ace_wrappers\ace\SOCK_Connector.h"\
	"..\ace_wrappers\ace\SOCK_Connector.i"\
	"..\ace_wrappers\ace\SOCK_IO.h"\
	"..\ace_wrappers\ace\SOCK_IO.i"\
	"..\ace_wrappers\ace\SOCK_Stream.h"\
	"..\ace_wrappers\ace\SOCK_Stream.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\Strategies.h"\
	"..\ace_wrappers\ace\Strategies_T.cpp"\
	"..\ace_wrappers\ace\Strategies_T.h"\
	"..\ace_wrappers\ace\Strategies_T.i"\
	"..\ace_wrappers\ace\Stream_Modules.cpp"\
	"..\ace_wrappers\ace\Stream_Modules.h"\
	"..\ace_wrappers\ace\Stream_Modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Svc_Handler.cpp"\
	"..\ace_wrappers\ace\Svc_Handler.h"\
	"..\ace_wrappers\ace\Svc_Handler.i"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Task.h"\
	"..\ace_wrappers\ace\Task_T.cpp"\
	"..\ace_wrappers\ace\Task_T.h"\
	"..\ace_wrappers\ace\Task_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread_Manager.h"\
	"..\ace_wrappers\ace\Time_Value.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\WFMO_Reactor.h"\
	".\include\base.h"\
	".\include\bob.hpp"\
	".\include\bob\bob.h"\
	".\include\bob\bobexp.h"\
	".\include\bob\compiler.h"\
	".\include\bob\eval.h"\
	".\include\bob\execute.h"\
	".\include\bob\function.h"\
	".\include\bob\objects.h"\
	".\include\bob\streams.h"\
	".\include\chanserv.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\ircsocket.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\nickserv.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\nickserv.obj"	"$(INTDIR)\nickserv.sbr" : $(SOURCE) $(DEP_CPP_NICKS)\
 "$(INTDIR)"
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
	

"$(INTDIR)\NoViableAltException.obj"	"$(INTDIR)\NoViableAltException.sbr" : \
$(SOURCE) $(DEP_CPP_NOVIA) "$(INTDIR)"
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
	

"$(INTDIR)\Parser.obj"	"$(INTDIR)\Parser.sbr" : $(SOURCE) $(DEP_CPP_PARSE)\
 "$(INTDIR)"
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
	

"$(INTDIR)\ParserException.obj"	"$(INTDIR)\ParserException.sbr" : $(SOURCE)\
 $(DEP_CPP_PARSER) "$(INTDIR)"
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
	

"$(INTDIR)\ScannerException.obj"	"$(INTDIR)\ScannerException.sbr" : $(SOURCE)\
 $(DEP_CPP_SCANN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\des\set_key.c

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_SET_K=\
	".\include\des\des.h"\
	".\include\des\des_locl.h"\
	".\include\des\podd.h"\
	".\include\des\sk.h"\
	

"$(INTDIR)\set_key.obj" : $(SOURCE) $(DEP_CPP_SET_K) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_SET_K=\
	".\include\des\des.h"\
	".\include\des\des_locl.h"\
	".\include\des\podd.h"\
	".\include\des\sk.h"\
	

"$(INTDIR)\set_key.obj"	"$(INTDIR)\set_key.sbr" : $(SOURCE) $(DEP_CPP_SET_K)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\des\str2key.c
DEP_CPP_STR2K=\
	".\include\des\des.h"\
	".\include\des\des_locl.h"\
	

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\str2key.obj" : $(SOURCE) $(DEP_CPP_STR2K) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\str2key.obj"	"$(INTDIR)\str2key.sbr" : $(SOURCE) $(DEP_CPP_STR2K)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\String.cpp
DEP_CPP_STRIN=\
	".\include\antlr\config.hpp"\
	".\include\antlr\string.hpp"\
	

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\String.obj" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\String.obj"	"$(INTDIR)\String.sbr" : $(SOURCE) $(DEP_CPP_STRIN)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\textfile.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_TEXTF=\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	
NODEP_CPP_TEXTF=\
	"..\..\program files\devstudio\vc\include\ace\ace.h"\
	"..\..\program files\devstudio\vc\include\ace\ace.i"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.h"\
	"..\..\program files\devstudio\vc\include\ace\basic_types.i"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-borland.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32-common.h"\
	"..\..\program files\devstudio\vc\include\ace\config-win32.h"\
	"..\..\program files\devstudio\vc\include\ace\config.h"\
	"..\..\program files\devstudio\vc\include\ace\inc_user_config.h"\
	"..\..\program files\devstudio\vc\include\ace\log_msg.h"\
	"..\..\program files\devstudio\vc\include\ace\log_priority.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.h"\
	"..\..\program files\devstudio\vc\include\ace\log_record.i"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.cpp"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.h"\
	"..\..\program files\devstudio\vc\include\ace\managed_object.i"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.h"\
	"..\..\program files\devstudio\vc\include\ace\object_manager.i"\
	"..\..\program files\devstudio\vc\include\ace\os.h"\
	"..\..\program files\devstudio\vc\include\ace\os.i"\
	"..\..\program files\devstudio\vc\include\ace\streams.h"\
	"..\..\program files\devstudio\vc\include\ace\trace.h"\
	

"$(INTDIR)\textfile.obj" : $(SOURCE) $(DEP_CPP_TEXTF) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_TEXTF=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Trace.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\textfile.obj"	"$(INTDIR)\textfile.sbr" : $(SOURCE) $(DEP_CPP_TEXTF)\
 "$(INTDIR)"
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
	

"$(INTDIR)\Token.obj"	"$(INTDIR)\Token.sbr" : $(SOURCE) $(DEP_CPP_TOKEN)\
 "$(INTDIR)"
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
	

"$(INTDIR)\TokenBuffer.obj"	"$(INTDIR)\TokenBuffer.sbr" : $(SOURCE)\
 $(DEP_CPP_TOKENB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\trace.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_TRACE=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\Basic_Types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config-WinCE.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers.i"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Event_Handler.i"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\Handle_Set.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\iosfwd.h"\
	"..\ace_wrappers\ace\Local_Tokens.h"\
	"..\ace_wrappers\ace\Local_Tokens.i"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc.h"\
	"..\ace_wrappers\ace\Malloc.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Mem_Map.i"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Memory_Pool.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\Object_Manager.i"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\OS.i"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Reactor.i"\
	"..\ace_wrappers\ace\Reactor_Impl.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Config.i"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Service_Object.i"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\Shared_Object.i"\
	"..\ace_wrappers\ace\Signal.h"\
	"..\ace_wrappers\ace\Signal.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\SString.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Complex.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Complex.i"\
	"..\ace_wrappers\ace\SV_Semaphore_Simple.h"\
	"..\ace_wrappers\ace\SV_Semaphore_Simple.i"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch.i"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_Options.i"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Thread.i"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\ws2tcpip.h"\
	".\include\lockable.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\trace.h"\
	

"$(INTDIR)\trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_TRACE=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Atomic_Op.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\Containers.h"\
	"..\ace_wrappers\ace\Containers_T.cpp"\
	"..\ace_wrappers\ace\Containers_T.h"\
	"..\ace_wrappers\ace\Containers_T.i"\
	"..\ace_wrappers\ace\Event_Handler.h"\
	"..\ace_wrappers\ace\Free_List.cpp"\
	"..\ace_wrappers\ace\Free_List.h"\
	"..\ace_wrappers\ace\Free_List.i"\
	"..\ace_wrappers\ace\Handle_Set.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\Local_Tokens.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Malloc_T.cpp"\
	"..\ace_wrappers\ace\Malloc_T.h"\
	"..\ace_wrappers\ace\Malloc_T.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Map_Manager.cpp"\
	"..\ace_wrappers\ace\Map_Manager.h"\
	"..\ace_wrappers\ace\Map_Manager.i"\
	"..\ace_wrappers\ace\Mem_Map.h"\
	"..\ace_wrappers\ace\Memory_Pool.h"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\Reactor.h"\
	"..\ace_wrappers\ace\Service_Config.h"\
	"..\ace_wrappers\ace\Service_Object.h"\
	"..\ace_wrappers\ace\Shared_Object.h"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Svc_Conf_Tokens.h"\
	"..\ace_wrappers\ace\Synch.h"\
	"..\ace_wrappers\ace\Synch_Options.h"\
	"..\ace_wrappers\ace\Synch_T.cpp"\
	"..\ace_wrappers\ace\Synch_T.h"\
	"..\ace_wrappers\ace\Synch_T.i"\
	"..\ace_wrappers\ace\Thread.h"\
	"..\ace_wrappers\ace\Timer_Queue.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.cpp"\
	"..\ace_wrappers\ace\Timer_Queue_T.h"\
	"..\ace_wrappers\ace\Timer_Queue_T.i"\
	"..\ace_wrappers\ace\Trace.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\trace.obj"	"$(INTDIR)\trace.sbr" : $(SOURCE) $(DEP_CPP_TRACE)\
 "$(INTDIR)"
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
	

"$(INTDIR)\TreeParser.obj"	"$(INTDIR)\TreeParser.sbr" : $(SOURCE)\
 $(DEP_CPP_TREEP) "$(INTDIR)"
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
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Trace.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	".\include\utils.h"\
	

"$(INTDIR)\utils.obj"	"$(INTDIR)\utils.sbr" : $(SOURCE) $(DEP_CPP_UTILS)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\variant.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_VARIA=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\Basic_Types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config-WinCE.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\iosfwd.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Malloc_Base.h"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\Object_Manager.i"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\OS.i"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\SString.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Trace.h"\
	"..\ace_wrappers\ace\ws2tcpip.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\trace.h"\
	

"$(INTDIR)\variant.obj" : $(SOURCE) $(DEP_CPP_VARIA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_VARIA=\
	"..\ace_wrappers\ace\ACE.h"\
	"..\ace_wrappers\ace\ACE.i"\
	"..\ace_wrappers\ace\Basic_Types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\Log_Msg.h"\
	"..\ace_wrappers\ace\Log_Priority.h"\
	"..\ace_wrappers\ace\Log_Record.h"\
	"..\ace_wrappers\ace\Log_Record.i"\
	"..\ace_wrappers\ace\Managed_Object.cpp"\
	"..\ace_wrappers\ace\Managed_Object.h"\
	"..\ace_wrappers\ace\Managed_Object.i"\
	"..\ace_wrappers\ace\Object_Manager.h"\
	"..\ace_wrappers\ace\OS.h"\
	"..\ace_wrappers\ace\SString.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\Trace.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\variant.obj"	"$(INTDIR)\variant.sbr" : $(SOURCE) $(DEP_CPP_VARIA)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

