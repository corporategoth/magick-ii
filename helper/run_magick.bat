@ECHO OFF

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
goto NotAsService

:AsService
REM
REM Run as NT Service
REM
ECHO Launching Magick IRC Services ...
ECHO.
magick.exe --service insert %*
magick.exe --service start %*
goto End

:NotAsService
REM
REM Run sequentially (when --nofork is specified, or non-NT).
REM
ECHO Launching Magick IRC Services ...
ECHO.
magick.exe %*
ECHO.
ECHO Executed has ended.
pause
goto End

:End
