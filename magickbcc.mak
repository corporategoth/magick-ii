# ---------------------------------------------------------------------------
!if !$d(BCB)
BCB = $(MAKEDIR)\..
!endif

# ---------------------------------------------------------------------------
# IDE SECTION
# ---------------------------------------------------------------------------
# The following section of the project makefile is managed by the BCB IDE.
# It is recommended to use the IDE to change any of the values in this
# section.
# ---------------------------------------------------------------------------

VERSION = BCB.05.03
# ---------------------------------------------------------------------------
PROJECT = magickbcc.exe
OBJFILES = Debug\magickbcc.obj Debug\base.obj Debug\chanserv.obj Debug\commserv.obj \
    Debug\datetime.obj Debug\dccengine.obj Debug\ircsocket.obj \
    Debug\lockable.obj Debug\magick.obj Debug\main.obj Debug\memoserv.obj \
    Debug\mstring.obj Debug\nickserv.obj Debug\operserv.obj Debug\server.obj \
    Debug\servmsg.obj Debug\trace.obj Debug\utils.obj Debug\variant.obj \
    Debug\filesys.obj Debug\mconfig.obj Debug\convert_magick.obj \
    Debug\convert_esper.obj Debug\convert_epona.obj Debug\stages.obj
RESFILES = 
MAINSOURCE = magickbcc.cpp
RESDEPEN = $(RESFILES)
LIBFILES = ..\support\zlib\zlib.LIB src\crypt\crypto.lib src\xml\xml.lib \
    ..\support\ACE_wrappers\bin\Dynamic\Debug\ace_bd.lib
IDLFILES = 
IDLGENFILES = 
LIBRARIES = vcl50.lib
PACKAGES = vcl50.bpi vcljpg50.bpi vclx50.bpi bcbsmp50.bpi vcldb50.bpi vclbde50.bpi \
    ibsmp50.bpi vcldbx50.bpi qrpt50.bpi teeui50.bpi teedb50.bpi tee50.bpi \
    dss50.bpi vclmid50.bpi nmfast50.bpi inetdb50.bpi inet50.bpi dclocx50.bpi \
    RXCTL4.bpi RXDB4.bpi Icsbcb40.bpi DragDropC4.bpi KBMMEMC4.bpi ABC45.bpi \
    ABCDB45.bpi A258_R41.bpi O305_R41.bpi O305BR41.bpi S204_R41.bpi S204BR41.bpi
SPARELIBS = vcl50.lib
DEFFILE = 
# ---------------------------------------------------------------------------
PATHCPP = .;src
PATHASM = .;
PATHPAS = .;
PATHRC = .;
DEBUGLIBPATH = $(BCB)\lib\debug
RELEASELIBPATH = $(BCB)\lib\release
USERDEFINES = _DEBUG;DEBUG;WIN32;_CONSOLE;_MBCS;DES_UNROLL;ACE_NTRACE=1;ACE_USE_RCSID=0;HAVE_CONFIG_H
SYSDEFINES = _NO_VCL;_ASSERTE;NO_STRICT;_RTLDLL;_VIS_NOLIB
INCLUDEPATH = src;include;..\support\ace_wrappers;..\support\zlib;..\include\sxp;..\include\expat;$(BCB)\include
LIBPATH = src;$(BCB)\lib
WARNINGS= -wstv -wstu -w-rvl -wpin -w-par -wnod -wnak -wdef -wamb
# ---------------------------------------------------------------------------
CFLAG1 = -Od -H=d:\PROGRA~1\borland\CBUILD~2\lib\vcl50.csm -Vmp -Tkh30000 -X- -r- \
    -a8 -5 -b -d -k -y -v -vi- -q -tWR -tWM -c
IDLCFLAGS = -Isrc -Iinclude -I..\support\ace_wrappers -I..\support\zlib \
    -I..\include\sxp -I..\include\expat -I$(BCB)\include -src_suffix cpp \
    -D_DEBUG -DDEBUG -DWIN32 -D_CONSOLE -D_MBCS -DDES_UNROLL -DACE_NTRACE=1 \
    -DACE_USE_RCSID=0 -DHAVE_CONFIG_H -boa
PFLAGS = -N2Debug -N0Debug -$YD -$W -$O-
RFLAGS = /l 0xc09 /d "_DEBUG" /i$(BCB)\include;$(BCB)\include\mfc
AFLAGS = /mx /w2 /zi
LFLAGS = -IDebug -D"" -H:0x1000000 -S:0x1000000 -ap -Tpe -GD -s -v -q
# ---------------------------------------------------------------------------
ALLOBJ = c0x32.obj ws2_32.lib $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) $(LIBRARIES) import32.lib cw32mti.lib
# ---------------------------------------------------------------------------
!ifdef IDEOPTIONS

[Version Info]
IncludeVerInfo=0
AutoIncBuild=0
MajorVer=1
MinorVer=0
Release=0
Build=0
Debug=0
PreRelease=0
Special=0
Private=0
DLL=0

[Version Info Keys]
CompanyName=
FileDescription=
FileVersion=1.0.0.0
InternalName=
LegalCopyright=
LegalTrademarks=
OriginalFilename=
ProductName=
ProductVersion=1.0.0.0
Comments=

[Debugging]
DebugSourceDirs=$(BCB)\source\vcl

!endif





# ---------------------------------------------------------------------------
# MAKE SECTION
# ---------------------------------------------------------------------------
# This section of the project file is not used by the BCB IDE.  It is for
# the benefit of building from the command-line using the MAKE utility.
# ---------------------------------------------------------------------------

.autodepend
# ---------------------------------------------------------------------------
!if "$(USERDEFINES)" != ""
AUSERDEFINES = -d$(USERDEFINES:;= -d)
!else
AUSERDEFINES =
!endif

!if !$d(BCC32)
BCC32 = bcc32
!endif

!if !$d(CPP32)
CPP32 = cpp32
!endif

!if !$d(DCC32)
DCC32 = dcc32
!endif

!if !$d(TASM32)
TASM32 = tasm32
!endif

!if !$d(LINKER)
LINKER = ilink32
!endif

!if !$d(BRCC32)
BRCC32 = brcc32
!endif


# ---------------------------------------------------------------------------
!if $d(PATHCPP)
.PATH.CPP = $(PATHCPP)
.PATH.C   = $(PATHCPP)
!endif

!if $d(PATHPAS)
.PATH.PAS = $(PATHPAS)
!endif

!if $d(PATHASM)
.PATH.ASM = $(PATHASM)
!endif

!if $d(PATHRC)
.PATH.RC  = $(PATHRC)
!endif
# ---------------------------------------------------------------------------
$(PROJECT): $(IDLGENFILES) $(OBJFILES) $(RESDEPEN) $(DEFFILE)
    $(BCB)\BIN\$(LINKER) @&&!
    $(LFLAGS) -L$(LIBPATH) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES)
!
# ---------------------------------------------------------------------------
.pas.hpp:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.pas.obj:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.cpp.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.cpp.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.asm.obj:
    $(BCB)\BIN\$(TASM32) $(AFLAGS) -i$(INCLUDEPATH:;= -i) $(AUSERDEFINES) -d$(SYSDEFINES:;= -d) $<, $@

.rc.res:
    $(BCB)\BIN\$(BRCC32) $(RFLAGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -fo$@ $<
# ---------------------------------------------------------------------------




