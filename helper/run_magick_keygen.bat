@ECHO OFF
IF NOT EXIST need_stamp.sem GOTO START_MAGICK
ECHO Stamping new random encryption keys on Magick binaries ...
magick_stampkeys.exe magick.exe magick_keygen.exe
del need_stamp.sem
:START_MAGICK
ECHO Launching Magick Key Generator ...
ECHO.
magick_keygen.exe %*
ECHO.
ECHO Execution has ended.
pause

