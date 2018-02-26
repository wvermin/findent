@echo off
REM $Id: wfindent.bat 241 2018-01-22 14:28:18Z willem_vermin $
setlocal EnableDelayedExpansion EnableExtensions
if -%1-==-- (
  echo wfindent is a wrapper for findent
  echo usage: wfindent [ findent-args ] filenames
  echo example:
  echo wfindent -ifree -i4 *.f90
  echo for a complete list of findent-args, type
  echo   wfindent -h
  echo NOTE: flags that influence the indenting: use
  echo       only the short varieties preceded with
  echo       a single '-', like -i5 -Rr
  goto :EOF
)
set fargs=
:astart
if -%1-==-- goto :EOF
set aa=%1

if "%aa%" == "-v"                goto :SPECIALRUN
if "%aa%" == "--version"         goto :SPECIALRUN
if "%aa%" == "-h"                goto :SPECIALRUN
if "%aa%" == "--help"            goto :SPECIALRUN
if "%aa%" == "-H"                goto :SPECIALRUN
if "%aa%" == "--manpage"         goto :SPECIALRUN
if "%aa%" == "--vim_help"        goto :SPECIALRUN
if "%aa%" == "--vim-help"        goto :SPECIALRUN
if "%aa%" == "--vim_fortran"     goto :SPECIALRUN
if "%aa%" == "--vim-fortran"     goto :SPECIALRUN
if "%aa%" == "--vim_findent"     goto :SPECIALRUN
if "%aa%" == "--vim-findent"     goto :SPECIALRUN
if "%aa%" == "--gedit_help"      goto :SPECIALRUN
if "%aa%" == "--gedit-help"      goto :SPECIALRUN
if "%aa%" == "--gedit_external"  goto :SPECIALRUN
if "%aa%" == "--gedit-external"  goto :SPECIALRUN
if "%aa%" == "--gedit_plugin"    goto :SPECIALRUN
if "%aa%" == "--gedit-plugin"    goto :SPECIALRUN
if "%aa%" == "--gedit_plugin_py" goto :SPECIALRUN
if "%aa%" == "--gedit-plugin-py" goto :SPECIALRUN
if "%aa%" == "--emacs_help"      goto :SPECIALRUN
if "%aa%" == "--emacs-help"      goto :SPECIALRUN
if "%aa%" == "--emacs_findent"   goto :SPECIALRUN
if "%aa%" == "--emacs-findent"   goto :SPECIALRUN
if "%aa%" == "-q" (
   echo wfindent: flag -q ignored
   echo           use "findent -q < filename"

   shift
   goto :astart
)
if %aa:~0,2%==-- (
   echo Error: found flag: %aa%
   echo Use only short indenting flags like -i5 -Rr ...
   exit /b
)

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

:SPECIALRUN
findent %aa%
exit /b

