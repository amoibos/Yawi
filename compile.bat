@echo off
setlocal
call :setESC
REM set output=yawi
for %%I in (.) do set output=%%~nxI

REM set DEMOFLAG=-D DEMO
set DEMOFLAG=

set TARGET_PLATFORM="SC" 
REM set TARGET_PLATFORM="WINDOWS" 

if %TARGET_PLATFORM% == "WINDOWS" 	set compiler=gcc
if %TARGET_PLATFORM% == "SMS" 		set compiler=sdcc
if %TARGET_PLATFORM% == "SG" 		set compiler=sdcc
if %TARGET_PLATFORM% == "SC" 		set compiler=sdcc

if %TARGET_PLATFORM% == "SMS" 		set FLAGS=-DPLATFORM_SMS %DEMOFLAG% -DGAME_NAME=\"%output%\"
if %TARGET_PLATFORM% == "SG"  		set FLAGS=-DPLATFORM_SG %DEMOFLAG% -DGAME_NAME=\"%output%\"
if %TARGET_PLATFORM% == "SC"  		set FLAGS=-DPLATFORM_SG %DEMOFLAG% -DGAME_NAME=\"%output%\" --debug
if %TARGET_PLATFORM% == "WINDOWS" 	set FLAGS=-g -DPLATFORM_WINDOWS %DEMOFLAG% -DGAME_NAME=\"%output%\" -fno-builtin -Wno-implicit-function-declaration

set linker=ihx2sms
REM set linker=makesms

set res2src=folder2c

set mainentry=main

call print_phase Cleanup directory..
call clean %output%

:compile

call print_phase Creating source files for %TARGET_PLATFORM% from binary..

set command=echo.
if %TARGET_PLATFORM% == "SMS" set command=%res2src% assets/font_SMS assets/font
if %TARGET_PLATFORM% == "SG"  set command=%res2src% assets/font_SG assets/font
if %TARGET_PLATFORM% == "SC"  set command=%res2src% assets/font_SG assets/font

call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

if %TARGET_PLATFORM% == "SMS" set command=%res2src% assets/levels assets/levels
if %TARGET_PLATFORM% == "SG"  set command=%res2src% assets/levels assets/levels
if %TARGET_PLATFORM% == "SC"  set command=%res2src% assets/levels assets/levels

call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

if %TARGET_PLATFORM% == "SMS" set command=%res2src% assets/images assets/images
if %TARGET_PLATFORM% == "SG"  set command=%res2src% assets/images assets/images
if %TARGET_PLATFORM% == "SC"  set command=%res2src% assets/images assets/images

call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

if %TARGET_PLATFORM% == "SMS" set command=%res2src% assets/audio assets/audio
if %TARGET_PLATFORM% == "SG"  set command=%res2src% assets/audio assets/audio
if %TARGET_PLATFORM% == "SC"  set command=%res2src% assets/audio assets/audio

call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

call print_phase Compiling Helpers..

if %TARGET_PLATFORM% == "SMS" goto COMPILE_HELPERS_SMS 
if %TARGET_PLATFORM% == "SG"  goto COMPILE_HELPERS_SMS
if %TARGET_PLATFORM% == "SC"  goto COMPILE_HELPERS_SMS

REM fallback when platform is not SG or SMS

call print_exec %compiler% %FLAGS% -c libs/SMScompat.c
if not %ERRORLEVEL% == 0 goto error

goto COMPILE_ASSETS

:COMPILE_HELPERS_SMS
call print_exec %compiler% %FLAGS% -c -mz80 libs/console.c
if not %ERRORLEVEL% == 0 goto error

:COMPILE_MISC
for %%X in (widgets animation views engine) do (
call print_exec %compiler% %FLAGS% -c -mz80 %%X.c
if not %ERRORLEVEL% == 0 goto error
)

:COMPILE_ASSETS
call print_phase Compiling assets and string functions..

if %TARGET_PLATFORM% == "WINDOWS" goto COMPILE_HELPERS_WINDOWS


for %%X in (libs/console libs/strings assets/font assets/levels assets/images assets/audio) do (
call print_exec %compiler% %FLAGS% -c -mz80 %%X.c
if not %ERRORLEVEL% == 0 goto error
)

goto COMPILE_MAIN

:COMPILE_HELPERS_WINDOWS

call print_phase Compiling helpers for Windows..

call print_exec %compiler% %FLAGS% -c libs/strings.c
if not %ERRORLEVEL% == 0 goto error

for %%X in (assets/font asset/level) do (
set command=%compiler% %FLAGS% -c %%X.c
call print_exec %command%
if not %ERRORLEVEL% == 0 goto error
)
set command=%compiler% %FLAGS% -LC:\PDCurses\wincon -lcurses -c libs/SMScompat.c
call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

:COMPILE_MAIN
call print_phase Compiling %mainentry%..

set command=echo.
if %TARGET_PLATFORM% == "SMS"   		set command=%compiler% %FLAGS% -c -mz80 --peep-file peep-rules.txt -Ilibs -Isrc %mainentry%.c
if %TARGET_PLATFORM% == "SG"    		set command=%compiler% %FLAGS% -c -mz80 --peep-file peep-rules.txt -Ilibs -Isrc %mainentry%.c
if %TARGET_PLATFORM% == "SC"    		set command=%compiler% %FLAGS% -c -mz80 --peep-file peep-rules.txt -Ilibs -Isrc %mainentry%.c
if %TARGET_PLATFORM% == "WINDOWS"   	set command=%compiler% %FLAGS% -c %mainentry%.c

call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

call print_phase Linking..

set command=echo.
if %TARGET_PLATFORM% == "SMS" 		set command=%compiler% %FLAGS% -o %output%.ihx -mz80 --no-std-crt0 --data-loc 0xC000 crt0/crt0_sms.rel images.rel font.rel levels.rel console.rel strings.rel engine.rel views.rel widgets.rel %mainentry%.rel SMSlib/SMSlib.rel
if %TARGET_PLATFORM% == "SG"  		set command=%compiler% %FLAGS% -o %output%.ihx -mz80 --no-std-crt0 --data-loc 0xC000 -Wl-b_BANK1=0x18000 -Wl-b_BANK2=0x28000 -Wl-b_BANK3=0x38000 -Wl-b_BANK4=0x48000 crt0/crt0_sg.rel PSGlib/PSGlib.rel SGlib/SGlib.rel console.rel strings.rel engine.rel animation.rel views.rel widgets.rel %mainentry%.rel font.rel levels.rel images.rel audio.rel
if %TARGET_PLATFORM% == "SC"  		set command=%compiler% %FLAGS% -o %output%.ihx -mz80 --no-std-crt0 --data-loc 0xC000 crt0/crt0_sg.rel PSGlib/PSGlib.rel SGlib/SGlib.rel console.rel strings.rel engine.rel animation.rel views.rel widgets.rel %mainentry%.rel font.rel levels.rel images.rel audio.rel
REM --print-search-dirs
REM if %TARGET_PLATFORM% == "WINDOWS" %compiler% %FLAGS% -LC:\PDCurses\wincon -lcurses -o %output% %mainentry%.o assets.o SMScompat.o strings.o 
if %TARGET_PLATFORM% == "WINDOWS"	set command=%compiler% %FLAGS% -o %output% %mainentry%.o assets.o SMScompat.o strings.o -lncurses 

call print_exec %command%
if not %ERRORLEVEL% == 0 goto error

call print_phase Creating ROM..

set command=echo.
if %TARGET_PLATFORM% == "SMS"	  set command=%linker% %output%.ihx %output%.sms
if %TARGET_PLATFORM% == "SG" 	  set command=%linker% %output%.ihx %output%.sg
if %TARGET_PLATFORM% == "SC" 	  set command=%linker% %output%.ihx %output%.sc

call print_exec %command%
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
