@echo off
setlocal
call :setESC
REM set output=yawi
for %%I in (.) do set output=%%~nxI

set TARGET_PLATFORM="SC" 

call compile
call play

goto exit
:setESC
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set ESC=%%b
  exit /B 0
)

:error
pause
:exit
