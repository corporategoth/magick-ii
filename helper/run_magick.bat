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
REM
REM First we do the semaphore check, and stamp new binaries.
REM
IF NOT EXIST need_stamp.sem GOTO Check
ECHO Stamping new random encryption keys on Magick binaries ...
magick_stampkeys.exe magick.exe magick_keygen.exe
del need_stamp.sem

:Check
REM
REM Check for the --nofork option (just incase we're on NT)
REM
IF "%1" == "--nofork" GOTO NotAsService
SHIFT
IF NOT "%1" == "" GOTO Check

REM
REM No --nofork specified, so if its NT, we run as a service
REM
IF "%OS%" == "Windows_NT" GOTO AsService
GOTO NotAsService

:AsService
REM
REM Run as NT Service
REM
ECHO Launching Magick IRC Services ...
ECHO.
magick.exe --service insert %*
IF NOT %ERRORLEVEL% == 0 GOTO Error
magick.exe --service start %*
IF NOT %ERRORLEVEL% == 0 GOTO Error
GOTO End

:NotAsService
REM
REM Run sequentially (when --nofork is specified, or non-NT).
REM
ECHO Launching Magick IRC Services ...
ECHO.
magick.exe %*
IF NOT %ERRORLEVEL% == 0 GOTO Error
ECHO.
ECHO Executed has ended.
pause
GOTO End

:Error
ECHO.
echo Magick has NOT been started.
pause

:End
