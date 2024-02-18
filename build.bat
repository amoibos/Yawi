@echo off
setlocal
call :setESC
set output=sg3keys


set TARGET_PLATFORM="SG" 
REM set TARGET_PLATFORM="WINDOWS" 

if %TARGET_PLATFORM% == "WINDOWS" set compiler=gcc
if %TARGET_PLATFORM% == "SMS" set compiler=sdcc
if %TARGET_PLATFORM% == "SG" set compiler=sdcc
if %TARGET_PLATFORM% == "SMS" set FLAGS=-D PLATFORM_SMS 
if %TARGET_PLATFORM% == "SG"  set FLAGS=-D PLATFORM_SG 

if %TARGET_PLATFORM% == "WINDOWS" set FLAGS=-g -D PLATFORM_WINDOWS -fno-builtin -Wno-implicit-function-declaration

set mainentry=main

set TEXT=Cleanup directory..
echo %ESC%[105m%TEXT%%ESC%[0m%ESC%[0m
call clean %output%

:compile

set TEXT=Creating source files for %TARGET_PLATFORM% from binary..
echo %ESC%[105m%TEXT%%ESC%[0m%ESC%[0m

set command=echo.
if %TARGET_PLATFORM% == "SMS" set command=folder2c assets/font_SMS assets/assets
if %TARGET_PLATFORM% == "SG"  set command=folder2c assets/font_SG assets/assets

echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

set TEXT=Compiling Helpers..
echo %ESC%[105m%TEXT%%ESC%[0m%ESC%[0m

if %TARGET_PLATFORM% == "SMS" goto COMPILE_HELPERS_SMS 
if %TARGET_PLATFORM% == "SG"  goto COMPILE_HELPERS_SMS

REM fallback when platform is not SG or SMS
echo DEBUG %compiler% %FLAGS% -c libs/SMScompat.c 

set command=%compiler% %FLAGS% -c libs/SMScompat.c
echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

goto COMPILE_ASSETS

:COMPILE_HELPERS_SMS
set command=%compiler% %FLAGS% -c -mz80 libs/console.c
echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

:COMPILE_ASSETS
set TEXT=Compiling assets and string functions..
echo %ESC%[105m%TEXT%%ESC%[0m%ESC%[0m

if %TARGET_PLATFORM% == "WINDOWS" goto COMPILE_HELPERS_WINDOWS
set command=%compiler% %FLAGS% -c -mz80 libs/console.c
echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

set command=%compiler% %FLAGS% -c -mz80 libs/strings.c
echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

set command=%compiler% %FLAGS% -c -mz80 assets/assets.c
echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

goto COMPILE_MAIN

:COMPILE_HELPERS_WINDOWS

set TEXT=Compiling helpers for Windows..
echo %ESC%[105m%TEXT%%ESC%[0m%ESC%[0m

set command=%compiler% %FLAGS% -c libs/strings.c
echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

set command=%compiler% %FLAGS% -c assets/assets.c
echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

set command=%compiler% %FLAGS% -LC:\PDCurses\wincon -lcurses -c libs/SMScompat.c
echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

:COMPILE_MAIN
set TEXT=Compiling %mainentry%..
echo %ESC%[105m%TEXT%%ESC%[0m%ESC%[0m

set command=echo.
if %TARGET_PLATFORM% == "SMS" 		set command=%compiler% %FLAGS% -c -mz80 --peep-file peep-rules.txt -Ilibs -Isrc %mainentry%.c
if %TARGET_PLATFORM% == "SG" 		set command=%compiler% %FLAGS% -c -mz80 --peep-file peep-rules.txt -Ilibs -Isrc %mainentry%.c
if %TARGET_PLATFORM% == "WINDOWS" 	set command=%compiler% %FLAGS% -c %mainentry%.c

echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

set TEXT=Linking..
echo %ESC%[105m%TEXT%%ESC%[0m%ESC%[0m

set command=echo.
if %TARGET_PLATFORM% == "SMS" 		set command=%compiler% %FLAGS% -o %output%.ihx -mz80 --no-std-crt0 --data-loc 0xC000 crt0/crt0_sms.rel %mainentry%.rel assets.rel console.rel strings.rel src/SMSlib.rel
if %TARGET_PLATFORM% == "SG"  		set command=%compiler% %FLAGS% -o %output%.ihx -mz80 --no-std-crt0 --data-loc 0xC000 crt0/crt0_sg.rel %mainentry%.rel assets.rel console.rel strings.rel  src/SGlib.rel  
REM --print-search-dirs
REM if %TARGET_PLATFORM% == "WINDOWS" %compiler% %FLAGS% -LC:\PDCurses\wincon -lcurses -o %output% %mainentry%.o assets.o SMScompat.o strings.o 
if %TARGET_PLATFORM% == "WINDOWS" 	set command=%compiler% %FLAGS% -o %output% %mainentry%.o assets.o SMScompat.o strings.o -lncurses 

echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

set TEXT=Creating ROM..
echo %ESC%[105m%TEXT%%ESC%[0m%ESC%[0m

set command=echo.
if %TARGET_PLATFORM% == "SMS" 	set command=ihx2sms %output%.ihx %output%.sms
if %TARGET_PLATFORM% == "SG" 	set command=ihx2sms %output%.ihx %output%.sg

echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

set TEXT=Starting binary..
echo %ESC%[105m%TEXT%%ESC%[0m%ESC%[0m

set command=echo.
if %TARGET_PLATFORM% == "SMS" 		set command=call run %output% %TARGET_PLATFORM%
if %TARGET_PLATFORM% == "SG"  		set command=call run %output% %TARGET_PLATFORM%
if %TARGET_PLATFORM% == "WINDOWS"  	set command=call run %output% %TARGET_PLATFORM%  

echo %ESC%[32m%command%%ESC%[0m
%command%
if not %ERRORLEVEL% == 0 goto error

goto exit
:setESC
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set ESC=%%b
  exit /B 0
)

:error
pause
:exit
exit /B 0