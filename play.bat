@echo off
setlocal
call :setESC
REM set output=yawi
for %%I in (.) do set output=%%~nxI


set TARGET_PLATFORM="SC" 

set command=echo.
if %TARGET_PLATFORM% == "SMS" 	  set command=call run %output% %TARGET_PLATFORM%
if %TARGET_PLATFORM% == "SG"  	  set command=call run %output% %TARGET_PLATFORM%
if %TARGET_PLATFORM% == "SC"  	  set command=call run %output% %TARGET_PLATFORM%
if %TARGET_PLATFORM% == "WINDOWS"	set command=call run %output% %TARGET_PLATFORM%  

call print_exec %command%
REM if not %ERRORLEVEL% == 0 goto error

goto exit
:setESC
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set ESC=%%b
  exit /B 0
)

:error
pause
:exit
