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

TITLE Magick IRC Services
if "%*" == "" GOTO NeedParam

REM
REM If its NT, remove it as a service
REM
IF "%OS%" == "Windows_NT" GOTO AsService
GOTO NotAsService

:AsService
REM
REM Run as NT Service
REM
ECHO Removing service ...
ECHO.
"%*"\magick.exe --config "%*\magick.ini" --service stop
IF NOT %ERRORLEVEL% == 0 GOTO Error
"%*"\magick.exe --config "%*\magick.ini" --service remove %*
IF NOT %ERRORLEVEL% == 0 GOTO Error

:NotAsService
uninst.exe -f"%*\Uninst.isu"
ECHO You are about to remove all residual files in
rd /S "%*"
GOTO End

:NeedParam
ECHO You MUST specify the path where Magick is installed.

:Error
ECHO.
echo Magick has NOT been uninstalled.
pause

:End
