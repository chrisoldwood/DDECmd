@echo off
rem **********************************************************************
rem Smoke test script to invoke DDECmd with various command lines.
rem **********************************************************************
setlocal

:handle_help_request
if /i "%~1" == "-?"     call :usage & exit /b 0
if /i "%~1" == "--help" call :usage & exit /b 0

:check_args
if /i "%~1" == "" call :usage & exit /b 1

:set_build
if /i "%~1" == "debug"   set build=Debug
if /i "%~1" == "release" set build=Release
if /i "%build%"== "" (
	echo ERROR: Invalid build type "%~1"
	call :usage
	exit /b 0
)

set root=%~dp0
set folder=%root%%build%
set exitCode=0

echo ----------------------------------------------------------------------

%folder%\DDECmd --version
if %errorlevel% neq 0 call :test_failed & set exitCode=1

echo ----------------------------------------------------------------------

%folder%\DDECmd --help
if %errorlevel% neq 0 call :test_failed & set exitCode=1

echo ----------------------------------------------------------------------

%folder%\DDECmd servers
if %errorlevel% neq 0 call :test_failed & set exitCode=1

echo ----------------------------------------------------------------------

%folder%\DDECmd request --server PROGMAN --topic PROGMAN --item Accessories
if %errorlevel% neq 0 call :test_failed & set exitCode=1

echo ----------------------------------------------------------------------

%folder%\DDECmd request -l "PROGMAN|PROGMAN!Accessories" -of "%%d %%s %%t %%i %%v" -df "ddd dd MMM yy" -tf "hh':'mm tt"
if %errorlevel% neq 0 call :test_failed & set exitCode=1

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

:usage
echo.
echo Usage: %~n0 [debug ^| release]
echo.
echo e.g.   %~n0 debug 
goto :eof
