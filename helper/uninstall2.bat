@ECHO OFF
REM Magick IRC Services
REM
REM $Id$
REM "(c) 1997-2003 Preston Elder <prez@magick.tm>"
REM "(c) 1998-2003 William King <ungod@magick.tm>"
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
if "%1" == "" GOTO NeedParam
set ALL_ARGS=%1
if "%2" == "" GOTO start
set ALL_ARGS=%ALL_ARGS% %2
if "%3" == "" GOTO start
set ALL_ARGS=%ALL_ARGS% %3
if "%4" == "" GOTO start
set ALL_ARGS=%ALL_ARGS% %4
if "%5" == "" GOTO start
set ALL_ARGS=%ALL_ARGS% %5
if "%6" == "" GOTO start
set ALL_ARGS=%ALL_ARGS% %6
if "%7" == "" GOTO start
set ALL_ARGS=%ALL_ARGS% %7
if "%8" == "" GOTO start
set ALL_ARGS=%ALL_ARGS% %8
if "%9" == "" GOTO start
set ALL_ARGS=%ALL_ARGS% %9
:start

IF NOT "%OS%" == "Windows_NT" GOTO CommonStartup
TITLE Magick IRC Services - Uninstall
set ALL_ARGS=%*

:CommonStartup
REM
REM If its NT, remove it as a service
REM
IF NOT "%OS%" == "Windows_NT" GOTO NotService

REM
REM Runing as NT Service
REM
ECHO Removing service ...
ECHO.
"%ALL_ARGS%\magick.exe" --config "%ALL_ARGS%\magick.ini" --service stop
IF NOT %ERRORLEVEL% == 0 GOTO Error
"%ALL_ARGS%\magick.exe" --config "%ALL_ARGS%\magick.ini" --service remove
IF NOT %ERRORLEVEL% == 0 GOTO Error

:NotService
IF NOT EXIST "%ALL_ARGS%\Uninst.isu" goto End
IF "%OS%" == "Windows_NT" GOTO NTDel
isuninst.exe -f"%ALL_ARGS%\Uninst.isu"
rem The above does not wait, so can't do the below.
rem deltree "%ALL_ARGS"
GOTO End

:NTDel
uninst.exe -f"%ALL_ARGS%\Uninst.isu"
ECHO You are about to remove all residual files in
rd /S "%ALL_ARGS%"
GOTO End

:NeedParam
ECHO You MUST specify the path where Magick is installed.
goto Error

:Error
ECHO.
echo Magick has NOT been uninstalled.
pause

:End
REM Delete ourselves!
del "%ALL_ARGS%\..\magick_uninstall.bat"
