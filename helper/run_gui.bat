@ECHO OFF

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
java -jar mct.jar
goto end

REM
REM The End.
REM
:end
