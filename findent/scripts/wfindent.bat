@echo off
setlocal EnableDelayedExpansion EnableExtensions
set t=%*
set flags=--safe

:loop
for /f "tokens=1*" %%a in ("%t%") do (
   set files=%t%
   set t=%%b
   echo.%%a | find  ".">Nul && ( 
      goto :b
   )
   if "%%a" == "-"  (
      goto :b
   )
   set flags=%flags% %%a
)
if defined t goto :loop

:b
call :GETTEMPNAME
set tmpf1="%TEMP%.\%FILENAME%_f"
set tmpc1="%TEMP%.\%FILENAME%_c1"
set tmpc2="%TEMP%.\%FILENAME%_c2"
for %%a in (%files%) do (
   if "%%a" NEQ "-" (
      if exist %%a (
	 echo.indenting %flags% %%a
	 findent %flags% < %%a  > %tmpf1%
	 find /v /c "" < %%a > %tmpc1%
	 find /v /c "" < %tmpf1% > %tmpc2%
	 set /p counter1=<%tmpc1%
	 set /p counter2=<%tmpc2%
	 if !counter1! neq !counter2! ( 
	    echo something wrong, file '%%a' unmodified
	 ) else ( 
	    copy /Y %tmpf1% %%a >nul 
	 )
	 if exist %tmpf1% del %tmpf1%
	 if exist %tmpc1% del %tmpc1%
	 if exist %tmpc2% del %tmpc2%
      ) else (
	 echo.warning: cannot find file '%%a'
      )
   )
)

:GETTEMPNAME
for /f "delims=:. tokens=1-4" %%t in ("%TIME: =0%") do (
   set FILENAME=wf-%%t%%u%%v%%w
)
