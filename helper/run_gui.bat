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

IF NOT EXIST "%*\files\pic" mkdir "%*\files\pic"
IF NOT EXIST "%*\files\memo" mkdir "%*\files\memo"
IF NOT EXIST "%*\files\public" mkdir "%*\files\public"
IF NOT EXIST "%*\files\temp" mkdir "%*\files\temp"

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
java -jar mct.jar "%*"
goto end

REM
REM The End.
REM
:end
