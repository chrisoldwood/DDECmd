@echo off
rem **********************************************************************
rem Smoke test script to invoke DDECmd with various command lines.
rem **********************************************************************

set exitCode=0

echo ----------------------------------------------------------------------

Debug\DDECmd --version
if errorlevel 1 call :test_failed & set exitCode=1

echo ----------------------------------------------------------------------

Debug\DDECmd --help
if errorlevel 1 call :test_failed & set exitCode=1

echo ----------------------------------------------------------------------

Debug\DDECmd servers
if errorlevel 1 call :test_failed & set exitCode=1

echo ----------------------------------------------------------------------

Debug\DDECmd request --server PROGMAN --topic PROGMAN --item Accessories
if errorlevel 1 call :test_failed & set exitCode=1

echo ----------------------------------------------------------------------

Debug\DDECmd request -l "PROGMAN|PROGMAN!Accessories" -of "%%d %%s %%t %%i %%v" -df "ddd dd MMM yy" -tf "hh':'mm tt"
if errorlevel 1 call :test_failed & set exitCode=1

if /i not "%exitCode%" == "0" goto failed

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

:test_failed
echo.
echo **********************************************************************
echo **********                 TEST FAILED                      **********
echo **********************************************************************
echo.
goto :eof
