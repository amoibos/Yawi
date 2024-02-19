@echo off
setlocal
call :setESC
set output=sg3keys


set TARGET_PLATFORM="SG" 
REM set TARGET_PLATFORM="WINDOWS" 

if %TARGET_PLATFORM% == "WINDOWS" 	set compiler=gcc
if %TARGET_PLATFORM% == "SMS" 		set compiler=sdcc
if %TARGET_PLATFORM% == "SG" 		set compiler=sdcc
if %TARGET_PLATFORM% == "SMS" 		set FLAGS=-D PLATFORM_SMS 
if %TARGET_PLATFORM% == "SG"  		set FLAGS=-D PLATFORM_SG 

if %TARGET_PLATFORM% == "WINDOWS" 	set FLAGS=-g -D PLATFORM_WINDOWS -fno-builtin -Wno-implicit-function-declaration

set mainentry=main

call print_phase Cleanup directory..
call clean %output%

:compile

call print_phase Creating source files for %TARGET_PLATFORM% from binary..

set command=echo.
if %TARGET_PLATFORM% == "SMS" set command=folder2c assets/font_SMS assets/assets
if %TARGET_PLATFORM% == "SG"  set command=folder2c assets/font_SG assets/assets

call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

call print_phase Compiling Helpers..

if %TARGET_PLATFORM% == "SMS" goto COMPILE_HELPERS_SMS 
if %TARGET_PLATFORM% == "SG"  goto COMPILE_HELPERS_SMS

REM fallback when platform is not SG or SMS
echo DEBUG %compiler% %FLAGS% -c libs/SMScompat.c 

call print_exec %compiler% %FLAGS% -c libs/SMScompat.c
if not %ERRORLEVEL% == 0 goto error

goto COMPILE_ASSETS

:COMPILE_HELPERS_SMS
call print_exec %compiler% %FLAGS% -c -mz80 libs/console.c
if not %ERRORLEVEL% == 0 goto error

:COMPILE_ASSETS
call print_phase Compiling assets and string functions..

if %TARGET_PLATFORM% == "WINDOWS" goto COMPILE_HELPERS_WINDOWS

call print_exec %compiler% %FLAGS% -c -mz80 libs/console.c
if not %ERRORLEVEL% == 0 goto error

call print_exec %compiler% %FLAGS% -c -mz80 libs/strings.c
if not %ERRORLEVEL% == 0 goto error

call print_exec %compiler% %FLAGS% -c -mz80 assets/assets.c
if not %ERRORLEVEL% == 0 goto error

goto COMPILE_MAIN

:COMPILE_HELPERS_WINDOWS

call print_phase Compiling helpers for Windows..

call print_exec %compiler% %FLAGS% -c libs/strings.c
if not %ERRORLEVEL% == 0 goto error

set command=%compiler% %FLAGS% -c assets/assets.c
call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

set command=%compiler% %FLAGS% -LC:\PDCurses\wincon -lcurses -c libs/SMScompat.c
call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

:COMPILE_MAIN
call print_phase Compiling %mainentry%..

set command=echo.
if %TARGET_PLATFORM% == "SMS" 		set command=%compiler% %FLAGS% -c -mz80 --peep-file peep-rules.txt -Ilibs -Isrc %mainentry%.c
if %TARGET_PLATFORM% == "SG" 		set command=%compiler% %FLAGS% -c -mz80 --peep-file peep-rules.txt -Ilibs -Isrc %mainentry%.c
if %TARGET_PLATFORM% == "WINDOWS" 	set command=%compiler% %FLAGS% -c %mainentry%.c

call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

call print_phase Linking..

set command=echo.
if %TARGET_PLATFORM% == "SMS" 		set command=%compiler% %FLAGS% -o %output%.ihx -mz80 --no-std-crt0 --data-loc 0xC000 crt0/crt0_sms.rel %mainentry%.rel assets.rel console.rel strings.rel src/SMSlib.rel
if %TARGET_PLATFORM% == "SG"  		set command=%compiler% %FLAGS% -o %output%.ihx -mz80 --no-std-crt0 --data-loc 0xC000 crt0/crt0_sg.rel %mainentry%.rel assets.rel console.rel strings.rel  src/SGlib.rel  
REM --print-search-dirs
REM if %TARGET_PLATFORM% == "WINDOWS" %compiler% %FLAGS% -LC:\PDCurses\wincon -lcurses -o %output% %mainentry%.o assets.o SMScompat.o strings.o 
if %TARGET_PLATFORM% == "WINDOWS" 	set command=%compiler% %FLAGS% -o %output% %mainentry%.o assets.o SMScompat.o strings.o -lncurses 

call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

call print_phase Creating ROM..

set command=echo.
if %TARGET_PLATFORM% == "SMS" 	set command=ihx2sms %output%.ihx %output%.sms
if %TARGET_PLATFORM% == "SG" 	set command=ihx2sms %output%.ihx %output%.sg

call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

call print_phase Starting binary..

set command=echo.
if %TARGET_PLATFORM% == "SMS" 		set command=call run %output% %TARGET_PLATFORM%
if %TARGET_PLATFORM% == "SG"  		set command=call run %output% %TARGET_PLATFORM%
if %TARGET_PLATFORM% == "WINDOWS"  	set command=call run %output% %TARGET_PLATFORM%  

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