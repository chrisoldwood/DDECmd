@echo off
rem **********************************************************************
rem Smoke test script to invoke DDECmd with various command lines.
rem **********************************************************************

echo ----------------------------------------------------------------------

Debug\DDECmd --version
if errorlevel 1 goto :failed

echo ----------------------------------------------------------------------

Debug\DDECmd --help
if errorlevel 1 goto :failed

echo ----------------------------------------------------------------------

Debug\DDECmd servers

echo ----------------------------------------------------------------------

Debug\DDECmd request --server PROGMAN --topic PROGMAN --item Accessories

:success
echo.
echo ======================================================================
echo All tests executed normally
echo ======================================================================
echo.
exit /b 0

:failed
echo.
echo **********************************************************************
echo One or more tests terminated abnormally
echo **********************************************************************
echo.
exit /b 1
