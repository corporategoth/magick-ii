@ECHO OFF

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
"%*"\magick.exe --service stop %*
IF NOT %ERRORLEVEL% == 0 GOTO Error
"%*"\magick.exe --service remove %*
IF NOT %ERRORLEVEL% == 0 GOTO Error

:NotAsService
uninst.exe -f"%*\Uninst.isu"
ECHO You are about to remove all residual files in
rmdir /S "%*"
GOTO End

:NeedParam
ECHO You MUST specify the path where Magick is installed.

:Error
ECHO.
echo Magick has NOT been uninstalled.
pause

:End
