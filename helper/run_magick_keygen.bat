@ECHO OFF
REM Magick IRC Services
REM
REM $Id$
REM (c) 1997-2002 Preston Elder <prez@magick.tm>
REM (c) 1998-2002 William King <ungod@magick.tm>
REM
REM The above copywright may not be removed under any circumstances,
REM however it may be added to if any modifications are made to this
REM file.  All modified code must be clearly documented and labelled.
REM
REM This code is released under the Artistic License v2.0 or better.
REM The full text of this license should be contained in a file called
REM COPYING distributed with this code.  If this file does not exist,
REM it may be viewed here: http://www.magick.tm/m2/license.html
REM
REM =======================================================================
REM
REM For official changes (by the Magick Development Team),please
REM check the ChangeLog* files that come with this distribution.
REM
REM Third Party Changes (please include e-mail address):
REM
REM N/A
REM
REM =======================================================================

REM This is needed for win95/98/me
if "%9" == "" GOTO args_8
set ALL_ARGS=%ALL_ARGS% %9
:args_8
if "%8" == "" GOTO args_7
set ALL_ARGS=%ALL_ARGS% %8
:args_7
if "%7" == "" GOTO args_6
set ALL_ARGS=%ALL_ARGS% %7
:args_6
if "%6" == "" GOTO args_5
set ALL_ARGS=%ALL_ARGS% %6
:args_5
if "%5" == "" GOTO args_4
set ALL_ARGS=%ALL_ARGS% %5
:args_4
if "%4" == "" GOTO args_3
set ALL_ARGS=%ALL_ARGS% %4
:args_3
if "%3" == "" GOTO args_2
set ALL_ARGS=%ALL_ARGS% %3
:args_2
if "%2" == "" GOTO args_1
set ALL_ARGS=%ALL_ARGS% %2
:args_1
if "%1" == "" GOTO start
set ALL_ARGS=%ALL_ARGS% %1
:start

IF NOT "%OS%" == "Windows_NT" GOTO CommonStartup
TITLE Magick IRC Services - Key Generator
set ALL_ARGS=%*

:CommonStartup
IF NOT EXIST need_stamp.sem GOTO START_MAGICK
ECHO Stamping new random encryption keys on Magick binaries ...
magick_stampkeys.exe magick.exe magick_keygen.exe
del need_stamp.sem
:START_MAGICK
ECHO Launching Magick Key Generator ...
ECHO.
magick_keygen.exe %ALL_ARGS%
ECHO.
ECHO Execution has ended.
pause

