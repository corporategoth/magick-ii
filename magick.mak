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

ALL : "crypto - Win32 Release" "rx - Win32 Release" "m_bob - Win32 Release"\
 "$(OUTDIR)\magick.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"m_bob - Win32 ReleaseCLEAN" "rx - Win32 ReleaseCLEAN"\
 "crypto - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ANTLRException.obj"
	-@erase "$(INTDIR)\AST.obj"
	-@erase "$(INTDIR)\ASTFactory.obj"
	-@erase "$(INTDIR)\base.obj"
	-@erase "$(INTDIR)\BitSet.obj"
	-@erase "$(INTDIR)\bob.obj"
	-@erase "$(INTDIR)\chanserv.obj"
	-@erase "$(INTDIR)\CharBuffer.obj"
	-@erase "$(INTDIR)\CharScanner.obj"
	-@erase "$(INTDIR)\CommonToken.obj"
	-@erase "$(INTDIR)\confbase.obj"
	-@erase "$(INTDIR)\cryptstream.obj"
	-@erase "$(INTDIR)\datetime.obj"
	-@erase "$(INTDIR)\dccengine.obj"
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
 "..\support\ace_wrappers" /I "..\support\zlib-1.1.3" /D "NDEBUG" /D "WIN32" /D\
 "_CONSOLE" /D "_MBCS" /D "DES_UNROLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\magick.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib ace.lib zlib.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\magick.pdb" /machine:I386 /out:"$(OUTDIR)\magick.exe"\
 /libpath:"..\support\ace_wrappers\ace" /libpath:"..\support\zlib-1.1.3" 
LINK32_OBJS= \
	"$(INTDIR)\ANTLRException.obj" \
	"$(INTDIR)\AST.obj" \
	"$(INTDIR)\ASTFactory.obj" \
	"$(INTDIR)\base.obj" \
	"$(INTDIR)\BitSet.obj" \
	"$(INTDIR)\bob.obj" \
	"$(INTDIR)\chanserv.obj" \
	"$(INTDIR)\CharBuffer.obj" \
	"$(INTDIR)\CharScanner.obj" \
	"$(INTDIR)\CommonToken.obj" \
	"$(INTDIR)\confbase.obj" \
	"$(INTDIR)\cryptstream.obj" \
	"$(INTDIR)\datetime.obj" \
	"$(INTDIR)\dccengine.obj" \
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
	"$(INTDIR)\String.obj" \
	"$(INTDIR)\textfile.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\TokenBuffer.obj" \
	"$(INTDIR)\trace.obj" \
	"$(INTDIR)\TreeParser.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\variant.obj" \
	".\src\bob\Release\bob.lib" \
	".\SRC\DES\Release\crypto.lib" \
	".\SRC\RX-1.5\RX\rx\Release\rx.lib"

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

ALL : "crypto - Win32 Debug" "rx - Win32 Debug" "m_bob - Win32 Debug"\
 "$(OUTDIR)\magick.exe" "$(OUTDIR)\magick.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"m_bob - Win32 DebugCLEAN" "rx - Win32 DebugCLEAN"\
 "crypto - Win32 DebugCLEAN" 
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
 /I "..\support\ace_wrappers" /I "..\support\zlib-1.1.3" /D "_DEBUG" /D "DEBUG"\
 /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "DES_UNROLL" /Fr"$(INTDIR)\\"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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
	"$(INTDIR)\chanserv.sbr" \
	"$(INTDIR)\CharBuffer.sbr" \
	"$(INTDIR)\CharScanner.sbr" \
	"$(INTDIR)\CommonToken.sbr" \
	"$(INTDIR)\confbase.sbr" \
	"$(INTDIR)\cryptstream.sbr" \
	"$(INTDIR)\datetime.sbr" \
	"$(INTDIR)\dccengine.sbr" \
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
 odbccp32.lib aced.lib zlib.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\magick.pdb" /debug /machine:I386 /out:"$(OUTDIR)\magick.exe"\
 /pdbtype:sept /libpath:"..\support\ace_wrappers\ace"\
 /libpath:"..\support\zlib-1.1.3" 
LINK32_OBJS= \
	"$(INTDIR)\ANTLRException.obj" \
	"$(INTDIR)\AST.obj" \
	"$(INTDIR)\ASTFactory.obj" \
	"$(INTDIR)\base.obj" \
	"$(INTDIR)\BitSet.obj" \
	"$(INTDIR)\bob.obj" \
	"$(INTDIR)\chanserv.obj" \
	"$(INTDIR)\CharBuffer.obj" \
	"$(INTDIR)\CharScanner.obj" \
	"$(INTDIR)\CommonToken.obj" \
	"$(INTDIR)\confbase.obj" \
	"$(INTDIR)\cryptstream.obj" \
	"$(INTDIR)\datetime.obj" \
	"$(INTDIR)\dccengine.obj" \
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
	"$(INTDIR)\String.obj" \
	"$(INTDIR)\textfile.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\TokenBuffer.obj" \
	"$(INTDIR)\trace.obj" \
	"$(INTDIR)\TreeParser.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\variant.obj" \
	".\src\bob\Debug\bob.lib" \
	".\SRC\DES\Debug\crypto.lib" \
	".\SRC\RX-1.5\RX\rx\Debug\rx.lib"

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

!IF  "$(CFG)" == "magick - Win32 Release"

"rx - Win32 Release" : 
   cd ".\SRC\RX-1.5\RX\rx"
   $(MAKE) /$(MAKEFLAGS) /F ".\rx.mak" CFG="rx - Win32 Release" 
   cd "..\..\..\.."

"rx - Win32 ReleaseCLEAN" : 
   cd ".\SRC\RX-1.5\RX\rx"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F ".\rx.mak" CFG="rx - Win32 Release" RECURSE=1\
 
   cd "..\..\..\.."

!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

"rx - Win32 Debug" : 
   cd ".\SRC\RX-1.5\RX\rx"
   $(MAKE) /$(MAKEFLAGS) /F ".\rx.mak" CFG="rx - Win32 Debug" 
   cd "..\..\..\.."

"rx - Win32 DebugCLEAN" : 
   cd ".\SRC\RX-1.5\RX\rx"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F ".\rx.mak" CFG="rx - Win32 Debug" RECURSE=1 
   cd "..\..\..\.."

!ENDIF 

!IF  "$(CFG)" == "magick - Win32 Release"

"crypto - Win32 Release" : 
   cd ".\SRC\DES"
   $(MAKE) /$(MAKEFLAGS) /F ".\crypto.mak" CFG="crypto - Win32 Release" 
   cd "..\.."

"crypto - Win32 ReleaseCLEAN" : 
   cd ".\SRC\DES"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F ".\crypto.mak" CFG="crypto - Win32 Release"\
 RECURSE=1 
   cd "..\.."

!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

"crypto - Win32 Debug" : 
   cd ".\SRC\DES"
   $(MAKE) /$(MAKEFLAGS) /F ".\crypto.mak" CFG="crypto - Win32 Debug" 
   cd "..\.."

"crypto - Win32 DebugCLEAN" : 
   cd ".\SRC\DES"
   $(MAKE) /$(MAKEFLAGS) CLEAN /F ".\crypto.mak" CFG="crypto - Win32 Debug"\
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
	"..\support\ace_wrappers\ace\ace.h"\
	"..\support\ace_wrappers\ace\ace.i"\
	"..\support\ace_wrappers\ace\activation_queue.h"\
	"..\support\ace_wrappers\ace\activation_queue.i"\
	"..\support\ace_wrappers\ace\addr.h"\
	"..\support\ace_wrappers\ace\addr.i"\
	"..\support\ace_wrappers\ace\atomic_op.i"\
	"..\support\ace_wrappers\ace\auto_ptr.cpp"\
	"..\support\ace_wrappers\ace\auto_ptr.h"\
	"..\support\ace_wrappers\ace\auto_ptr.i"\
	"..\support\ace_wrappers\ace\basic_types.h"\
	"..\support\ace_wrappers\ace\basic_types.i"\
	"..\support\ace_wrappers\ace\config-win32-borland.h"\
	"..\support\ace_wrappers\ace\config-win32-common.h"\
	"..\support\ace_wrappers\ace\config-win32.h"\
	"..\support\ace_wrappers\ace\config.h"\
	"..\support\ace_wrappers\ace\connector.cpp"\
	"..\support\ace_wrappers\ace\connector.h"\
	"..\support\ace_wrappers\ace\connector.i"\
	"..\support\ace_wrappers\ace\containers.h"\
	"..\support\ace_wrappers\ace\containers.i"\
	"..\support\ace_wrappers\ace\containers_t.cpp"\
	"..\support\ace_wrappers\ace\containers_t.h"\
	"..\support\ace_wrappers\ace\containers_t.i"\
	"..\support\ace_wrappers\ace\dynamic.h"\
	"..\support\ace_wrappers\ace\dynamic.i"\
	"..\support\ace_wrappers\ace\event_handler.h"\
	"..\support\ace_wrappers\ace\event_handler.i"\
	"..\support\ace_wrappers\ace\free_list.cpp"\
	"..\support\ace_wrappers\ace\free_list.h"\
	"..\support\ace_wrappers\ace\free_list.i"\
	"..\support\ace_wrappers\ace\functor.h"\
	"..\support\ace_wrappers\ace\functor.i"\
	"..\support\ace_wrappers\ace\functor_t.cpp"\
	"..\support\ace_wrappers\ace\functor_t.h"\
	"..\support\ace_wrappers\ace\functor_t.i"\
	"..\support\ace_wrappers\ace\handle_set.h"\
	"..\support\ace_wrappers\ace\handle_set.i"\
	"..\support\ace_wrappers\ace\hash_map_manager.h"\
	"..\support\ace_wrappers\ace\hash_map_manager.i"\
	"..\support\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\support\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\support\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\support\ace_wrappers\ace\inc_user_config.h"\
	"..\support\ace_wrappers\ace\inet_addr.h"\
	"..\support\ace_wrappers\ace\inet_addr.i"\
	"..\support\ace_wrappers\ace\io_cntl_msg.h"\
	"..\support\ace_wrappers\ace\ipc_sap.h"\
	"..\support\ace_wrappers\ace\ipc_sap.i"\
	"..\support\ace_wrappers\ace\local_tokens.h"\
	"..\support\ace_wrappers\ace\local_tokens.i"\
	"..\support\ace_wrappers\ace\log_msg.h"\
	"..\support\ace_wrappers\ace\log_priority.h"\
	"..\support\ace_wrappers\ace\log_record.h"\
	"..\support\ace_wrappers\ace\log_record.i"\
	"..\support\ace_wrappers\ace\malloc.i"\
	"..\support\ace_wrappers\ace\malloc_base.h"\
	"..\support\ace_wrappers\ace\malloc_t.cpp"\
	"..\support\ace_wrappers\ace\malloc_t.h"\
	"..\support\ace_wrappers\ace\malloc_t.i"\
	"..\support\ace_wrappers\ace\managed_object.cpp"\
	"..\support\ace_wrappers\ace\managed_object.h"\
	"..\support\ace_wrappers\ace\managed_object.i"\
	"..\support\ace_wrappers\ace\map_manager.cpp"\
	"..\support\ace_wrappers\ace\map_manager.h"\
	"..\support\ace_wrappers\ace\map_manager.i"\
	"..\support\ace_wrappers\ace\mem_map.h"\
	"..\support\ace_wrappers\ace\mem_map.i"\
	"..\support\ace_wrappers\ace\memory_pool.h"\
	"..\support\ace_wrappers\ace\memory_pool.i"\
	"..\support\ace_wrappers\ace\message_block.h"\
	"..\support\ace_wrappers\ace\message_block.i"\
	"..\support\ace_wrappers\ace\message_block_t.cpp"\
	"..\support\ace_wrappers\ace\message_block_t.h"\
	"..\support\ace_wrappers\ace\message_block_t.i"\
	"..\support\ace_wrappers\ace\message_queue.h"\
	"..\support\ace_wrappers\ace\message_queue.i"\
	"..\support\ace_wrappers\ace\message_queue_t.cpp"\
	"..\support\ace_wrappers\ace\message_queue_t.h"\
	"..\support\ace_wrappers\ace\message_queue_t.i"\
	"..\support\ace_wrappers\ace\method_object.h"\
	"..\support\ace_wrappers\ace\method_request.h"\
	"..\support\ace_wrappers\ace\module.cpp"\
	"..\support\ace_wrappers\ace\module.h"\
	"..\support\ace_wrappers\ace\module.i"\
	"..\support\ace_wrappers\ace\object_manager.h"\
	"..\support\ace_wrappers\ace\object_manager.i"\
	"..\support\ace_wrappers\ace\os.h"\
	"..\support\ace_wrappers\ace\os.i"\
	"..\support\ace_wrappers\ace\reactor.h"\
	"..\support\ace_wrappers\ace\reactor.i"\
	"..\support\ace_wrappers\ace\reactor_impl.h"\
	"..\support\ace_wrappers\ace\service_config.h"\
	"..\support\ace_wrappers\ace\service_config.i"\
	"..\support\ace_wrappers\ace\service_object.h"\
	"..\support\ace_wrappers\ace\service_object.i"\
	"..\support\ace_wrappers\ace\service_repository.h"\
	"..\support\ace_wrappers\ace\service_repository.i"\
	"..\support\ace_wrappers\ace\service_types.h"\
	"..\support\ace_wrappers\ace\service_types.i"\
	"..\support\ace_wrappers\ace\shared_object.h"\
	"..\support\ace_wrappers\ace\shared_object.i"\
	"..\support\ace_wrappers\ace\signal.i"\
	"..\support\ace_wrappers\ace\singleton.cpp"\
	"..\support\ace_wrappers\ace\singleton.h"\
	"..\support\ace_wrappers\ace\singleton.i"\
	"..\support\ace_wrappers\ace\sock.h"\
	"..\support\ace_wrappers\ace\sock.i"\
	"..\support\ace_wrappers\ace\sock_connector.h"\
	"..\support\ace_wrappers\ace\sock_connector.i"\
	"..\support\ace_wrappers\ace\sock_io.h"\
	"..\support\ace_wrappers\ace\sock_io.i"\
	"..\support\ace_wrappers\ace\sock_stream.h"\
	"..\support\ace_wrappers\ace\sock_stream.i"\
	"..\support\ace_wrappers\ace\sstring.h"\
	"..\support\ace_wrappers\ace\sstring.i"\
	"..\support\ace_wrappers\ace\strategies.h"\
	"..\support\ace_wrappers\ace\strategies.i"\
	"..\support\ace_wrappers\ace\strategies_t.cpp"\
	"..\support\ace_wrappers\ace\strategies_t.h"\
	"..\support\ace_wrappers\ace\strategies_t.i"\
	"..\support\ace_wrappers\ace\stream_modules.cpp"\
	"..\support\ace_wrappers\ace\stream_modules.h"\
	"..\support\ace_wrappers\ace\stream_modules.i"\
	"..\support\ace_wrappers\ace\streams.h"\
	"..\support\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\support\ace_wrappers\ace\svc_handler.cpp"\
	"..\support\ace_wrappers\ace\svc_handler.h"\
	"..\support\ace_wrappers\ace\svc_handler.i"\
	"..\support\ace_wrappers\ace\synch.h"\
	"..\support\ace_wrappers\ace\synch.i"\
	"..\support\ace_wrappers\ace\synch_options.h"\
	"..\support\ace_wrappers\ace\synch_options.i"\
	"..\support\ace_wrappers\ace\synch_t.cpp"\
	"..\support\ace_wrappers\ace\synch_t.h"\
	"..\support\ace_wrappers\ace\synch_t.i"\
	"..\support\ace_wrappers\ace\task.h"\
	"..\support\ace_wrappers\ace\task.i"\
	"..\support\ace_wrappers\ace\task_t.cpp"\
	"..\support\ace_wrappers\ace\task_t.h"\
	"..\support\ace_wrappers\ace\task_t.i"\
	"..\support\ace_wrappers\ace\thread.h"\
	"..\support\ace_wrappers\ace\thread.i"\
	"..\support\ace_wrappers\ace\thread_manager.h"\
	"..\support\ace_wrappers\ace\thread_manager.i"\
	"..\support\ace_wrappers\ace\time_value.h"\
	"..\support\ace_wrappers\ace\timer_queue.h"\
	"..\support\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\support\ace_wrappers\ace\timer_queue_t.h"\
	"..\support\ace_wrappers\ace\timer_queue_t.i"\
	"..\support\ace_wrappers\ace\trace.h"\
	"..\support\ace_wrappers\ace\wfmo_reactor.h"\
	"..\support\ace_wrappers\ace\wfmo_reactor.i"\
	"..\support\zlib-1.1.3\zconf.h"\
	"..\support\zlib-1.1.3\zlib.h"\
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
	".\include\pch.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\base.obj" : $(SOURCE) $(DEP_CPP_BASE_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_BASE_=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\connector.cpp"\
	"..\ace_wrappers\ace\connector.h"\
	"..\ace_wrappers\ace\connector.i"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\dynamic.h"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\ipc_sap.h"\
	"..\ace_wrappers\ace\ipc_sap.i"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\singleton.cpp"\
	"..\ace_wrappers\ace\singleton.h"\
	"..\ace_wrappers\ace\singleton.i"\
	"..\ace_wrappers\ace\sock.h"\
	"..\ace_wrappers\ace\sock.i"\
	"..\ace_wrappers\ace\sock_connector.h"\
	"..\ace_wrappers\ace\sock_connector.i"\
	"..\ace_wrappers\ace\sock_io.h"\
	"..\ace_wrappers\ace\sock_io.i"\
	"..\ace_wrappers\ace\sock_stream.h"\
	"..\ace_wrappers\ace\sock_stream.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\svc_handler.cpp"\
	"..\ace_wrappers\ace\svc_handler.h"\
	"..\ace_wrappers\ace\svc_handler.i"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\time_value.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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
DEP_CPP_BITSE=\
	".\include\antlr\bitset.hpp"\
	".\include\antlr\config.hpp"\
	

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\BitSet.obj" : $(SOURCE) $(DEP_CPP_BITSE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\BitSet.obj"	"$(INTDIR)\BitSet.sbr" : $(SOURCE) $(DEP_CPP_BITSE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\bob.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_BOB_C=\
	"..\support\ace_wrappers\ace\ace.h"\
	"..\support\ace_wrappers\ace\ace.i"\
	"..\support\ace_wrappers\ace\activation_queue.h"\
	"..\support\ace_wrappers\ace\activation_queue.i"\
	"..\support\ace_wrappers\ace\addr.h"\
	"..\support\ace_wrappers\ace\addr.i"\
	"..\support\ace_wrappers\ace\atomic_op.i"\
	"..\support\ace_wrappers\ace\auto_ptr.cpp"\
	"..\support\ace_wrappers\ace\auto_ptr.h"\
	"..\support\ace_wrappers\ace\auto_ptr.i"\
	"..\support\ace_wrappers\ace\basic_types.h"\
	"..\support\ace_wrappers\ace\basic_types.i"\
	"..\support\ace_wrappers\ace\config-win32-borland.h"\
	"..\support\ace_wrappers\ace\config-win32-common.h"\
	"..\support\ace_wrappers\ace\config-win32.h"\
	"..\support\ace_wrappers\ace\config.h"\
	"..\support\ace_wrappers\ace\connector.cpp"\
	"..\support\ace_wrappers\ace\connector.h"\
	"..\support\ace_wrappers\ace\connector.i"\
	"..\support\ace_wrappers\ace\containers.h"\
	"..\support\ace_wrappers\ace\containers.i"\
	"..\support\ace_wrappers\ace\containers_t.cpp"\
	"..\support\ace_wrappers\ace\containers_t.h"\
	"..\support\ace_wrappers\ace\containers_t.i"\
	"..\support\ace_wrappers\ace\dynamic.h"\
	"..\support\ace_wrappers\ace\dynamic.i"\
	"..\support\ace_wrappers\ace\event_handler.h"\
	"..\support\ace_wrappers\ace\event_handler.i"\
	"..\support\ace_wrappers\ace\free_list.cpp"\
	"..\support\ace_wrappers\ace\free_list.h"\
	"..\support\ace_wrappers\ace\free_list.i"\
	"..\support\ace_wrappers\ace\functor.h"\
	"..\support\ace_wrappers\ace\functor.i"\
	"..\support\ace_wrappers\ace\functor_t.cpp"\
	"..\support\ace_wrappers\ace\functor_t.h"\
	"..\support\ace_wrappers\ace\functor_t.i"\
	"..\support\ace_wrappers\ace\handle_set.h"\
	"..\support\ace_wrappers\ace\handle_set.i"\
	"..\support\ace_wrappers\ace\hash_map_manager.h"\
	"..\support\ace_wrappers\ace\hash_map_manager.i"\
	"..\support\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\support\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\support\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\support\ace_wrappers\ace\inc_user_config.h"\
	"..\support\ace_wrappers\ace\inet_addr.h"\
	"..\support\ace_wrappers\ace\inet_addr.i"\
	"..\support\ace_wrappers\ace\io_cntl_msg.h"\
	"..\support\ace_wrappers\ace\ipc_sap.h"\
	"..\support\ace_wrappers\ace\ipc_sap.i"\
	"..\support\ace_wrappers\ace\local_tokens.h"\
	"..\support\ace_wrappers\ace\local_tokens.i"\
	"..\support\ace_wrappers\ace\log_msg.h"\
	"..\support\ace_wrappers\ace\log_priority.h"\
	"..\support\ace_wrappers\ace\log_record.h"\
	"..\support\ace_wrappers\ace\log_record.i"\
	"..\support\ace_wrappers\ace\malloc.i"\
	"..\support\ace_wrappers\ace\malloc_base.h"\
	"..\support\ace_wrappers\ace\malloc_t.cpp"\
	"..\support\ace_wrappers\ace\malloc_t.h"\
	"..\support\ace_wrappers\ace\malloc_t.i"\
	"..\support\ace_wrappers\ace\managed_object.cpp"\
	"..\support\ace_wrappers\ace\managed_object.h"\
	"..\support\ace_wrappers\ace\managed_object.i"\
	"..\support\ace_wrappers\ace\map_manager.cpp"\
	"..\support\ace_wrappers\ace\map_manager.h"\
	"..\support\ace_wrappers\ace\map_manager.i"\
	"..\support\ace_wrappers\ace\mem_map.h"\
	"..\support\ace_wrappers\ace\mem_map.i"\
	"..\support\ace_wrappers\ace\memory_pool.h"\
	"..\support\ace_wrappers\ace\memory_pool.i"\
	"..\support\ace_wrappers\ace\message_block.h"\
	"..\support\ace_wrappers\ace\message_block.i"\
	"..\support\ace_wrappers\ace\message_block_t.cpp"\
	"..\support\ace_wrappers\ace\message_block_t.h"\
	"..\support\ace_wrappers\ace\message_block_t.i"\
	"..\support\ace_wrappers\ace\message_queue.h"\
	"..\support\ace_wrappers\ace\message_queue.i"\
	"..\support\ace_wrappers\ace\message_queue_t.cpp"\
	"..\support\ace_wrappers\ace\message_queue_t.h"\
	"..\support\ace_wrappers\ace\message_queue_t.i"\
	"..\support\ace_wrappers\ace\method_object.h"\
	"..\support\ace_wrappers\ace\method_request.h"\
	"..\support\ace_wrappers\ace\module.cpp"\
	"..\support\ace_wrappers\ace\module.h"\
	"..\support\ace_wrappers\ace\module.i"\
	"..\support\ace_wrappers\ace\object_manager.h"\
	"..\support\ace_wrappers\ace\object_manager.i"\
	"..\support\ace_wrappers\ace\os.h"\
	"..\support\ace_wrappers\ace\os.i"\
	"..\support\ace_wrappers\ace\reactor.h"\
	"..\support\ace_wrappers\ace\reactor.i"\
	"..\support\ace_wrappers\ace\reactor_impl.h"\
	"..\support\ace_wrappers\ace\service_config.h"\
	"..\support\ace_wrappers\ace\service_config.i"\
	"..\support\ace_wrappers\ace\service_object.h"\
	"..\support\ace_wrappers\ace\service_object.i"\
	"..\support\ace_wrappers\ace\service_repository.h"\
	"..\support\ace_wrappers\ace\service_repository.i"\
	"..\support\ace_wrappers\ace\service_types.h"\
	"..\support\ace_wrappers\ace\service_types.i"\
	"..\support\ace_wrappers\ace\shared_object.h"\
	"..\support\ace_wrappers\ace\shared_object.i"\
	"..\support\ace_wrappers\ace\signal.i"\
	"..\support\ace_wrappers\ace\singleton.cpp"\
	"..\support\ace_wrappers\ace\singleton.h"\
	"..\support\ace_wrappers\ace\singleton.i"\
	"..\support\ace_wrappers\ace\sock.h"\
	"..\support\ace_wrappers\ace\sock.i"\
	"..\support\ace_wrappers\ace\sock_connector.h"\
	"..\support\ace_wrappers\ace\sock_connector.i"\
	"..\support\ace_wrappers\ace\sock_io.h"\
	"..\support\ace_wrappers\ace\sock_io.i"\
	"..\support\ace_wrappers\ace\sock_stream.h"\
	"..\support\ace_wrappers\ace\sock_stream.i"\
	"..\support\ace_wrappers\ace\sstring.h"\
	"..\support\ace_wrappers\ace\sstring.i"\
	"..\support\ace_wrappers\ace\strategies.h"\
	"..\support\ace_wrappers\ace\strategies.i"\
	"..\support\ace_wrappers\ace\strategies_t.cpp"\
	"..\support\ace_wrappers\ace\strategies_t.h"\
	"..\support\ace_wrappers\ace\strategies_t.i"\
	"..\support\ace_wrappers\ace\stream_modules.cpp"\
	"..\support\ace_wrappers\ace\stream_modules.h"\
	"..\support\ace_wrappers\ace\stream_modules.i"\
	"..\support\ace_wrappers\ace\streams.h"\
	"..\support\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\support\ace_wrappers\ace\svc_handler.cpp"\
	"..\support\ace_wrappers\ace\svc_handler.h"\
	"..\support\ace_wrappers\ace\svc_handler.i"\
	"..\support\ace_wrappers\ace\synch.h"\
	"..\support\ace_wrappers\ace\synch.i"\
	"..\support\ace_wrappers\ace\synch_options.h"\
	"..\support\ace_wrappers\ace\synch_options.i"\
	"..\support\ace_wrappers\ace\synch_t.cpp"\
	"..\support\ace_wrappers\ace\synch_t.h"\
	"..\support\ace_wrappers\ace\synch_t.i"\
	"..\support\ace_wrappers\ace\task.h"\
	"..\support\ace_wrappers\ace\task.i"\
	"..\support\ace_wrappers\ace\task_t.cpp"\
	"..\support\ace_wrappers\ace\task_t.h"\
	"..\support\ace_wrappers\ace\task_t.i"\
	"..\support\ace_wrappers\ace\thread.h"\
	"..\support\ace_wrappers\ace\thread.i"\
	"..\support\ace_wrappers\ace\thread_manager.h"\
	"..\support\ace_wrappers\ace\thread_manager.i"\
	"..\support\ace_wrappers\ace\time_value.h"\
	"..\support\ace_wrappers\ace\timer_queue.h"\
	"..\support\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\support\ace_wrappers\ace\timer_queue_t.h"\
	"..\support\ace_wrappers\ace\timer_queue_t.i"\
	"..\support\ace_wrappers\ace\trace.h"\
	"..\support\ace_wrappers\ace\wfmo_reactor.h"\
	"..\support\ace_wrappers\ace\wfmo_reactor.i"\
	"..\support\zlib-1.1.3\zconf.h"\
	"..\support\zlib-1.1.3\zlib.h"\
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
	".\include\pch.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\bob.obj" : $(SOURCE) $(DEP_CPP_BOB_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_BOB_C=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\connector.cpp"\
	"..\ace_wrappers\ace\connector.h"\
	"..\ace_wrappers\ace\connector.i"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\dynamic.h"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\ipc_sap.h"\
	"..\ace_wrappers\ace\ipc_sap.i"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\singleton.cpp"\
	"..\ace_wrappers\ace\singleton.h"\
	"..\ace_wrappers\ace\singleton.i"\
	"..\ace_wrappers\ace\sock.h"\
	"..\ace_wrappers\ace\sock.i"\
	"..\ace_wrappers\ace\sock_connector.h"\
	"..\ace_wrappers\ace\sock_connector.i"\
	"..\ace_wrappers\ace\sock_io.h"\
	"..\ace_wrappers\ace\sock_io.i"\
	"..\ace_wrappers\ace\sock_stream.h"\
	"..\ace_wrappers\ace\sock_stream.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\svc_handler.cpp"\
	"..\ace_wrappers\ace\svc_handler.h"\
	"..\ace_wrappers\ace\svc_handler.i"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\time_value.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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

SOURCE=.\src\chanserv.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_CHANS=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\activation_queue.i"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\addr.i"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\auto_ptr.cpp"\
	"..\ace_wrappers\ace\auto_ptr.h"\
	"..\ace_wrappers\ace\auto_ptr.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\connector.cpp"\
	"..\ace_wrappers\ace\connector.h"\
	"..\ace_wrappers\ace\connector.i"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers.i"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\dynamic.h"\
	"..\ace_wrappers\ace\dynamic.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\event_handler.i"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor.i"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\handle_set.i"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager.i"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\inet_addr.i"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\ipc_sap.h"\
	"..\ace_wrappers\ace\ipc_sap.i"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\local_tokens.i"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\mem_map.i"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\memory_pool.i"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block.i"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue.i"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor.i"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_config.i"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_object.i"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_repository.i"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\service_types.i"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\shared_object.i"\
	"..\ace_wrappers\ace\signal.i"\
	"..\ace_wrappers\ace\singleton.cpp"\
	"..\ace_wrappers\ace\singleton.h"\
	"..\ace_wrappers\ace\singleton.i"\
	"..\ace_wrappers\ace\sock.h"\
	"..\ace_wrappers\ace\sock.i"\
	"..\ace_wrappers\ace\sock_connector.h"\
	"..\ace_wrappers\ace\sock_connector.i"\
	"..\ace_wrappers\ace\sock_io.h"\
	"..\ace_wrappers\ace\sock_io.i"\
	"..\ace_wrappers\ace\sock_stream.h"\
	"..\ace_wrappers\ace\sock_stream.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies.i"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\svc_handler.cpp"\
	"..\ace_wrappers\ace\svc_handler.h"\
	"..\ace_wrappers\ace\svc_handler.i"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch.i"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_options.i"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task.i"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread.i"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\thread_manager.i"\
	"..\ace_wrappers\ace\time_value.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	"..\ace_wrappers\ace\wfmo_reactor.i"\
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
	".\include\pch.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

"$(INTDIR)\chanserv.obj" : $(SOURCE) $(DEP_CPP_CHANS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_CHANS=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\connector.cpp"\
	"..\ace_wrappers\ace\connector.h"\
	"..\ace_wrappers\ace\connector.i"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\dynamic.h"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\ipc_sap.h"\
	"..\ace_wrappers\ace\ipc_sap.i"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\singleton.cpp"\
	"..\ace_wrappers\ace\singleton.h"\
	"..\ace_wrappers\ace\singleton.i"\
	"..\ace_wrappers\ace\sock.h"\
	"..\ace_wrappers\ace\sock.i"\
	"..\ace_wrappers\ace\sock_connector.h"\
	"..\ace_wrappers\ace\sock_connector.i"\
	"..\ace_wrappers\ace\sock_io.h"\
	"..\ace_wrappers\ace\sock_io.i"\
	"..\ace_wrappers\ace\sock_stream.h"\
	"..\ace_wrappers\ace\sock_stream.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\svc_handler.cpp"\
	"..\ace_wrappers\ace\svc_handler.h"\
	"..\ace_wrappers\ace\svc_handler.i"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\time_value.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\activation_queue.i"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\addr.i"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\auto_ptr.cpp"\
	"..\ace_wrappers\ace\auto_ptr.h"\
	"..\ace_wrappers\ace\auto_ptr.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\connector.cpp"\
	"..\ace_wrappers\ace\connector.h"\
	"..\ace_wrappers\ace\connector.i"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers.i"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\dynamic.h"\
	"..\ace_wrappers\ace\dynamic.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\event_handler.i"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor.i"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\handle_set.i"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager.i"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\inet_addr.i"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\ipc_sap.h"\
	"..\ace_wrappers\ace\ipc_sap.i"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\local_tokens.i"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\mem_map.i"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\memory_pool.i"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block.i"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue.i"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor.i"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_config.i"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_object.i"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_repository.i"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\service_types.i"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\shared_object.i"\
	"..\ace_wrappers\ace\signal.i"\
	"..\ace_wrappers\ace\singleton.cpp"\
	"..\ace_wrappers\ace\singleton.h"\
	"..\ace_wrappers\ace\singleton.i"\
	"..\ace_wrappers\ace\sock.h"\
	"..\ace_wrappers\ace\sock.i"\
	"..\ace_wrappers\ace\sock_connector.h"\
	"..\ace_wrappers\ace\sock_connector.i"\
	"..\ace_wrappers\ace\sock_io.h"\
	"..\ace_wrappers\ace\sock_io.i"\
	"..\ace_wrappers\ace\sock_stream.h"\
	"..\ace_wrappers\ace\sock_stream.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies.i"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\svc_handler.cpp"\
	"..\ace_wrappers\ace\svc_handler.h"\
	"..\ace_wrappers\ace\svc_handler.i"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch.i"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_options.i"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task.i"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread.i"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\thread_manager.i"\
	"..\ace_wrappers\ace\time_value.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	"..\ace_wrappers\ace\wfmo_reactor.i"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\pch.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

"$(INTDIR)\confbase.obj" : $(SOURCE) $(DEP_CPP_CONFB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_CONFB=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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
	".\include\datetime.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	

"$(INTDIR)\datetime.obj" : $(SOURCE) $(DEP_CPP_DATET) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_DATET=\
	".\include\datetime.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	

"$(INTDIR)\datetime.obj"	"$(INTDIR)\datetime.sbr" : $(SOURCE) $(DEP_CPP_DATET)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\dccengine.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_DCCEN=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\activation_queue.i"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\addr.i"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers.i"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\event_handler.i"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor.i"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\handle_set.i"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager.i"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\inet_addr.i"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\mem_map.i"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\memory_pool.i"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block.i"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue.i"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor.i"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_config.i"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_object.i"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_repository.i"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\service_types.i"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\shared_object.i"\
	"..\ace_wrappers\ace\signal.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies.i"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch.i"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_options.i"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task.i"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread.i"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\thread_manager.i"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	"..\ace_wrappers\ace\wfmo_reactor.i"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\dccengine.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\dccengine.obj" : $(SOURCE) $(DEP_CPP_DCCEN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_DCCEN=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\dccengine.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\dccengine.obj"	"$(INTDIR)\dccengine.sbr" : $(SOURCE)\
 $(DEP_CPP_DCCEN) "$(INTDIR)"
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
	".\include\EscLexer.hpp"\
	".\include\esclexertokentypes.hpp"\
	".\include\mstring.h"\
	

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
	".\include\esclexertokentypes.hpp"\
	".\include\EscParser.hpp"\
	".\include\mstring.h"\
	

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
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\activation_queue.i"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers.i"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\event_handler.i"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor.i"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\handle_set.i"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager.i"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\mem_map.i"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\memory_pool.i"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block.i"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue.i"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor.i"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_config.i"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_object.i"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_repository.i"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\service_types.i"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\shared_object.i"\
	"..\ace_wrappers\ace\signal.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies.i"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch.i"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_options.i"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task.i"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread.i"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\thread_manager.i"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	"..\ace_wrappers\ace\wfmo_reactor.i"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	".\include\utils.h"\
	

"$(INTDIR)\fileconf.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_FILEC=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\activation_queue.i"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\addr.i"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\auto_ptr.cpp"\
	"..\ace_wrappers\ace\auto_ptr.h"\
	"..\ace_wrappers\ace\auto_ptr.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\connector.cpp"\
	"..\ace_wrappers\ace\connector.h"\
	"..\ace_wrappers\ace\connector.i"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers.i"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\dynamic.h"\
	"..\ace_wrappers\ace\dynamic.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\event_handler.i"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor.i"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\handle_set.i"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager.i"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\inet_addr.i"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\ipc_sap.h"\
	"..\ace_wrappers\ace\ipc_sap.i"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\local_tokens.i"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\mem_map.i"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\memory_pool.i"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block.i"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue.i"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor.i"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_config.i"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_object.i"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_repository.i"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\service_types.i"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\shared_object.i"\
	"..\ace_wrappers\ace\signal.i"\
	"..\ace_wrappers\ace\singleton.cpp"\
	"..\ace_wrappers\ace\singleton.h"\
	"..\ace_wrappers\ace\singleton.i"\
	"..\ace_wrappers\ace\sock.h"\
	"..\ace_wrappers\ace\sock.i"\
	"..\ace_wrappers\ace\sock_connector.h"\
	"..\ace_wrappers\ace\sock_connector.i"\
	"..\ace_wrappers\ace\sock_io.h"\
	"..\ace_wrappers\ace\sock_io.i"\
	"..\ace_wrappers\ace\sock_stream.h"\
	"..\ace_wrappers\ace\sock_stream.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies.i"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\svc_handler.cpp"\
	"..\ace_wrappers\ace\svc_handler.h"\
	"..\ace_wrappers\ace\svc_handler.i"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch.i"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_options.i"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task.i"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread.i"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\thread_manager.i"\
	"..\ace_wrappers\ace\time_value.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	"..\ace_wrappers\ace\wfmo_reactor.i"\
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
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\nickserv.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\ircsocket.obj" : $(SOURCE) $(DEP_CPP_IRCSO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_IRCSO=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\auto_ptr.cpp"\
	"..\ace_wrappers\ace\auto_ptr.h"\
	"..\ace_wrappers\ace\auto_ptr.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\connector.cpp"\
	"..\ace_wrappers\ace\connector.h"\
	"..\ace_wrappers\ace\connector.i"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\dynamic.h"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\ipc_sap.h"\
	"..\ace_wrappers\ace\ipc_sap.i"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\singleton.cpp"\
	"..\ace_wrappers\ace\singleton.h"\
	"..\ace_wrappers\ace\singleton.i"\
	"..\ace_wrappers\ace\sock.h"\
	"..\ace_wrappers\ace\sock.i"\
	"..\ace_wrappers\ace\sock_connector.h"\
	"..\ace_wrappers\ace\sock_connector.i"\
	"..\ace_wrappers\ace\sock_io.h"\
	"..\ace_wrappers\ace\sock_io.i"\
	"..\ace_wrappers\ace\sock_stream.h"\
	"..\ace_wrappers\ace\sock_stream.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\svc_handler.cpp"\
	"..\ace_wrappers\ace\svc_handler.h"\
	"..\ace_wrappers\ace\svc_handler.i"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\time_value.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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
	".\include\log.h"\
	".\include\magick.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\nickserv.h"\
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
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\activation_queue.i"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers.i"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\event_handler.i"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor.i"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\handle_set.i"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager.i"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\local_tokens.i"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\mem_map.i"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\memory_pool.i"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block.i"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue.i"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor.i"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_config.i"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_object.i"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_repository.i"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\service_types.i"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\shared_object.i"\
	"..\ace_wrappers\ace\signal.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies.i"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch.i"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_options.i"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task.i"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread.i"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\thread_manager.i"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	"..\ace_wrappers\ace\wfmo_reactor.i"\
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
	

"$(INTDIR)\lockable.obj" : $(SOURCE) $(DEP_CPP_LOCKA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_LOCKA=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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
DEP_CPP_LOG_C=\
	".\include\log.h"\
	".\include\mstring.h"\
	

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\log.obj" : $(SOURCE) $(DEP_CPP_LOG_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) $(DEP_CPP_LOG_C)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\magick.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_MAGIC=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\activation_queue.i"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\addr.i"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\connector.cpp"\
	"..\ace_wrappers\ace\connector.h"\
	"..\ace_wrappers\ace\connector.i"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers.i"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\dynamic.h"\
	"..\ace_wrappers\ace\dynamic.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\event_handler.i"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor.i"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\handle_set.i"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager.i"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\inet_addr.i"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\ipc_sap.h"\
	"..\ace_wrappers\ace\ipc_sap.i"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\local_tokens.i"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\mem_map.i"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\memory_pool.i"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block.i"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue.i"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor.i"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_config.i"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_object.i"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_repository.i"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\service_types.i"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\shared_object.i"\
	"..\ace_wrappers\ace\signal.i"\
	"..\ace_wrappers\ace\singleton.cpp"\
	"..\ace_wrappers\ace\singleton.h"\
	"..\ace_wrappers\ace\singleton.i"\
	"..\ace_wrappers\ace\sock.h"\
	"..\ace_wrappers\ace\sock.i"\
	"..\ace_wrappers\ace\sock_connector.h"\
	"..\ace_wrappers\ace\sock_connector.i"\
	"..\ace_wrappers\ace\sock_io.h"\
	"..\ace_wrappers\ace\sock_io.i"\
	"..\ace_wrappers\ace\sock_stream.h"\
	"..\ace_wrappers\ace\sock_stream.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies.i"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\svc_handler.cpp"\
	"..\ace_wrappers\ace\svc_handler.h"\
	"..\ace_wrappers\ace\svc_handler.i"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch.i"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_options.i"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task.i"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread.i"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\thread_manager.i"\
	"..\ace_wrappers\ace\time_value.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	"..\ace_wrappers\ace\wfmo_reactor.i"\
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
	".\include\utils.h"\
	

"$(INTDIR)\magick.obj" : $(SOURCE) $(DEP_CPP_MAGIC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MAGIC=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\connector.cpp"\
	"..\ace_wrappers\ace\connector.h"\
	"..\ace_wrappers\ace\connector.i"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\dynamic.h"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\ipc_sap.h"\
	"..\ace_wrappers\ace\ipc_sap.i"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\singleton.cpp"\
	"..\ace_wrappers\ace\singleton.h"\
	"..\ace_wrappers\ace\singleton.i"\
	"..\ace_wrappers\ace\sock.h"\
	"..\ace_wrappers\ace\sock.i"\
	"..\ace_wrappers\ace\sock_connector.h"\
	"..\ace_wrappers\ace\sock_connector.i"\
	"..\ace_wrappers\ace\sock_io.h"\
	"..\ace_wrappers\ace\sock_io.i"\
	"..\ace_wrappers\ace\sock_stream.h"\
	"..\ace_wrappers\ace\sock_stream.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\svc_handler.cpp"\
	"..\ace_wrappers\ace\svc_handler.h"\
	"..\ace_wrappers\ace\svc_handler.i"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\time_value.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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
	".\include\utils.h"\
	

"$(INTDIR)\magick.obj"	"$(INTDIR)\magick.sbr" : $(SOURCE) $(DEP_CPP_MAGIC)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\main.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_MAIN_=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\activation_queue.i"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\addr.i"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\connector.cpp"\
	"..\ace_wrappers\ace\connector.h"\
	"..\ace_wrappers\ace\connector.i"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers.i"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\dynamic.h"\
	"..\ace_wrappers\ace\dynamic.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\event_handler.i"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor.i"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\handle_set.i"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager.i"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\inet_addr.i"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\ipc_sap.h"\
	"..\ace_wrappers\ace\ipc_sap.i"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\local_tokens.i"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\mem_map.i"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\memory_pool.i"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block.i"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue.i"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor.i"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_config.i"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_object.i"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_repository.i"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\service_types.i"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\shared_object.i"\
	"..\ace_wrappers\ace\signal.i"\
	"..\ace_wrappers\ace\singleton.cpp"\
	"..\ace_wrappers\ace\singleton.h"\
	"..\ace_wrappers\ace\singleton.i"\
	"..\ace_wrappers\ace\sock.h"\
	"..\ace_wrappers\ace\sock.i"\
	"..\ace_wrappers\ace\sock_connector.h"\
	"..\ace_wrappers\ace\sock_connector.i"\
	"..\ace_wrappers\ace\sock_io.h"\
	"..\ace_wrappers\ace\sock_io.i"\
	"..\ace_wrappers\ace\sock_stream.h"\
	"..\ace_wrappers\ace\sock_stream.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies.i"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\svc_handler.cpp"\
	"..\ace_wrappers\ace\svc_handler.h"\
	"..\ace_wrappers\ace\svc_handler.i"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch.i"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_options.i"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task.i"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread.i"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\thread_manager.i"\
	"..\ace_wrappers\ace\time_value.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	"..\ace_wrappers\ace\wfmo_reactor.i"\
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
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MAIN_=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\connector.cpp"\
	"..\ace_wrappers\ace\connector.h"\
	"..\ace_wrappers\ace\connector.i"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\dynamic.h"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\ipc_sap.h"\
	"..\ace_wrappers\ace\ipc_sap.i"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\singleton.cpp"\
	"..\ace_wrappers\ace\singleton.h"\
	"..\ace_wrappers\ace\singleton.i"\
	"..\ace_wrappers\ace\sock.h"\
	"..\ace_wrappers\ace\sock.i"\
	"..\ace_wrappers\ace\sock_connector.h"\
	"..\ace_wrappers\ace\sock_connector.i"\
	"..\ace_wrappers\ace\sock_io.h"\
	"..\ace_wrappers\ace\sock_io.i"\
	"..\ace_wrappers\ace\sock_stream.h"\
	"..\ace_wrappers\ace\sock_stream.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\svc_handler.cpp"\
	"..\ace_wrappers\ace\svc_handler.h"\
	"..\ace_wrappers\ace\svc_handler.i"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\time_value.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\trace.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\utils.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

"$(INTDIR)\mstream.obj" : $(SOURCE) $(DEP_CPP_MSTRE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MSTRE=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\trace.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\utils.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

"$(INTDIR)\mstream.obj"	"$(INTDIR)\mstream.sbr" : $(SOURCE) $(DEP_CPP_MSTRE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\mstring.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_MSTRI=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\trace.h"\
	".\include\mstring.h"\
	".\include\rx\rxposix.h"\
	

"$(INTDIR)\mstring.obj" : $(SOURCE) $(DEP_CPP_MSTRI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_MSTRI=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\trace.h"\
	".\include\mstring.h"\
	".\include\rx\rxposix.h"\
	

"$(INTDIR)\mstring.obj"	"$(INTDIR)\mstring.sbr" : $(SOURCE) $(DEP_CPP_MSTRI)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\nickserv.cpp

!IF  "$(CFG)" == "magick - Win32 Release"

DEP_CPP_NICKS=\
	"..\support\ace_wrappers\ace\ace.h"\
	"..\support\ace_wrappers\ace\ace.i"\
	"..\support\ace_wrappers\ace\activation_queue.h"\
	"..\support\ace_wrappers\ace\activation_queue.i"\
	"..\support\ace_wrappers\ace\addr.h"\
	"..\support\ace_wrappers\ace\addr.i"\
	"..\support\ace_wrappers\ace\atomic_op.i"\
	"..\support\ace_wrappers\ace\auto_ptr.cpp"\
	"..\support\ace_wrappers\ace\auto_ptr.h"\
	"..\support\ace_wrappers\ace\auto_ptr.i"\
	"..\support\ace_wrappers\ace\basic_types.h"\
	"..\support\ace_wrappers\ace\basic_types.i"\
	"..\support\ace_wrappers\ace\config-win32-borland.h"\
	"..\support\ace_wrappers\ace\config-win32-common.h"\
	"..\support\ace_wrappers\ace\config-win32.h"\
	"..\support\ace_wrappers\ace\config-WinCE.h"\
	"..\support\ace_wrappers\ace\config.h"\
	"..\support\ace_wrappers\ace\connector.cpp"\
	"..\support\ace_wrappers\ace\connector.h"\
	"..\support\ace_wrappers\ace\connector.i"\
	"..\support\ace_wrappers\ace\containers.h"\
	"..\support\ace_wrappers\ace\containers.i"\
	"..\support\ace_wrappers\ace\containers_t.cpp"\
	"..\support\ace_wrappers\ace\containers_t.h"\
	"..\support\ace_wrappers\ace\containers_t.i"\
	"..\support\ace_wrappers\ace\dynamic.h"\
	"..\support\ace_wrappers\ace\dynamic.i"\
	"..\support\ace_wrappers\ace\event_handler.h"\
	"..\support\ace_wrappers\ace\event_handler.i"\
	"..\support\ace_wrappers\ace\free_list.cpp"\
	"..\support\ace_wrappers\ace\free_list.h"\
	"..\support\ace_wrappers\ace\free_list.i"\
	"..\support\ace_wrappers\ace\functor.h"\
	"..\support\ace_wrappers\ace\functor.i"\
	"..\support\ace_wrappers\ace\functor_t.cpp"\
	"..\support\ace_wrappers\ace\functor_t.h"\
	"..\support\ace_wrappers\ace\functor_t.i"\
	"..\support\ace_wrappers\ace\handle_set.h"\
	"..\support\ace_wrappers\ace\handle_set.i"\
	"..\support\ace_wrappers\ace\hash_map_manager.h"\
	"..\support\ace_wrappers\ace\hash_map_manager.i"\
	"..\support\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\support\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\support\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\support\ace_wrappers\ace\inc_user_config.h"\
	"..\support\ace_wrappers\ace\inet_addr.h"\
	"..\support\ace_wrappers\ace\inet_addr.i"\
	"..\support\ace_wrappers\ace\io_cntl_msg.h"\
	"..\support\ace_wrappers\ace\iosfwd.h"\
	"..\support\ace_wrappers\ace\ipc_sap.h"\
	"..\support\ace_wrappers\ace\ipc_sap.i"\
	"..\support\ace_wrappers\ace\local_tokens.h"\
	"..\support\ace_wrappers\ace\local_tokens.i"\
	"..\support\ace_wrappers\ace\log_msg.h"\
	"..\support\ace_wrappers\ace\log_priority.h"\
	"..\support\ace_wrappers\ace\log_record.h"\
	"..\support\ace_wrappers\ace\log_record.i"\
	"..\support\ace_wrappers\ace\Malloc.h"\
	"..\support\ace_wrappers\ace\malloc.i"\
	"..\support\ace_wrappers\ace\malloc_base.h"\
	"..\support\ace_wrappers\ace\malloc_t.cpp"\
	"..\support\ace_wrappers\ace\malloc_t.h"\
	"..\support\ace_wrappers\ace\malloc_t.i"\
	"..\support\ace_wrappers\ace\managed_object.cpp"\
	"..\support\ace_wrappers\ace\managed_object.h"\
	"..\support\ace_wrappers\ace\managed_object.i"\
	"..\support\ace_wrappers\ace\map_manager.cpp"\
	"..\support\ace_wrappers\ace\map_manager.h"\
	"..\support\ace_wrappers\ace\map_manager.i"\
	"..\support\ace_wrappers\ace\mem_map.h"\
	"..\support\ace_wrappers\ace\mem_map.i"\
	"..\support\ace_wrappers\ace\memory_pool.h"\
	"..\support\ace_wrappers\ace\memory_pool.i"\
	"..\support\ace_wrappers\ace\message_block.h"\
	"..\support\ace_wrappers\ace\message_block.i"\
	"..\support\ace_wrappers\ace\message_block_t.cpp"\
	"..\support\ace_wrappers\ace\message_block_t.h"\
	"..\support\ace_wrappers\ace\message_block_t.i"\
	"..\support\ace_wrappers\ace\message_queue.h"\
	"..\support\ace_wrappers\ace\message_queue.i"\
	"..\support\ace_wrappers\ace\message_queue_t.cpp"\
	"..\support\ace_wrappers\ace\message_queue_t.h"\
	"..\support\ace_wrappers\ace\message_queue_t.i"\
	"..\support\ace_wrappers\ace\method_object.h"\
	"..\support\ace_wrappers\ace\method_request.h"\
	"..\support\ace_wrappers\ace\module.cpp"\
	"..\support\ace_wrappers\ace\module.h"\
	"..\support\ace_wrappers\ace\module.i"\
	"..\support\ace_wrappers\ace\object_manager.h"\
	"..\support\ace_wrappers\ace\object_manager.i"\
	"..\support\ace_wrappers\ace\os.h"\
	"..\support\ace_wrappers\ace\os.i"\
	"..\support\ace_wrappers\ace\reactor.h"\
	"..\support\ace_wrappers\ace\reactor.i"\
	"..\support\ace_wrappers\ace\reactor_impl.h"\
	"..\support\ace_wrappers\ace\service_config.h"\
	"..\support\ace_wrappers\ace\service_config.i"\
	"..\support\ace_wrappers\ace\service_object.h"\
	"..\support\ace_wrappers\ace\service_object.i"\
	"..\support\ace_wrappers\ace\service_repository.h"\
	"..\support\ace_wrappers\ace\service_repository.i"\
	"..\support\ace_wrappers\ace\service_types.h"\
	"..\support\ace_wrappers\ace\service_types.i"\
	"..\support\ace_wrappers\ace\shared_object.h"\
	"..\support\ace_wrappers\ace\shared_object.i"\
	"..\support\ace_wrappers\ace\Signal.h"\
	"..\support\ace_wrappers\ace\signal.i"\
	"..\support\ace_wrappers\ace\singleton.cpp"\
	"..\support\ace_wrappers\ace\singleton.h"\
	"..\support\ace_wrappers\ace\singleton.i"\
	"..\support\ace_wrappers\ace\sock.h"\
	"..\support\ace_wrappers\ace\sock.i"\
	"..\support\ace_wrappers\ace\sock_connector.h"\
	"..\support\ace_wrappers\ace\sock_connector.i"\
	"..\support\ace_wrappers\ace\sock_io.h"\
	"..\support\ace_wrappers\ace\sock_io.i"\
	"..\support\ace_wrappers\ace\sock_stream.h"\
	"..\support\ace_wrappers\ace\sock_stream.i"\
	"..\support\ace_wrappers\ace\sstring.h"\
	"..\support\ace_wrappers\ace\sstring.i"\
	"..\support\ace_wrappers\ace\strategies.h"\
	"..\support\ace_wrappers\ace\strategies.i"\
	"..\support\ace_wrappers\ace\strategies_t.cpp"\
	"..\support\ace_wrappers\ace\strategies_t.h"\
	"..\support\ace_wrappers\ace\strategies_t.i"\
	"..\support\ace_wrappers\ace\stream_modules.cpp"\
	"..\support\ace_wrappers\ace\stream_modules.h"\
	"..\support\ace_wrappers\ace\stream_modules.i"\
	"..\support\ace_wrappers\ace\streams.h"\
	"..\support\ace_wrappers\ace\SV_Semaphore_Complex.h"\
	"..\support\ace_wrappers\ace\SV_Semaphore_Complex.i"\
	"..\support\ace_wrappers\ace\SV_Semaphore_Simple.h"\
	"..\support\ace_wrappers\ace\SV_Semaphore_Simple.i"\
	"..\support\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\support\ace_wrappers\ace\svc_handler.cpp"\
	"..\support\ace_wrappers\ace\svc_handler.h"\
	"..\support\ace_wrappers\ace\svc_handler.i"\
	"..\support\ace_wrappers\ace\synch.h"\
	"..\support\ace_wrappers\ace\synch.i"\
	"..\support\ace_wrappers\ace\synch_options.h"\
	"..\support\ace_wrappers\ace\synch_options.i"\
	"..\support\ace_wrappers\ace\synch_t.cpp"\
	"..\support\ace_wrappers\ace\synch_t.h"\
	"..\support\ace_wrappers\ace\synch_t.i"\
	"..\support\ace_wrappers\ace\task.h"\
	"..\support\ace_wrappers\ace\task.i"\
	"..\support\ace_wrappers\ace\task_t.cpp"\
	"..\support\ace_wrappers\ace\task_t.h"\
	"..\support\ace_wrappers\ace\task_t.i"\
	"..\support\ace_wrappers\ace\thread.h"\
	"..\support\ace_wrappers\ace\thread.i"\
	"..\support\ace_wrappers\ace\thread_manager.h"\
	"..\support\ace_wrappers\ace\thread_manager.i"\
	"..\support\ace_wrappers\ace\time_value.h"\
	"..\support\ace_wrappers\ace\timer_queue.h"\
	"..\support\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\support\ace_wrappers\ace\timer_queue_t.h"\
	"..\support\ace_wrappers\ace\timer_queue_t.i"\
	"..\support\ace_wrappers\ace\trace.h"\
	"..\support\ace_wrappers\ace\wfmo_reactor.h"\
	"..\support\ace_wrappers\ace\wfmo_reactor.i"\
	"..\support\ace_wrappers\ace\ws2tcpip.h"\
	"..\support\zlib-1.1.3\zconf.h"\
	"..\support\zlib-1.1.3\zlib.h"\
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
	".\include\pch.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\nickserv.obj" : $(SOURCE) $(DEP_CPP_NICKS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_NICKS=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\addr.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\connector.cpp"\
	"..\ace_wrappers\ace\connector.h"\
	"..\ace_wrappers\ace\connector.i"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\dynamic.h"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\inet_addr.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\ipc_sap.h"\
	"..\ace_wrappers\ace\ipc_sap.i"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\singleton.cpp"\
	"..\ace_wrappers\ace\singleton.h"\
	"..\ace_wrappers\ace\singleton.i"\
	"..\ace_wrappers\ace\sock.h"\
	"..\ace_wrappers\ace\sock.i"\
	"..\ace_wrappers\ace\sock_connector.h"\
	"..\ace_wrappers\ace\sock_connector.i"\
	"..\ace_wrappers\ace\sock_io.h"\
	"..\ace_wrappers\ace\sock_io.i"\
	"..\ace_wrappers\ace\sock_stream.h"\
	"..\ace_wrappers\ace\sock_stream.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\svc_handler.cpp"\
	"..\ace_wrappers\ace\svc_handler.h"\
	"..\ace_wrappers\ace\svc_handler.i"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\time_value.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\activation_queue.i"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers.i"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\event_handler.i"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor.i"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\handle_set.i"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager.i"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\mem_map.i"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\memory_pool.i"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block.i"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue.i"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor.i"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_config.i"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_object.i"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_repository.i"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\service_types.i"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\shared_object.i"\
	"..\ace_wrappers\ace\signal.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies.i"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch.i"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_options.i"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task.i"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread.i"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\thread_manager.i"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	"..\ace_wrappers\ace\wfmo_reactor.i"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\textfile.obj" : $(SOURCE) $(DEP_CPP_TEXTF) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_TEXTF=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\activation_queue.i"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\auto_ptr.cpp"\
	"..\ace_wrappers\ace\auto_ptr.h"\
	"..\ace_wrappers\ace\auto_ptr.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers.i"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\event_handler.i"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor.i"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\handle_set.i"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager.i"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\local_tokens.i"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\mem_map.i"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\memory_pool.i"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block.i"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue.i"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor.i"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_config.i"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_object.i"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_repository.i"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\service_types.i"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\shared_object.i"\
	"..\ace_wrappers\ace\signal.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies.i"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch.i"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_options.i"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task.i"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread.i"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\thread_manager.i"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	"..\ace_wrappers\ace\wfmo_reactor.i"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\lockable.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_TRACE=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\auto_ptr.cpp"\
	"..\ace_wrappers\ace\auto_ptr.h"\
	"..\ace_wrappers\ace\auto_ptr.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\local_tokens.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\map_manager.cpp"\
	"..\ace_wrappers\ace\map_manager.h"\
	"..\ace_wrappers\ace\map_manager.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\activation_queue.i"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers.i"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\event_handler.i"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor.i"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\handle_set.i"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager.i"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\mem_map.i"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\memory_pool.i"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block.i"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue.i"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor.i"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_config.i"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_object.i"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_repository.i"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\service_types.i"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\shared_object.i"\
	"..\ace_wrappers\ace\signal.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies.i"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch.i"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_options.i"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task.i"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread.i"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\thread_manager.i"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	"..\ace_wrappers\ace\wfmo_reactor.i"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	".\include\utils.h"\
	

"$(INTDIR)\utils.obj" : $(SOURCE) $(DEP_CPP_UTILS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_UTILS=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\activation_queue.i"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\basic_types.i"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers.i"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\event_handler.i"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor.i"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\handle_set.i"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager.i"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\mem_map.i"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\memory_pool.i"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block.i"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue.i"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\object_manager.i"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\os.i"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor.i"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_config.i"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_object.i"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_repository.i"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\service_types.i"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\shared_object.i"\
	"..\ace_wrappers\ace\signal.i"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\sstring.i"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies.i"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch.i"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_options.i"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task.i"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread.i"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\thread_manager.i"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
	"..\ace_wrappers\ace\wfmo_reactor.i"\
	".\include\confbase.h"\
	".\include\datetime.h"\
	".\include\fileconf.h"\
	".\include\log.h"\
	".\include\mstream.h"\
	".\include\mstring.h"\
	".\include\textfile.h"\
	".\include\trace.h"\
	

"$(INTDIR)\variant.obj" : $(SOURCE) $(DEP_CPP_VARIA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"

DEP_CPP_VARIA=\
	"..\ace_wrappers\ace\ace.h"\
	"..\ace_wrappers\ace\ace.i"\
	"..\ace_wrappers\ace\activation_queue.h"\
	"..\ace_wrappers\ace\atomic_op.i"\
	"..\ace_wrappers\ace\basic_types.h"\
	"..\ace_wrappers\ace\config-win32-borland.h"\
	"..\ace_wrappers\ace\config-win32-common.h"\
	"..\ace_wrappers\ace\config-win32.h"\
	"..\ace_wrappers\ace\config.h"\
	"..\ace_wrappers\ace\containers.h"\
	"..\ace_wrappers\ace\containers_t.cpp"\
	"..\ace_wrappers\ace\containers_t.h"\
	"..\ace_wrappers\ace\containers_t.i"\
	"..\ace_wrappers\ace\event_handler.h"\
	"..\ace_wrappers\ace\free_list.cpp"\
	"..\ace_wrappers\ace\free_list.h"\
	"..\ace_wrappers\ace\free_list.i"\
	"..\ace_wrappers\ace\functor.h"\
	"..\ace_wrappers\ace\functor_t.cpp"\
	"..\ace_wrappers\ace\functor_t.h"\
	"..\ace_wrappers\ace\functor_t.i"\
	"..\ace_wrappers\ace\handle_set.h"\
	"..\ace_wrappers\ace\hash_map_manager.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.cpp"\
	"..\ace_wrappers\ace\hash_map_manager_t.h"\
	"..\ace_wrappers\ace\hash_map_manager_t.i"\
	"..\ace_wrappers\ace\inc_user_config.h"\
	"..\ace_wrappers\ace\io_cntl_msg.h"\
	"..\ace_wrappers\ace\log_msg.h"\
	"..\ace_wrappers\ace\log_priority.h"\
	"..\ace_wrappers\ace\log_record.h"\
	"..\ace_wrappers\ace\log_record.i"\
	"..\ace_wrappers\ace\malloc_base.h"\
	"..\ace_wrappers\ace\malloc_t.cpp"\
	"..\ace_wrappers\ace\malloc_t.h"\
	"..\ace_wrappers\ace\malloc_t.i"\
	"..\ace_wrappers\ace\managed_object.cpp"\
	"..\ace_wrappers\ace\managed_object.h"\
	"..\ace_wrappers\ace\managed_object.i"\
	"..\ace_wrappers\ace\mem_map.h"\
	"..\ace_wrappers\ace\memory_pool.h"\
	"..\ace_wrappers\ace\message_block.h"\
	"..\ace_wrappers\ace\message_block_t.cpp"\
	"..\ace_wrappers\ace\message_block_t.h"\
	"..\ace_wrappers\ace\message_block_t.i"\
	"..\ace_wrappers\ace\message_queue.h"\
	"..\ace_wrappers\ace\message_queue_t.cpp"\
	"..\ace_wrappers\ace\message_queue_t.h"\
	"..\ace_wrappers\ace\message_queue_t.i"\
	"..\ace_wrappers\ace\method_object.h"\
	"..\ace_wrappers\ace\method_request.h"\
	"..\ace_wrappers\ace\module.cpp"\
	"..\ace_wrappers\ace\module.h"\
	"..\ace_wrappers\ace\module.i"\
	"..\ace_wrappers\ace\object_manager.h"\
	"..\ace_wrappers\ace\os.h"\
	"..\ace_wrappers\ace\reactor.h"\
	"..\ace_wrappers\ace\reactor_impl.h"\
	"..\ace_wrappers\ace\service_config.h"\
	"..\ace_wrappers\ace\service_object.h"\
	"..\ace_wrappers\ace\service_repository.h"\
	"..\ace_wrappers\ace\service_types.h"\
	"..\ace_wrappers\ace\shared_object.h"\
	"..\ace_wrappers\ace\sstring.h"\
	"..\ace_wrappers\ace\strategies.h"\
	"..\ace_wrappers\ace\strategies_t.cpp"\
	"..\ace_wrappers\ace\strategies_t.h"\
	"..\ace_wrappers\ace\strategies_t.i"\
	"..\ace_wrappers\ace\stream_modules.cpp"\
	"..\ace_wrappers\ace\stream_modules.h"\
	"..\ace_wrappers\ace\stream_modules.i"\
	"..\ace_wrappers\ace\streams.h"\
	"..\ace_wrappers\ace\svc_conf_tokens.h"\
	"..\ace_wrappers\ace\synch.h"\
	"..\ace_wrappers\ace\synch_options.h"\
	"..\ace_wrappers\ace\synch_t.cpp"\
	"..\ace_wrappers\ace\synch_t.h"\
	"..\ace_wrappers\ace\synch_t.i"\
	"..\ace_wrappers\ace\task.h"\
	"..\ace_wrappers\ace\task_t.cpp"\
	"..\ace_wrappers\ace\task_t.h"\
	"..\ace_wrappers\ace\task_t.i"\
	"..\ace_wrappers\ace\thread.h"\
	"..\ace_wrappers\ace\thread_manager.h"\
	"..\ace_wrappers\ace\timer_queue.h"\
	"..\ace_wrappers\ace\timer_queue_t.cpp"\
	"..\ace_wrappers\ace\timer_queue_t.h"\
	"..\ace_wrappers\ace\timer_queue_t.i"\
	"..\ace_wrappers\ace\trace.h"\
	"..\ace_wrappers\ace\wfmo_reactor.h"\
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

