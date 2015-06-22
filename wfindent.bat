@echo off
REM $Id: wfindent.bat 65 2015-06-22 10:01:15Z willem_vermin $
setlocal EnableDelayedExpansion EnableExtensions
if -%1-==-- (
  echo wfindent is a wrapper for findent
  echo usage: wfindent [ findent-args ] filenames
  echo example:
  echo wfindent -ifree -i4 *.f90
  echo for a complete list of findent-args, type
  echo   findent -h
  goto :EOF
)
set fargs=
:astart
if -%1-==-- goto :EOF
set aa=%1
if %aa:~0,1%==- (
   set fargs=%fargs% %aa%
   shift
   goto astart
)

:bstart
if -%1-==-- goto :EOF
if exist %1 goto one
echo file %1 does not exist
goto :EOF
:one
call :GETTEMPNAME
set tmpf1="%TEMP%.\%FILENAME%_f"
set tmpc1="%TEMP%.\%FILENAME%_c1"
set tmpc2="%TEMP%.\%FILENAME%_c2"
for %%a in (%1) do ( 
   echo indenting %fargs% %%a
   findent %fargs% < %%a  > %tmpf1%
   find /v /c "" < %%a > %tmpc1%
   find /v /c "" < %tmpf1% > %tmpc2%
   set /p counter1=<%tmpc1%
   set /p counter2=<%tmpc2%
   if !counter1! neq !counter2! ( echo something wrong, file unmodified
   ) else ( copy /Y %tmpf1% %%a >nul )
   if exist %tmpf1% del %tmpf1%
   if exist %tmpc1% del %tmpc1%
   if exist %tmpc2% del %tmpc2%
)
shift
goto bstart

:GETTEMPNAME
for /f "delims=:. tokens=1-4" %%t in ("%TIME: =0%") do (
        set FILENAME=wf-%%t%%u%%v%%w
    )
exit /b

