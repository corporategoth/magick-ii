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
	-@erase "$(INTDIR)\ANTLRException.obj"
	-@erase "$(INTDIR)\AST.obj"
	-@erase "$(INTDIR)\ASTFactory.obj"
	-@erase "$(INTDIR)\BitSet.obj"
	-@erase "$(INTDIR)\bob.obj"
	-@erase "$(INTDIR)\CharBuffer.obj"
	-@erase "$(INTDIR)\CharScanner.obj"
	-@erase "$(INTDIR)\CommonToken.obj"
	-@erase "$(INTDIR)\confbase.obj"
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
	-@erase "$(INTDIR)\TreeParser.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\magick.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\bob" /I "..\cryptlib21" /I "include" /D\
 "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
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
	"$(INTDIR)\ANTLRException.obj" \
	"$(INTDIR)\AST.obj" \
	"$(INTDIR)\ASTFactory.obj" \
	"$(INTDIR)\BitSet.obj" \
	"$(INTDIR)\bob.obj" \
	"$(INTDIR)\CharBuffer.obj" \
	"$(INTDIR)\CharScanner.obj" \
	"$(INTDIR)\CommonToken.obj" \
	"$(INTDIR)\confbase.obj" \
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
	"$(INTDIR)\TreeParser.obj" \
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

ALL : "$(OUTDIR)\magick.exe" "$(OUTDIR)\magick.bsc"

!ELSE 

ALL : "$(OUTDIR)\magick.exe" "$(OUTDIR)\magick.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ANTLRException.obj"
	-@erase "$(INTDIR)\ANTLRException.sbr"
	-@erase "$(INTDIR)\AST.obj"
	-@erase "$(INTDIR)\AST.sbr"
	-@erase "$(INTDIR)\ASTFactory.obj"
	-@erase "$(INTDIR)\ASTFactory.sbr"
	-@erase "$(INTDIR)\BitSet.obj"
	-@erase "$(INTDIR)\BitSet.sbr"
	-@erase "$(INTDIR)\bob.obj"
	-@erase "$(INTDIR)\bob.sbr"
	-@erase "$(INTDIR)\CharBuffer.obj"
	-@erase "$(INTDIR)\CharBuffer.sbr"
	-@erase "$(INTDIR)\CharScanner.obj"
	-@erase "$(INTDIR)\CharScanner.sbr"
	-@erase "$(INTDIR)\CommonToken.obj"
	-@erase "$(INTDIR)\CommonToken.sbr"
	-@erase "$(INTDIR)\confbase.obj"
	-@erase "$(INTDIR)\confbase.sbr"
	-@erase "$(INTDIR)\EscLexer.obj"
	-@erase "$(INTDIR)\EscLexer.sbr"
	-@erase "$(INTDIR)\EscParser.obj"
	-@erase "$(INTDIR)\EscParser.sbr"
	-@erase "$(INTDIR)\fileconf.obj"
	-@erase "$(INTDIR)\fileconf.sbr"
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
	-@erase "$(INTDIR)\TreeParser.obj"
	-@erase "$(INTDIR)\TreeParser.sbr"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\utils.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\magick.bsc"
	-@erase "$(OUTDIR)\magick.exe"
	-@erase "$(OUTDIR)\magick.ilk"
	-@erase "$(OUTDIR)\magick.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Gf /I "..\bob" /I\
 "..\cryptlib21" /I "include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS"\
 /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\magick.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ANTLRException.sbr" \
	"$(INTDIR)\AST.sbr" \
	"$(INTDIR)\ASTFactory.sbr" \
	"$(INTDIR)\BitSet.sbr" \
	"$(INTDIR)\bob.sbr" \
	"$(INTDIR)\CharBuffer.sbr" \
	"$(INTDIR)\CharScanner.sbr" \
	"$(INTDIR)\CommonToken.sbr" \
	"$(INTDIR)\confbase.sbr" \
	"$(INTDIR)\EscLexer.sbr" \
	"$(INTDIR)\EscParser.sbr" \
	"$(INTDIR)\fileconf.sbr" \
	"$(INTDIR)\LLkParser.sbr" \
	"$(INTDIR)\lockable.sbr" \
	"$(INTDIR)\log.sbr" \
	"$(INTDIR)\magick.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\MismatchedTokenException.sbr" \
	"$(INTDIR)\mstream.sbr" \
	"$(INTDIR)\mstring.sbr" \
	"$(INTDIR)\NoViableAltException.sbr" \
	"$(INTDIR)\Parser.sbr" \
	"$(INTDIR)\ParserException.sbr" \
	"$(INTDIR)\ScannerException.sbr" \
	"$(INTDIR)\String.sbr" \
	"$(INTDIR)\textfile.sbr" \
	"$(INTDIR)\Token.sbr" \
	"$(INTDIR)\TokenBuffer.sbr" \
	"$(INTDIR)\TreeParser.sbr" \
	"$(INTDIR)\utils.sbr"

"$(OUTDIR)\magick.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

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
	"$(INTDIR)\TreeParser.obj" \
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
SOURCE=.\src\antlr\ANTLRException.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\ANTLRException.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\ANTLRException.obj"	"$(INTDIR)\ANTLRException.sbr" : $(SOURCE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\AST.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\AST.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\AST.obj"	"$(INTDIR)\AST.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\ASTFactory.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\ASTFactory.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\ASTFactory.obj"	"$(INTDIR)\ASTFactory.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\BitSet.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\BitSet.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\BitSet.obj"	"$(INTDIR)\BitSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\bob.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\bob.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\bob.obj"	"$(INTDIR)\bob.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\CharBuffer.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\CharBuffer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\CharBuffer.obj"	"$(INTDIR)\CharBuffer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\CharScanner.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\CharScanner.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\CharScanner.obj"	"$(INTDIR)\CharScanner.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\CommonToken.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\CommonToken.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\CommonToken.obj"	"$(INTDIR)\CommonToken.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\confbase.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\confbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\confbase.obj"	"$(INTDIR)\confbase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\EscLexer.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\EscLexer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\EscLexer.obj"	"$(INTDIR)\EscLexer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\EscParser.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\EscParser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\EscParser.obj"	"$(INTDIR)\EscParser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\fileconf.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\fileconf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\fileconf.obj"	"$(INTDIR)\fileconf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\LLkParser.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\LLkParser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\LLkParser.obj"	"$(INTDIR)\LLkParser.sbr" : $(SOURCE) "$(INTDIR)"
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

SOURCE=.\src\log.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) "$(INTDIR)"
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

SOURCE=.\src\antlr\MismatchedTokenException.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\MismatchedTokenException.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\MismatchedTokenException.obj"\
	"$(INTDIR)\MismatchedTokenException.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\mstream.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\mstream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\mstream.obj"	"$(INTDIR)\mstream.sbr" : $(SOURCE) "$(INTDIR)"
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

SOURCE=.\src\antlr\NoViableAltException.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\NoViableAltException.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\NoViableAltException.obj"	"$(INTDIR)\NoViableAltException.sbr" : \
$(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\Parser.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\Parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\Parser.obj"	"$(INTDIR)\Parser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\ParserException.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\ParserException.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\ParserException.obj"	"$(INTDIR)\ParserException.sbr" : $(SOURCE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\ScannerException.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\ScannerException.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\ScannerException.obj"	"$(INTDIR)\ScannerException.sbr" : $(SOURCE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\String.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\String.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\String.obj"	"$(INTDIR)\String.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\textfile.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\textfile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\textfile.obj"	"$(INTDIR)\textfile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\Token.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\Token.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\Token.obj"	"$(INTDIR)\Token.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\TokenBuffer.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\TokenBuffer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\TokenBuffer.obj"	"$(INTDIR)\TokenBuffer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\antlr\TreeParser.cpp

!IF  "$(CFG)" == "magick - Win32 Release"


"$(INTDIR)\TreeParser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "magick - Win32 Debug"


"$(INTDIR)\TreeParser.obj"	"$(INTDIR)\TreeParser.sbr" : $(SOURCE) "$(INTDIR)"
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


!ENDIF 

