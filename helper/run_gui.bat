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
TITLE Magick IRC Services - Configuration GUI
set ALL_ARGS=%*

:CommonStartup
IF NOT EXIST "%ALL_ARGS%\files\pic" mkdir "%ALL_ARGS%\files\pic"
IF NOT EXIST "%ALL_ARGS%\files\memo" mkdir "%ALL_ARGS%\files\memo"
IF NOT EXIST "%ALL_ARGS%\files\public" mkdir "%ALL_ARGS%\files\public"
IF NOT EXIST "%ALL_ARGS%\files\temp" mkdir "%ALL_ARGS%\files\temp"

REM
REM Test if we have java installed ...
REM
java -version >nul 2>nul
IF ERRORLEVEL 0 goto have_java

REM
REM If not, run in HTML mode, we'll get the plugin
REM
config.html
goto end

REM
REM Otherwise, run in local mode, its much nicer :)
REM
:have_java
java -jar mct.jar "%ALL_ARGS%"
goto end

REM
REM The End.
REM
:end
